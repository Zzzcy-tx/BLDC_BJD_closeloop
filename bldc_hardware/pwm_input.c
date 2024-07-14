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
* 文件名称          pwm_input
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

#include "pwm_input.h"
#include "motor.h"
#include "control.h"
PID fan;
pwm_input_struct motor_pwm_input_value;

//-------------------------------------------------------------------------------------------------------------------
// @brief      PWM输入捕获触发中断回调函数
// @param      void
// @return     void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_trigger_callback(void)
{
    // 获取周期值
    motor_pwm_input_value.period = TIM_GetCapture1(TIM2) + 1;
    // 获取高电平时间
    motor_pwm_input_value.high   = TIM_GetCapture2(TIM2) + 1;
    // 获取低电平
    motor_pwm_input_value.low    = motor_pwm_input_value.period - motor_pwm_input_value.high;
    // 计算频率
    motor_pwm_input_value.freq   = 1000000 / motor_pwm_input_value.period;
    // 电调输入频率要求为50HZ
    // 高电平时长需要在1ms-2ms之间，这里设置的最大值为3ms
    if( (20 < motor_pwm_input_value.freq) &&
            (400 > motor_pwm_input_value.freq) &&
            (3000 > motor_pwm_input_value.high) &&
            (1000 < motor_pwm_input_value.high) )
    {
        // 超过2ms，就设置为最大值
        if(2000 <= motor_pwm_input_value.high)
        {
            motor_pwm_input_value.throttle = 1000;
            clac_flag = 1;
        }
        else
        {
            motor_pwm_input_value.throttle = (motor_pwm_input_value.high - 1000);
            clac_flag = 1;
        }

        if(BLDC_MIN_DUTY > (motor_pwm_input_value.throttle * BLDC_MAX_DUTY / 1000))
        {
            motor_pwm_input_value.throttle = 0;
            clac_flag = 0;
        }
    }
    else
    {
        motor_pwm_input_value.throttle = 0;
        clac_flag = 0;
    }

//     if (motor.run_flag == MOTOR_IDLE || motor.run_flag == MOTOR_STOP_STALL)
//     {
//        motor.duty = (uint32)motor_pwm_input_value.throttle * BLDC_MAX_DUTY / 1000;
//     } else {
//    fan.SetPoint = 0.8 * (uint32)motor_pwm_input_value.throttle * BLDC_MAX_DUTY / 1000;   //Side Fans
    fan.SetPoint = 0.73 * (uint32)motor_pwm_input_value.throttle * BLDC_MAX_DUTY / 1000;    //Front and Back side
//     }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief      PWM输入捕获超时中断回调函数
// @param      void
// @return     void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_timeout_callback(void)
{
    motor_pwm_input_value.high   = 0;
    motor_pwm_input_value.low    = 0;
    motor_pwm_input_value.period = 3000;       // 获取周期值
    motor_pwm_input_value.freq   = 0;
    motor_pwm_input_value.throttle = 0;
    motor.duty = 0;
    motor.duty_register = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PWM输入脉宽捕获初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            // 使能AFIO复用功能模块时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);               // 完全重映射
    TIM_ICInitTypeDef       TIM_ICInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
    TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1; // 1us 计数一次,支持的范围20hz-400hz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         // 上升沿，获取周期值
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICFilter = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;         // 下降沿，获取高电平时间
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICFilter = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
    TIM_OCInitTypeDef       TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       // PWM模式1，定时器计数值小于比较值的时候，引脚输出有效电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           // CHx通道使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;         //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               // CHx  有效电平为高电平 无效电平为低电平
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;             // CHxN 有效电平为高电平 无效电平为低电平
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;            // CHx  空闲状态输出低电平
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;          // CHxN 空闲状态输出低电平
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseInitStructure.TIM_Period - 1;   // 设置通道4比较值
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                       // CC4用于判断超时
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);                        // 使能触发中断
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);                        // 使能通道4 判断超时
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    NVIC_SetPriority(TIM2_IRQn, 0x00);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);
}











