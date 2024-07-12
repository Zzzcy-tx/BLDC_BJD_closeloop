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
* �ļ�����          pwm_output
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

#include "pwm_output.h"

uint8 comp_pwm = 1;

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����PWMռ�ձȸ���
// @param       void
// @return      duty    ռ�ձ�
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty_update(uint16 duty)
{
    if (BLDC_MAX_DUTY < duty)
    {
        duty = BLDC_MAX_DUTY;
    }

    TIM1->ATRLR = BLDC_MAX_DUTY - 1;
    TIM1->CH1CVR = duty;
    TIM1->CH2CVR = duty;
    TIM1->CH3CVR = duty;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       A�����PWM��B�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_a_bn_output(uint8 comp_pwm)
{
    // A������PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    if(comp_pwm)
    {
        // A�����Ź�
        PHASE_A_GPIO_PORT_LOW->CFGLR &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR |= (0xb<<28);
    }
    else
    {
        // A�����Ź�
        PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
        PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    }
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
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;//high close
    // C�����Ź�
    PHASE_C_GPIO_PORT_LOW->CFGLR &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       A�����PWM��C�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_a_cn_output(uint8 comp_pwm)
{
    // A������PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    if(comp_pwm)
    {
        // A������PWM
        PHASE_A_GPIO_PORT_LOW->CFGLR &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR |= (0xb<<28);
    }
    else
    {
        // A�����Ź�
        PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
        PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    }
    // C�����Ź�
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C�����ſ�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // B�����Ź�
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH; //high close
    // B�����Ź�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       B�����PWM��C�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_b_cn_output(uint8 comp_pwm)
{
    // B������PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    if(comp_pwm)
    {
        // B������PWM
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0xb<<0);
    }
    else
    {
        // B�����Ź�
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
        PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
    }
    // C�����Ź�
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C�����ſ�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A�����Ź�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       B�����PWM��A�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_b_an_output(uint8 comp_pwm)
{
    // B������PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    if(comp_pwm)
    {
        // B������PWM
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0xb<<0);
    }
    else
    {
        // B�����Ź�
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
        PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
    }

    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH; //high close
    // A�����ſ�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;   // low on
    // C�����Ź�
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;//high close
    // C�����Ź�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       C�����PWM��A�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_c_an_output(uint8 comp_pwm)
{
    // C������PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    if(comp_pwm)
    {
        // C������PWM
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0xb<<4);
    }
    else
    {
        // C�����Ź�
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
        PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
    }

    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH; //high close
    // A�����ſ�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;   // low on
    // B�����Ź�
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH; //high close
    // B�����Ź�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       C�����PWM��B�����ſ�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_c_bn_output(uint8 comp_pwm)
{
    // C������PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    if(comp_pwm)
    {
        // C������PWM
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0xb<<4);
    }
    else
    {
        // C�����Ź�
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
        PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
    }
    // B�����Ź�
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B�����ſ�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A�����Ź�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ȫ��
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_an_bn_cn_off()
{
    // A�����Ź�
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A�����Ź�
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    // B�����Ź�
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B�����Ź�
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;
    // C�����Ź�
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C�����Ź�
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       PWM���������ʼ��
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_complementary_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);            // ��ӳ��
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = PHASE_A_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_A_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_B_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_B_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_C_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_C_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_A_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_A_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_B_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_B_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_C_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_C_GPIO_PORT_HIGH, &GPIO_InitStruct);
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_BDTRInitTypeDef     TIM_BDTRInitStructure = {0};
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = BLDC_MAX_DUTY - 1;                   // ���ö�ʱ������ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                                // ��ʱ��ʱ�Ӳ���Ƶ
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 // ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;             // ���Ķ���ģʽ  PWMƵ��=��ʱ������ʱ��Ƶ��/(2*����ֵ)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                        // ռ�ձȵĸ���Ƶ��=��ʱ������ʱ��Ƶ��/((TIM_RepetitionCounter+1)*����ֵ)
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           // PWMģʽ1����ʱ������ֵС�ڱȽ�ֵ��ʱ�����������Ч��ƽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;               // CHxͨ��ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;             //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   // CHx  ��Ч��ƽΪ�ߵ�ƽ ��Ч��ƽΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;                // CHx  ����״̬����͵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;                 // CHxN ��Ч��ƽΪ�ߵ�ƽ ��Ч��ƽΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;              // CHxN ����״̬����͵�ƽ
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC1FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC2FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC3FastConfig(TIM1, TIM_OCFast_Disable);
//    TIM_OC4FastConfig(TIM1, TIM_OCFast_Disable);
//    TIM_CCPreloadControl(TIM1, ENABLE);
    TIM_BDTRStructInit(&TIM_BDTRInitStructure);
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = BLDC_PWM_DEADTIME;                          // PWM����ʱ��
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                        // ɲ��ʧ��
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;           // ɲ������Ϊ�ߣ��ߵ�ƽ��ʱ��ɲ��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;    // ��ɲ���ź���Ч��ʱ���Զ��������
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Break,    DISABLE);                   // �ر�ɲ���ж�
    TIM_ITConfig(TIM1, TIM_IT_Update,   DISABLE);                   // �رո����ж�
    TIM_ITConfig(TIM1, TIM_IT_Trigger,  DISABLE);                   // ����ʱ�������ж�
    TIM_ITConfig(TIM1, TIM_IT_COM,      DISABLE);
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);                             // ʹ���Զ���װ��Ԥװ��ʹ��
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update);

}
