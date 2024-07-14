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
* �ļ�����          control
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

#ifndef _control_h_
#define _control_h_
#include "headfile.h"
#include "bldc_config.h"

typedef struct 
{
    float SetPoint;
    float LastSetPoint;
    float Proportion;
    float Integral;
    float Derivative;
    float Derivative_Input;
    float LastError;
    float PreError;
    float SumError;
    float Output;
    float LastOutput;
}PID;

extern PID fan;
extern uint8 clac_flag;

void commutation_and_degauss_callback();
void commutation_time_out_callback();
void opama_callback();
void pwm_output_callback();
void pit_callback();

void calc_speed_init();
void advance_commutation_count_init();
void commutation_time_count_init();
void pit_init();
void set_vector_table_free();
void PID_Init(PID *pid, double setpoint, double kp, double ki, double kd, double kd_input);
void PID_Calculate(PID *pid, float current_value);

#endif
