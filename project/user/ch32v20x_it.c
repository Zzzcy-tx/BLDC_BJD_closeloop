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
* 文件名称          main
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

#include "ch32v20x_it.h"
#include "motor.h"
#include "control.h"
#include "battery.h"
#include "led.h"
#include "pwm_input.h"
uint16 period_time = 0;
uint8 clac_flag = 0;
uint16 delay_flag = 0;
uint16 last_commutation_num = 0;


void NMI_Handler        (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler  (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler    (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler    (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler    (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler    (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler   (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_IQRHandler    (void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM1_IQRHandler(void){
    printf("TIM1_IQRHandler\r\n");
}

void SysTick_Handler(void)
{
    // 电池电压采集
    if(battery_voltage_get())
    {
        // 电池电压过低
        motor.run_flag = BYTE_LOW_VOLTAGE;
    }

    // 电机控制
    pit_callback();

    // led灯控制
    led_control();

//    if(period_time >= 1){
//
//        period_time = 0;
        PID_Calculate(&fan ,1000000/(float)motor.filter_commutation_time_sum);
        if (motor.run_flag == MOTOR_CLOSE_LOOP) {
            clac_flag = 1;
            if(fan.Output >= 0 && fan.Output <= BLDC_MAX_DUTY){
                motor.duty = fan.Output;
            }else if(fan.Output <= 0){
                motor.duty = 0;
            }else{
                motor.duty = BLDC_MAX_DUTY;
            }
        } else {
            motor.duty = fan.SetPoint;
        }
//    } else {
//        period_time++;
//    }
//     last_commutation_num = motor.filter_commutation_time_sum;


    // 清除中断标志位
    SysTick->SR = 0;
}



void TIM2_IRQHandler(void)
{
    if (RESET != TIM_GetITStatus(TIM2, TIM_FLAG_CC1))
    {
        pwm_input_trigger_callback();
        // 清除中断标志位
        TIM2->INTFR = ~TIM_FLAG_CC1;
    }

    if (RESET != TIM_GetITStatus(TIM2, TIM_FLAG_CC4))
    {
        pwm_input_timeout_callback();
        // 清除中断标志位
        TIM2->INTFR = ~TIM_FLAG_CC4;
    }
}

void TIM3_IRQHandler(void)
{
    commutation_and_degauss_callback();



    // 清除中断标志位
    TIM3->INTFR = 0;
}

void TIM4_IRQHandler(void)
{
    commutation_time_out_callback();
    // 清除中断标志位
    TIM4->INTFR = 0;
}

void EXTI2_IRQHandler(void)
{

    opama_callback();
    // 清除中断标志位
    EXTI->INTFR = EXTI_Line2;
}


/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}



/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    uint32_t temp_reg[3];
    motor_stop(BLDC_BRAKE_DUTY);
    printf("HardFault_Handler\r\n");
    temp_reg[0] = __get_MCAUSE();
    temp_reg[1] = __get_MTVAL();
    temp_reg[2] = __get_MEPC();
    printf("mcause:%0x\r\n",temp_reg[0]);
    printf("mtval:%0x\r\n",temp_reg[1]);
    printf("mepc:%0x\r\n",temp_reg[2]);
    while (1)
    {

    }
}


