#ifndef _bldc_config_h_
#define _bldc_config_h_

// ����������ѹ
#define BLDC_START_VOLTAGE          (1.3f )         // 1.0 - 2.0 ��λV

#define BLDC_MIN_VOLTAGE            (10500)         // ��͵�ѹֵ����λmv

#define BLDC_COMMUTATION_FAILED_MAX (2000 )         // ������������� ���������������Ϊ�����ת

#define BLDC_MAX_DUTY               (3000)          // ���Ƶ��Ϊ48kHZ��PWM�����ռ�ձȣ���Ҫ���и���PWM��ʼ��ȥ����
                                                    // ���㹫ʽ��SystemCoreClock / 48000 = 3000
                                                    // SystemCoreClock = 144000000

#define BLDC_BRAKE_DUTY             (100)           // ɲ��ʱ��ռ�ձ�.

#define BLDC_MIN_DUTY               (150)           // PWM����Сռ�ձȣ������������Ŵ�С���������ռ�ձ�С�ڴ˶�������ͣת ����Ҫ����150

#define BLDC_POLES                  (7  )           // ���������

#define BLDC_OPEN_LOOP_WRITE        (10 )           // �����ȴ��������

#define BLDC_CLOSE_LOOP_WRITE       (30 )           // �ջ��ȴ��������

#define BLDC_STALL_TIME_OUT         (200)           // ��ת��ʱ��⣬��λms

#define BLDC_PWM_DEADTIME           (30)            // PWM��������

#define BLDC_BEEP_ENABLE            (1  )           // 1:ʹ���ϵ������й��� 0������

#define BLDC_BEEP_VOLUME            (100)           // �������������С 0-100

#define BLDC_RESTART_DELAY          (100)           // ����ʧ�ܣ�������ʱ����λms

#define BLDC_RESTART_MAX            (20)            // �����������

#define BLDC_SPEED_INCREMENTAL      (5 )            // ���μ������ֵ 1-15�����޸������߼�����Ӧ

#endif










