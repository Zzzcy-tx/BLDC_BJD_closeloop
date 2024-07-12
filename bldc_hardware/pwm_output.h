#include "headfile.h"
#include "bldc_config.h"/*********************************************************************************************************************
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

#ifndef _pwm_output_h_
#define _pwm_output_h_

#include "headfile.h"
#include "bldc_config.h"



#define PHASE_A_GPIO_LOW        GPIO_Pin_7
#define PHASE_A_GPIO_PORT_LOW   GPIOA
#define PHASE_A_GPIO_HIGH       GPIO_Pin_8
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        GPIO_Pin_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       GPIO_Pin_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        GPIO_Pin_1
#define PHASE_C_GPIO_PORT_LOW   GPIOB
#define PHASE_C_GPIO_HIGH       GPIO_Pin_10
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define LOW_BITREG_ON  BSHR
#define LOW_BITREG_OFF BCR
#define HIGH_BITREG_ON BSHR
#define HIGH_BITREG_OFF BCR



void pwm_complementary_control(uint8 status);
void pwm_complementary_set_duty(uint16 duty);
void pwm_complementary_brake();
void pwm_a_bn_output(uint8 comp_pwm);
void pwm_a_cn_output(uint8 comp_pwm);
void pwm_b_cn_output(uint8 comp_pwm);
void pwm_b_an_output(uint8 comp_pwm);
void pwm_c_an_output(uint8 comp_pwm);
void pwm_c_bn_output(uint8 comp_pwm);
void pwm_an_bn_cn_off(void);

void PWM_TIM1_Init(void);  //PWM;

void pwm_duty_update(uint16 duty);

void pwm_complementary_init();

#endif
