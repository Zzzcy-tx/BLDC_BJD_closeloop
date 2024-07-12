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
* �ļ�����          led
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

#include "led.h"
#include "motor.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       led�����¼�
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_control(void)
{
    static uint32 time_count = 0;
    time_count++;
    switch(motor.run_flag)
    {
    case MOTOR_IDLE:
    {
        led_err_control(LED_OFF);
        if(0 == (time_count%2000))
        {
            led_run_toggle();
        }
    }
    break;
    case MOTOR_START:
    case MOTOR_OPEN_LOOP:
    case MOTOR_CLOSE_LOOP:
    {
        led_run_control(LED_ON);
        led_err_control(LED_OFF);
    }
    break;
    case MOTOR_STOP_STALL:
    {
        led_run_control(LED_OFF);
        if(0 == (time_count%2000))
        {
            led_err_toggle();
        }
    }
    break;
    case BYTE_LOW_VOLTAGE:
    {
        led_run_control(LED_OFF);
        led_err_control(LED_ON);
    }
    break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����LED����
// @param       void
// @return      LED_STATUS_enum         LED_ON-�� LED_OFF-��
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_run_control(LED_STATUS_enum state)
{
    if(state == LED_ON)
    {
        LED_RUN_GROUP->BCR = LED_RUN_PIN;
    }
    else
    {
        LED_RUN_GROUP->BSHR = LED_RUN_PIN;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����LED����
// @param       void
// @return      LED_STATUS_enum         LED_ON-�� LED_OFF-��
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_err_control(LED_STATUS_enum state)
{
    if(state == LED_ON)
    {
        LED_ERR_GROUP->BCR = LED_ERR_PIN;
    }
    else
    {
        LED_ERR_GROUP->BSHR = LED_ERR_PIN;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����LED��ת
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_run_toggle(void)
{
    ((LED_RUN_GROUP))->OUTDR ^= LED_RUN_PIN;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����LED��ת
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_err_toggle(void)
{
    ((LED_ERR_GROUP))->OUTDR ^= LED_ERR_PIN;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LED��ʼ��
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = LED_RUN_PIN;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_RUN_GROUP, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LED_ERR_PIN;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_ERR_GROUP, &GPIO_InitStruct);
    led_run_control(LED_ON);
    led_err_control(LED_OFF);
}

