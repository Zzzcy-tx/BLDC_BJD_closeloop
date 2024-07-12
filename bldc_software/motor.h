/*********************************************************************************************************************
* CH32V203C8T6_BLDC_ESC ����CH32V203C8T6 �޸���ˢ�������һ�����ڹٷ�? SDK �ӿڵĵ�������Դ��Ŀ
* Copyright (c) 2022 SEEKFREE ��ɿƼ�?
*
* CH32V203C8T6_BLDC_ESC ��Դ��Ŀ��һ����
*
* CH32V203C8T6 ��Դ�� ���������?
* �����Ը����������������?���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����?/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı��?
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ�? GPL
*
* ��Ӧ�����յ�����Դ����?ʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� bldc_config/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ bldc_config �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ��?����������������
*
* �ļ�����          motor
* ��˾����          �ɶ���ɿƼ����޹��?
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio
* ����ƽ̨          CH32V203C8T6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����            ����         ��ע
* 2024-01-04        ��W          first version
********************************************************************************************************************/

#ifndef _motor_h_
#define _motor_h_

#include "headfile.h"
#include "bldc_config.h"

typedef struct
{
    vuint8  run_flag;                       // ����������б�־�?
    uint8   step;                           // ������еĲ���?
    uint16  commutation_time[6];            // ���?6�λ���ʱ��
    int32   duty;                           // PWMռ�ձ�       �û��ı����ٶ�ʱ�޸Ĵ˱���
    int32   duty_register;                  // PWMռ�ձȼĴ��� �û����ɲ���
    uint32  commutation_faile_count;        // ����ʧ�ܼ���
    uint32  filter_commutation_time_sum;    // �˲���Ļ����ܺ�?
    uint32  commutation_time_sum;           // ���?6�λ���ʱ���ܺ�
    uint32  commutation_num;                // ͳ�ƻ������?
}motor_struct;

struct PID;

typedef enum
{
    MOTOR_IDLE = 0,
    MOTOR_START,
    MOTOR_OPEN_LOOP,
    MOTOR_CLOSE_LOOP ,
    MOTOR_STOP_STALL,
    MOTOR_RESTART,

    BYTE_LOW_VOLTAGE,

}run_state_enum;

#define CLOSE_COMP_PWM    0
#define OPEN_COMP_PWM     1


extern motor_struct motor;

void motor_power_on_beep(uint16 volume);
void motor_start();
void motor_next_step(uint8 comp_pwm);
void motor_stop(uint16 duty);
void motor_commutation_execute(uint8 step, uint8 comp_pwm);

void battery_voltage_init();

#endif
