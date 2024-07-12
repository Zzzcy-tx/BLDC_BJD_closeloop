/*********************************************************************************************************************
* CH32V203C8T6_BLDC_ESC 即（CH32V203C8T6 无感无刷电调）是一个基于官方 SDK 接口的第三方开源项目
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* CH32V203C8T6_BLDC_ESC 开源项目的一部分
*
* CH32V203C8T6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 bldc_config/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 bldc_config 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          control
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio
* 适用平台          CH32V203C8T6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期            作者         备注
* 2024-01-04        大W          first version
********************************************************************************************************************/

#include "control.h"
#include "operational_amplifier.h"
#include "pwm_output.h"
#include "motor.h"
#include "battery.h"
#include "stdlib.h"

static uint16 temp_commutation_time;

//-------------------------------------------------------------------------------------------------------------------
// @brief       换向和消磁铁中断回调函数
// @param       void
// @return      uint8         1-初始化失败 0-初始化成功
// @note        该函数在ch32v20x_it.c中的TIM3_IRQHandler中被调用
//-------------------------------------------------------------------------------------------------------------------
void commutation_and_degauss_callback()
{
    if(motor.run_flag == MOTOR_CLOSE_LOOP)
    {
        // 换向
        motor_next_step(CLOSE_COMP_PWM);

        // 使能比较器中断
        enable_opama_interrupts();

        // 去掉最早的数据
        motor.commutation_time_sum -= motor.commutation_time[motor.step];
        // 保存换相时间
        motor.commutation_time[motor.step] = temp_commutation_time;
        // 叠加新的换相时间，求6次换相总时长
        motor.commutation_time_sum += motor.commutation_time[motor.step];
        // 换相次数增加一
        motor.commutation_num++;
        // 一阶低通滤波
        motor.filter_commutation_time_sum = (motor.filter_commutation_time_sum * 7 + motor.commutation_time_sum * 1) >> 3;

        // 等待稳定，再开始换相错误判断
        if((BLDC_CLOSE_LOOP_WRITE * BLDC_POLES) < motor.commutation_num)
        {
            // 本次换向60度的时间，在上一次换向一圈时间的45度到75度，否则认为换相错误
            // (360.0f / 45) = 8
            // (360.0f / 75) = 4.8
            if((temp_commutation_time > ((motor.filter_commutation_time_sum >> 3))) && (temp_commutation_time < (motor.filter_commutation_time_sum / 4.8f)))
            {
                // 延时减去换向失败计数器
                if((motor.commutation_faile_count))
                {
                    motor.commutation_faile_count--;
                }
            }
            else
            {
                if(BLDC_COMMUTATION_FAILED_MAX < motor.commutation_faile_count++)
                {
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }
        }
    }
    TIM3->DMAINTENR &= ~TIM_IT_Update;  // 关闭定时器3更新中断
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       换向超时回调函数
// @param       void
// @return      uint8         1-初始化失败 0-初始化成功
// @note        该函数在ch32v20x_it.c中的TIM4_IRQHandler中被调用
//-------------------------------------------------------------------------------------------------------------------
void commutation_time_out_callback()
{
    if(motor.run_flag == MOTOR_CLOSE_LOOP)
    {
        if(motor.commutation_num > (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES))
        {
            // 如果是闭环换向，进入了这里，则代表堵转
            motor.run_flag = MOTOR_STOP_STALL;
        }
    }
    else
    {
        // 关闭定时器4更新中断
        TIM4->DMAINTENR &= ~TIM_IT_Update;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       运放作为比较器中断回调
// @param       void
// @return      uint8         1-初始化失败 0-初始化成功
// @note        该函数在ch32v20x_it.c中的EXTI2_IRQHandler中被调用
//              运放作为比较器输出到PA2、PA3、PA4引脚上面
//              PA2和PA3和PA4硬件上面连接在一起
//              当PA2引脚有上升沿或者下降沿的时候，就触发外部引脚中断
//-------------------------------------------------------------------------------------------------------------------
void opama_callback()
{
    // 获取本次换相时间
    temp_commutation_time = TIM4->CNT;

    // 去除反电动势毛刺
    if(temp_commutation_time < (motor.filter_commutation_time_sum / 12) && (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES) < motor.commutation_num)
    {
        return;
    }

    // 定时器清空
    TIM4->CNT = 0;
    TIM3->CNT = 0;
    TIM3->ATRLR = motor.filter_commutation_time_sum >> 4;     // 22.5度后开始换向
    // 开启定时器3，更新事件中断
    TIM3->INTFR = 0x00;
    TIM3->DMAINTENR |= TIM_IT_Update;

    disable_opama_interrupts();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       pit周期中断回调
// @param       void
// @return      void
// @note        该函数在ch32v20x_it.c中的SysTick_Handler中被调用
//              嘀嗒定时器被用作周期中断，就不能再作为延时使用了
//-------------------------------------------------------------------------------------------------------------------
void pit_callback(void)
{
    // 电池电压检测为最高优先级，当电压过低，就不转
    if(motor.run_flag != BYTE_LOW_VOLTAGE)
    {
        // 输入的占空比为0，则为空闲状态
        if(motor.duty == 0)
        {
            motor.run_flag = MOTOR_IDLE;
        }
        else
        {
            // 输入的占空比不为0，则从空闲状态转为电机开始
            if(motor.run_flag == MOTOR_IDLE)
            {
                motor.run_flag = MOTOR_START;
            }
        }
    }

    static uint16 stall_time_out_check = 0;
    static uint8  restart_count = 0;
    switch(motor.run_flag)
    {
        case MOTOR_START:
        {
            // 置随机数种子
            srand(battery.adc_reg_value);

            // 关闭定时器1，更新中断
            TIM1->DMAINTENR &= ~TIM_IT_Update;
            // 关闭定时器3，更新中断
            TIM3->DMAINTENR &= ~TIM_IT_Update;
            // 关闭定时器4，更新中断
            TIM4->DMAINTENR &= ~TIM_IT_Update;
            // 关闭PA2外部引脚中断
            disable_opama_interrupts();
            // 开始阶段
            // 等待10ms + 随机延时(0-50)ms，
            if(TIM4->CNT > (10 + rand()%50)*2000)
            {
                motor.duty_register = ((float)BLDC_START_VOLTAGE * BLDC_MAX_DUTY / ((float)battery.voltage/1000));
                if(motor.duty_register < (BLDC_MAX_DUTY / 25))
                {
                    motor.duty_register = (BLDC_MAX_DUTY / 25);
                }
                pwm_duty_update(motor.duty_register);
                TIM4->CNT = 0;
                // 清除一些变量的值
                motor_start();
                // 执行换向
                motor_next_step(CLOSE_COMP_PWM);
                motor.filter_commutation_time_sum = 0;
                stall_time_out_check = 0;
                motor.run_flag = MOTOR_OPEN_LOOP;
            }
        }
        break;
        case MOTOR_OPEN_LOOP:
        {
            // 开环强拉
            static uint8 gpio_state = 0;
            uint8 motor_next_step_flag = 0;
            uint16 temp_commutation_time = TIM4->CNT;
            // 通过位移的方式，将电平数据存入变量中
            gpio_state = (gpio_state << 1) | ((GPIOA->INDR & GPIO_Pin_2) ? 1 : 0);
            if ((motor.step % 2))
            {
                //下降沿
                if(gpio_state == 0)
                {
                    motor_next_step_flag = 1;
                    gpio_state = 0x03;
                }
            }
            else
            {
                // 上升沿
                if(gpio_state == 0xFF)
                {
                    motor_next_step_flag = 1;
                    gpio_state = 0xFC;
                }
            }
            if(motor_next_step_flag)
            {
                // 堵转计数清空
                stall_time_out_check = 0;
                // 定时器计数值清空
                TIM4->CNT = 0;
                // 去掉最早的数据
                motor.commutation_time_sum -= motor.commutation_time[motor.step];
                // 保存换相时间
                motor.commutation_time[motor.step] = temp_commutation_time;
                // 叠加新的换相时间，求6次换相总时长
                motor.commutation_time_sum += motor.commutation_time[motor.step];
                // 换相次数增加一
                motor.commutation_num++;
                // 一阶低通滤波
                motor.filter_commutation_time_sum = (motor.filter_commutation_time_sum * 3 + motor.commutation_time_sum * 1) >> 2;
                // 等待15度
                while(TIM4->CNT < (motor.filter_commutation_time_sum / 24));
                // 执行换向
                motor_next_step(CLOSE_COMP_PWM);
            }
            else
            {
                // 堵转检测
                if((10 * BLDC_STALL_TIME_OUT) < stall_time_out_check++)
                {
                    stall_time_out_check = 0;
                    motor.commutation_num = 0;
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }
            // 闭环状态切换
            if((motor.commutation_num > (BLDC_OPEN_LOOP_WRITE * BLDC_POLES)) && (motor.filter_commutation_time_sum < 40000))
            {
                // 清除一些变量
                motor.commutation_num = 0;
                // 堵转计数器清空
                stall_time_out_check = 0;
                // 切换为闭环状态
                motor.run_flag = MOTOR_CLOSE_LOOP;
                // 打开PA2外部引脚中断
                enable_opama_interrupts();
            }
            // 超过20ms + 随机延时(0-50)ms没有检测到跳变，则从进入电机开始状态
            if((TIM4->CNT > (20 + rand()%30)*2000) && (motor.run_flag == MOTOR_OPEN_LOOP))
            {
                TIM4->CNT = 0;
                motor_next_step(CLOSE_COMP_PWM);
                motor.run_flag = MOTOR_START;
            }
        }
        break;
        case MOTOR_CLOSE_LOOP:
        {
            static uint8 oled_pin_state = 0;
            uint8 pin_state = (GPIOA->INDR & GPIO_Pin_2);
            // 通过PA2引脚的电平状态，进行堵转检测
            if(oled_pin_state != pin_state)
            {
                stall_time_out_check = 0;
                oled_pin_state = pin_state;
            }
            else
            {
                // 如果引脚一直是一个状态，且超过了 BLDC_STALL_TIME_OUT ms 则认为堵转了
                if(stall_time_out_check++ > (10 * BLDC_STALL_TIME_OUT))          // 10KHZ，0.1ms计数一次
                {
                    stall_time_out_check = 0;
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }

            if(motor.commutation_num < (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES))
            {
                 if (motor.duty_register < BLDC_PWM_DEADTIME)
                 {
                     motor.duty_register = BLDC_PWM_DEADTIME;
                 }
                 if (motor.duty_register > (BLDC_MAX_DUTY / 5 + BLDC_PWM_DEADTIME))
                 {
                     motor.duty_register = BLDC_MAX_DUTY / 5 + BLDC_PWM_DEADTIME;
                 }
            }

            // 缓慢加减速
            if((motor.duty != motor.duty_register))
            {
                if(0 > motor.duty)
                {
                    motor.duty_register = 0;
                }

                if(motor.duty > motor.duty_register)
                {
                    motor.duty_register += BLDC_SPEED_INCREMENTAL;
                }
                else
                {
                    motor.duty_register = motor.duty;
                }

                pwm_duty_update(motor.duty_register);
            }
        }

        break;
        case MOTOR_IDLE:
        case BYTE_LOW_VOLTAGE:
        {
            motor_stop(BLDC_BRAKE_DUTY);
        }
        break;
        case MOTOR_STOP_STALL:
        {
            if (restart_count < BLDC_RESTART_MAX)
            {
                motor.run_flag = MOTOR_RESTART;
            }
            motor_stop(BLDC_BRAKE_DUTY);
        }
        break;
        case MOTOR_RESTART:
        {
            static int16 restart_delay = BLDC_RESTART_DELAY*10;
            if (restart_count < BLDC_RESTART_MAX)
            {
                if(restart_delay-- < 0)
                {
                    restart_delay = BLDC_RESTART_DELAY*10;
                    restart_count++;
                    motor.run_flag = MOTOR_IDLE;
                }
            }
        }
        break;
    }
}

void PID_Init(PID *pid, double setpoint, double kp, double ki, double kd) {
    pid->SetPoint = setpoint;
    pid->Proportion = kp;
    pid->Integral = ki;
    pid->Derivative = kd;
    pid->LastError = 0;
    pid->PreError = 0;
    pid->SumError = 0;
    pid->Output = 0;
}


void PID_Calculate(PID *pid, float current_value) {
    float error = pid->SetPoint - current_value;   // 计算误差
    pid->SumError += error;                         // 累计误差用于积分

    if((error>0 && pid->LastError<0) || (error<0 && pid->LastError>0)){
        pid->SumError = 0;
    }

    // 计算PID输出
    pid->Output = pid->Proportion * error                     // 比例项
                 + pid->Integral * pid->SumError              // 积分项
                 + pid->Derivative * (error - pid->LastError); // 微分项

    if (pid->Output <= 0 && pid->LastOutput <= 0 && clac_flag == 0){
        pid->Output = 0;
    } else if (pid->Output <= 0 && clac_flag == 1){
        pid->Output = 5;
    }


    // 更新误差变量
    pid->PreError = pid->LastError;
    pid->LastError = error;
    pid->LastOutput = pid->Output;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       周期中断初始化
// @param       void
// @return      void
// @note        使用嘀嗒定时器用作周期中断
//              嘀嗒定时器被用作周期中断，就不能再作为延时使用了
//-------------------------------------------------------------------------------------------------------------------
void pit_init(void)
{
    // 10Khz
    // 100us 进一次中断
    SysTick->CTLR = 0;
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = SystemCoreClock / 10000;
    SysTick->CTLR = 0xF;
    NVIC_SetPriority(SysTicK_IRQn, 0xE0);
    NVIC_EnableIRQ(SysTicK_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       提前换向计数初始化
// @param       void
// @return      void
// @note        使用定时器3
//-------------------------------------------------------------------------------------------------------------------
void advance_commutation_count_init(void)
{
    // 1 us计数一次
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2 /1000000) - 1;                    // 分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM3_IRQn, 0x00);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM_Cmd(TIM3, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       换向时间计数初始化
// @param       void
// @return      void
// @note        使用定时器4
//-------------------------------------------------------------------------------------------------------------------
void commutation_time_count_init(void)
{
    // 1 us计数一次
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2 / 1000000) - 1;                 // 分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                            // 重复计数器设置为0
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM4_IRQn, 0x02);
    NVIC_EnableIRQ(TIM4_IRQn);
    TIM_Cmd(TIM4, ENABLE);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       设置中断免表VTF响应
// @param       void
// @return      void
// @note        将中断设置为免查询中断向量表跳转
//              如果想了解免表中断，请自行查阅芯片手册
//-------------------------------------------------------------------------------------------------------------------
void set_vector_table_free(void)
{
    extern void TIM3_IRQHandler(void);
    extern void EXTI2_IRQHandler(void);
    extern void TIM1_UP_IRQHandler(void);
    extern void SysTick_Handler(void);
    SetVTFIRQ((uint32)EXTI2_IRQHandler,   EXTI2_IRQn,     0, ENABLE);
    SetVTFIRQ((uint32)TIM3_IRQHandler,    TIM3_IRQn,      1, ENABLE);
    SetVTFIRQ((uint32)TIM1_UP_IRQHandler, TIM1_UP_IRQn,   2, ENABLE);
    SetVTFIRQ((uint32)SysTick_Handler,    SysTicK_IRQn,   3, ENABLE);
}
