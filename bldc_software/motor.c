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
* 文件名称          motor
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

#include "motor.h"
#include "delay.h"
#include "pwm_output.h"

motor_struct motor;

//-------------------------------------------------------------------------------------------------------------------
// @brief       电机执行换向
// @param       void
// @return      uint8         下一步的序号
// @note
//-------------------------------------------------------------------------------------------------------------------
void motor_commutation_execute(uint8 step, uint8 comp_pwm)
{
//    TIM1->CTLR1 &= ~TIM_CEN;
//    pwm_an_bn_cn_off();
    switch(step)
    {
        case 0:
        {
            pwm_a_bn_output(comp_pwm);
        }
        break;
        case 1:
        {
            pwm_a_cn_output(comp_pwm);
        }
        break;
        case 2:
        {
            pwm_b_cn_output(comp_pwm);
        }
        break;
        case 3:
        {
            pwm_b_an_output(comp_pwm);
        }
        break;
        case 4:
        {
            pwm_c_an_output(comp_pwm);
        }
        break;
        case 5:
        {
            pwm_c_bn_output(comp_pwm);
        }
        break;
    }
//    TIM1->CTLR1 |=  TIM_CEN;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电机启动
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void motor_start(void)
{
//    motor.step = 1;
//    motor.duty = motor_pwm_output.pwm_duty_value;
    motor.run_flag = MOTOR_START;
    motor.commutation_time[0] = 6000;
    motor.commutation_time[1] = 6000;
    motor.commutation_time[2] = 6000;
    motor.commutation_time[3] = 6000;
    motor.commutation_time[4] = 6000;
    motor.commutation_time[5] = 6000;
    motor.commutation_time_sum = motor.commutation_time[0] + motor.commutation_time[1] + motor.commutation_time[2] + \
                                 motor.commutation_time[3] + motor.commutation_time[4] + motor.commutation_time[5] ;
    motor.filter_commutation_time_sum = motor.commutation_time_sum;
    motor.commutation_num   = 0;
    motor.commutation_faile_count = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       换向到下一个相序
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void motor_next_step(uint8 comp_pwm)
{
    motor.step++;
    if (motor.step > 5)
    {
        motor.step = 0;
    }
    // 换相
    motor_commutation_execute(motor.step, comp_pwm);
    // 设置比较器通道
    if (motor.step == 0 || motor.step == 3)
    {
        OPA->CR = 0x70;
    }
    if (motor.step == 1 || motor.step == 4)
    {
        OPA->CR = 0x10;
    }
    if (motor.step == 2 || motor.step == 5)
    {
        OPA->CR = 0x01;
    }
    // 开启对应的边沿检测
    if ((motor.step % 2))
    {
        EXTI->RTENR = 0;
        EXTI->FTENR = (1<<2);       // 下降沿
    }
    else
    {
        EXTI->FTENR = 0;
        EXTI->RTENR = (1<<2);       // 上升沿
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      刹车
//  @param      duty    刹车占空比
//  @return     void
//  @since      v1.0
//  Sample usage:   上桥全关,下桥输入PWM,进行刹车
//-------------------------------------------------------------------------------------------------------------------
void motor_brake(uint16 duty)
{
    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A相下桥开
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B相下桥开
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C相下桥开
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;

    pwm_duty_update(duty);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电机停止转动
// @param       void
// @return      void
// Sample usage:
// @note        刹车
//-------------------------------------------------------------------------------------------------------------------
void motor_stop(uint16 duty)
{
    motor_brake(duty);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电机上电鸣叫
// @param       void
// @return      volume          鸣叫音量大小
// Sample usage:
// @note        使用一相上桥PWM和另一项下桥开启的方式播放音乐
//-------------------------------------------------------------------------------------------------------------------
void motor_power_on_beep(uint16 volume)
{
#define MUSIC_DELAY_MS   250
    uint16 frequency_spectrum[6] = {0, 523, 587, 659, 698, 783};
    uint16 beep_duty;
    uint32 match_temp;
    uint32 period_temp;
    uint16 freq_div = 0;
    beep_duty = volume;
    // 保护限制，避免设置过大烧毁电机
    if(100 < beep_duty)
    {
        beep_duty = 100;
    }
    pwm_an_bn_cn_off();
    // A相PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    // B相下桥开
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // 分频计算，周期计算，占空比计算
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[1]) >> 16);                   // 多少分频
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[1] * (freq_div + 1)));    // 周期
    match_temp = period_temp * beep_duty / 10000;                                               // 占空比
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = match_temp;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
    system_delay_ms(MUSIC_DELAY_MS);
    pwm_an_bn_cn_off();
    // B相上桥PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    // C相下桥开
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // 分频计算，周期计算，占空比计算
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[2]) >> 16);                   // 多少分频
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[2] * (freq_div + 1)));    // 周期
    match_temp = period_temp * beep_duty / 10000;                                               // 占空比
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = match_temp;
    TIM1->CH3CVR = 0;
    system_delay_ms(MUSIC_DELAY_MS);
    pwm_an_bn_cn_off();
    // C相上桥PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    // A相下桥开
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;
    // 分频计算，周期计算，占空比计算
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[3]) >> 16);                   // 多少分频
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[3] * (freq_div + 1)));    // 周期
    match_temp = period_temp * beep_duty / 10000;                                               // 占空比
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = match_temp;
    system_delay_ms(MUSIC_DELAY_MS);
    // 恢复48Khz的控制频率
    TIM1->PSC = 0;
    TIM1->ATRLR = BLDC_MAX_DUTY - 1;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
}



