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
* �ļ�����          pwm_input
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

#include "pwm_input.h"
#include "motor.h"
#include "control.h"
PID fan;
pwm_input_struct motor_pwm_input_value;

//-------------------------------------------------------------------------------------------------------------------
// @brief      PWM���벶�񴥷��жϻص�����
// @param      void
// @return     void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_trigger_callback(void)
{
    // ��ȡ����ֵ
    motor_pwm_input_value.period = TIM_GetCapture1(TIM2) + 1;
    // ��ȡ�ߵ�ƽʱ��
    motor_pwm_input_value.high   = TIM_GetCapture2(TIM2) + 1;
    // ��ȡ�͵�ƽ
    motor_pwm_input_value.low    = motor_pwm_input_value.period - motor_pwm_input_value.high;
    // ����Ƶ��
    motor_pwm_input_value.freq   = 1000000 / motor_pwm_input_value.period;
    // �������Ƶ��Ҫ��Ϊ50HZ
    // �ߵ�ƽʱ����Ҫ��1ms-2ms֮�䣬�������õ����ֵΪ3ms
    if( (20 < motor_pwm_input_value.freq) &&
            (400 > motor_pwm_input_value.freq) &&
            (3000 > motor_pwm_input_value.high) &&
            (1000 < motor_pwm_input_value.high) )
    {
        // ����2ms��������Ϊ���ֵ
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
// @brief      PWM���벶��ʱ�жϻص�����
// @param      void
// @return     void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_timeout_callback(void)
{
    motor_pwm_input_value.high   = 0;
    motor_pwm_input_value.low    = 0;
    motor_pwm_input_value.period = 3000;       // ��ȡ����ֵ
    motor_pwm_input_value.freq   = 0;
    motor_pwm_input_value.throttle = 0;
    motor.duty = 0;
    motor.duty_register = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PWM�����������ʼ��
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_input_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            // ʹ��AFIO���ù���ģ��ʱ��
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);               // ��ȫ��ӳ��
    TIM_ICInitTypeDef       TIM_ICInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
    TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1; // 1us ����һ��,֧�ֵķ�Χ20hz-400hz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         // �����أ���ȡ����ֵ
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICFilter = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;         // �½��أ���ȡ�ߵ�ƽʱ��
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICFilter = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
    TIM_OCInitTypeDef       TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       // PWMģʽ1����ʱ������ֵС�ڱȽ�ֵ��ʱ�����������Ч��ƽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           // CHxͨ��ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;         //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               // CHx  ��Ч��ƽΪ�ߵ�ƽ ��Ч��ƽΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;             // CHxN ��Ч��ƽΪ�ߵ�ƽ ��Ч��ƽΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;            // CHx  ����״̬����͵�ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;          // CHxN ����״̬����͵�ƽ
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseInitStructure.TIM_Period - 1;   // ����ͨ��4�Ƚ�ֵ
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                       // CC4�����жϳ�ʱ
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);                        // ʹ�ܴ����ж�
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);                        // ʹ��ͨ��4 �жϳ�ʱ
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    NVIC_SetPriority(TIM2_IRQn, 0x00);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);
}











