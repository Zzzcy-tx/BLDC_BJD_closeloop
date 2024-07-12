/*********************************************************************************************************************
* CH32V203C8T6_BLDC_ESC ����CH32V203C8T6 �޸���ˢ�������һ�����ڹٷ� SDK �ӿڵĵ�������Դ��Ŀ
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* CH32V203C8T6_BLDC_ESC ��Դ��Ŀ��һ����
*
* CH32V203C8T6 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� bldc_config/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ bldc_config �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          motor
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio
* ����ƽ̨          CH32V203C8T6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����            ����         ��ע
* 2024-01-04        ��W          first version
********************************************************************************************************************/

#include "motor.h"
#include "delay.h"
#include "pwm_output.h"

motor_struct motor;

//-------------------------------------------------------------------------------------------------------------------
// @brief       ���ִ�л���
// @param       void
// @return      uint8         ��һ�������
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
// @brief       �������
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
// @brief       ������һ������
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
    // ����
    motor_commutation_execute(motor.step, comp_pwm);
    // ���ñȽ���ͨ��
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
    // ������Ӧ�ı��ؼ��
    if ((motor.step % 2))
    {
        EXTI->RTENR = 0;
        EXTI->FTENR = (1<<2);       // �½���
    }
    else
    {
        EXTI->FTENR = 0;
        EXTI->RTENR = (1<<2);       // ������
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ɲ��
//  @param      duty    ɲ��ռ�ձ�
//  @return     void
//  @since      v1.0
//  Sample usage:   ����ȫ��,��������PWM,����ɲ��
//-------------------------------------------------------------------------------------------------------------------
void motor_brake(uint16 duty)
{
    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A�����ſ�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;
    // B�����Ź�
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B�����ſ�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // C�����Ź�
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C�����ſ�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;

    pwm_duty_update(duty);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ���ֹͣת��
// @param       void
// @return      void
// Sample usage:
// @note        ɲ��
//-------------------------------------------------------------------------------------------------------------------
void motor_stop(uint16 duty)
{
    motor_brake(duty);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ϵ�����
// @param       void
// @return      volume          ����������С
// Sample usage:
// @note        ʹ��һ������PWM����һ�����ſ����ķ�ʽ��������
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
    // �������ƣ��������ù����ջٵ��
    if(100 < beep_duty)
    {
        beep_duty = 100;
    }
    pwm_an_bn_cn_off();
    // A��PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    // B�����ſ�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // ��Ƶ���㣬���ڼ��㣬ռ�ձȼ���
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[1]) >> 16);                   // ���ٷ�Ƶ
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[1] * (freq_div + 1)));    // ����
    match_temp = period_temp * beep_duty / 10000;                                               // ռ�ձ�
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = match_temp;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
    system_delay_ms(MUSIC_DELAY_MS);
    pwm_an_bn_cn_off();
    // B������PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    // C�����ſ�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // ��Ƶ���㣬���ڼ��㣬ռ�ձȼ���
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[2]) >> 16);                   // ���ٷ�Ƶ
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[2] * (freq_div + 1)));    // ����
    match_temp = period_temp * beep_duty / 10000;                                               // ռ�ձ�
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = match_temp;
    TIM1->CH3CVR = 0;
    system_delay_ms(MUSIC_DELAY_MS);
    pwm_an_bn_cn_off();
    // C������PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    // A�����ſ�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;
    // ��Ƶ���㣬���ڼ��㣬ռ�ձȼ���
    freq_div = (uint16)((SystemCoreClock / 2 / frequency_spectrum[3]) >> 16);                   // ���ٷ�Ƶ
    period_temp = (uint16)(SystemCoreClock / 2 / ( frequency_spectrum[3] * (freq_div + 1)));    // ����
    match_temp = period_temp * beep_duty / 10000;                                               // ռ�ձ�
    TIM1->ATRLR = period_temp - 1;
    TIM1->PSC = freq_div;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = match_temp;
    system_delay_ms(MUSIC_DELAY_MS);
    // �ָ�48Khz�Ŀ���Ƶ��
    TIM1->PSC = 0;
    TIM1->ATRLR = BLDC_MAX_DUTY - 1;
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
}



