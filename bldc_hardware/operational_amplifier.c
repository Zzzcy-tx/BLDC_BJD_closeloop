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
* �ļ�����          operational_amplifier
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

#include "operational_amplifier.h"
#include "pwm_output.h"
#include "motor.h"
#include "control.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       ʧȥ��PA2�ⲿ�����ж�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void disable_opama_interrupts(void)
{
    EXTI->INTENR &= ~(1 << 2);          // �ر�PA2�ⲿ�����ж�
    EXTI->INTFR = EXTI_Line2;           // ����жϱ�־λ
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ʧȥ��PA2�ⲿ�����ж�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void enable_opama_interrupts(void)
{
//    EXTI->INTFR = EXTI_Line2;           // ����жϱ�־λ
    EXTI->INTENR |= (1 << 2);           // ��PA2�ⲿ�����ж�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ⲿ�����жϳ�ʼ��
// @param       void
// @return      void
// @note        PA2��PA3��PA4Ϊ�˷��������
//              ��ԭ��ͼ����PA2��PA3��PA4�Ѿ�������һ��
//-------------------------------------------------------------------------------------------------------------------
void pa2_exit_init()
{
    EXTI_InitTypeDef        EXTI_InitStructure = {0};
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line2);
    NVIC_SetPriority(EXTI2_IRQn, 0x00);
    NVIC_EnableIRQ(EXTI2_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief      �˷ų�ʼ��
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void opama_init(void)
{
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    OPA->CR = 0x01;
}


