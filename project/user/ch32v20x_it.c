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
* �ļ�����          main
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
    // ��ص�ѹ�ɼ�
    if(battery_voltage_get())
    {
        // ��ص�ѹ����
        motor.run_flag = BYTE_LOW_VOLTAGE;
    }

    // �������
    pit_callback();

    // led�ƿ���
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


    // ����жϱ�־λ
    SysTick->SR = 0;
}



void TIM2_IRQHandler(void)
{
    if (RESET != TIM_GetITStatus(TIM2, TIM_FLAG_CC1))
    {
        pwm_input_trigger_callback();
        // ����жϱ�־λ
        TIM2->INTFR = ~TIM_FLAG_CC1;
    }

    if (RESET != TIM_GetITStatus(TIM2, TIM_FLAG_CC4))
    {
        pwm_input_timeout_callback();
        // ����жϱ�־λ
        TIM2->INTFR = ~TIM_FLAG_CC4;
    }
}

void TIM3_IRQHandler(void)
{
    commutation_and_degauss_callback();



    // ����жϱ�־λ
    TIM3->INTFR = 0;
}

void TIM4_IRQHandler(void)
{
    commutation_time_out_callback();
    // ����жϱ�־λ
    TIM4->INTFR = 0;
}

void EXTI2_IRQHandler(void)
{

    opama_callback();
    // ����жϱ�־λ
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


