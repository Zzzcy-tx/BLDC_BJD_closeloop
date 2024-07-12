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
* �ļ�����          control
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

#include "control.h"
#include "operational_amplifier.h"
#include "pwm_output.h"
#include "motor.h"
#include "battery.h"
#include "stdlib.h"

static uint16 temp_commutation_time;

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������������жϻص�����
// @param       void
// @return      uint8         1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// @note        �ú�����ch32v20x_it.c�е�TIM3_IRQHandler�б�����
//-------------------------------------------------------------------------------------------------------------------
void commutation_and_degauss_callback()
{
    if(motor.run_flag == MOTOR_CLOSE_LOOP)
    {
        // ����
        motor_next_step(CLOSE_COMP_PWM);

        // ʹ�ܱȽ����ж�
        enable_opama_interrupts();

        // ȥ�����������
        motor.commutation_time_sum -= motor.commutation_time[motor.step];
        // ���滻��ʱ��
        motor.commutation_time[motor.step] = temp_commutation_time;
        // �����µĻ���ʱ�䣬��6�λ�����ʱ��
        motor.commutation_time_sum += motor.commutation_time[motor.step];
        // �����������һ
        motor.commutation_num++;
        // һ�׵�ͨ�˲�
        motor.filter_commutation_time_sum = (motor.filter_commutation_time_sum * 7 + motor.commutation_time_sum * 1) >> 3;

        // �ȴ��ȶ����ٿ�ʼ��������ж�
        if((BLDC_CLOSE_LOOP_WRITE * BLDC_POLES) < motor.commutation_num)
        {
            // ���λ���60�ȵ�ʱ�䣬����һ�λ���һȦʱ���45�ȵ�75�ȣ�������Ϊ�������
            // (360.0f / 45) = 8
            // (360.0f / 75) = 4.8
            if((temp_commutation_time > ((motor.filter_commutation_time_sum >> 3))) && (temp_commutation_time < (motor.filter_commutation_time_sum / 4.8f)))
            {
                // ��ʱ��ȥ����ʧ�ܼ�����
                if((motor.commutation_faile_count))
                {
                    motor.commutation_faile_count--;
                }
            }
            else
            {
                if(BLDC_COMMUTATION_FAILED_MAX < motor.commutation_faile_count++)
                {
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }
        }
    }
    TIM3->DMAINTENR &= ~TIM_IT_Update;  // �رն�ʱ��3�����ж�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ʱ�ص�����
// @param       void
// @return      uint8         1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// @note        �ú�����ch32v20x_it.c�е�TIM4_IRQHandler�б�����
//-------------------------------------------------------------------------------------------------------------------
void commutation_time_out_callback()
{
    if(motor.run_flag == MOTOR_CLOSE_LOOP)
    {
        if(motor.commutation_num > (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES))
        {
            // ����Ǳջ����򣬽��������������ת
            motor.run_flag = MOTOR_STOP_STALL;
        }
    }
    else
    {
        // �رն�ʱ��4�����ж�
        TIM4->DMAINTENR &= ~TIM_IT_Update;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �˷���Ϊ�Ƚ����жϻص�
// @param       void
// @return      uint8         1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// @note        �ú�����ch32v20x_it.c�е�EXTI2_IRQHandler�б�����
//              �˷���Ϊ�Ƚ��������PA2��PA3��PA4��������
//              PA2��PA3��PA4Ӳ������������һ��
//              ��PA2�����������ػ����½��ص�ʱ�򣬾ʹ����ⲿ�����ж�
//-------------------------------------------------------------------------------------------------------------------
void opama_callback()
{
    // ��ȡ���λ���ʱ��
    temp_commutation_time = TIM4->CNT;

    // ȥ�����綯��ë��
    if(temp_commutation_time < (motor.filter_commutation_time_sum / 12) && (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES) < motor.commutation_num)
    {
        return;
    }

    // ��ʱ�����
    TIM4->CNT = 0;
    TIM3->CNT = 0;
    TIM3->ATRLR = motor.filter_commutation_time_sum >> 4;     // 22.5�Ⱥ�ʼ����
    // ������ʱ��3�������¼��ж�
    TIM3->INTFR = 0x00;
    TIM3->DMAINTENR |= TIM_IT_Update;

    disable_opama_interrupts();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       pit�����жϻص�
// @param       void
// @return      void
// @note        �ú�����ch32v20x_it.c�е�SysTick_Handler�б�����
//              ��શ�ʱ�������������жϣ��Ͳ�������Ϊ��ʱʹ����
//-------------------------------------------------------------------------------------------------------------------
void pit_callback(void)
{
    // ��ص�ѹ���Ϊ������ȼ�������ѹ���ͣ��Ͳ�ת
    if(motor.run_flag != BYTE_LOW_VOLTAGE)
    {
        // �����ռ�ձ�Ϊ0����Ϊ����״̬
        if(motor.duty == 0)
        {
            motor.run_flag = MOTOR_IDLE;
        }
        else
        {
            // �����ռ�ձȲ�Ϊ0����ӿ���״̬תΪ�����ʼ
            if(motor.run_flag == MOTOR_IDLE)
            {
                motor.run_flag = MOTOR_START;
            }
        }
    }

    static uint16 stall_time_out_check = 0;
    static uint8  restart_count = 0;
    switch(motor.run_flag)
    {
        case MOTOR_START:
        {
            // �����������
            srand(battery.adc_reg_value);

            // �رն�ʱ��1�������ж�
            TIM1->DMAINTENR &= ~TIM_IT_Update;
            // �رն�ʱ��3�������ж�
            TIM3->DMAINTENR &= ~TIM_IT_Update;
            // �رն�ʱ��4�������ж�
            TIM4->DMAINTENR &= ~TIM_IT_Update;
            // �ر�PA2�ⲿ�����ж�
            disable_opama_interrupts();
            // ��ʼ�׶�
            // �ȴ�10ms + �����ʱ(0-50)ms��
            if(TIM4->CNT > (10 + rand()%50)*2000)
            {
                motor.duty_register = ((float)BLDC_START_VOLTAGE * BLDC_MAX_DUTY / ((float)battery.voltage/1000));
                if(motor.duty_register < (BLDC_MAX_DUTY / 25))
                {
                    motor.duty_register = (BLDC_MAX_DUTY / 25);
                }
                pwm_duty_update(motor.duty_register);
                TIM4->CNT = 0;
                // ���һЩ������ֵ
                motor_start();
                // ִ�л���
                motor_next_step(CLOSE_COMP_PWM);
                motor.filter_commutation_time_sum = 0;
                stall_time_out_check = 0;
                motor.run_flag = MOTOR_OPEN_LOOP;
            }
        }
        break;
        case MOTOR_OPEN_LOOP:
        {
            // ����ǿ��
            static uint8 gpio_state = 0;
            uint8 motor_next_step_flag = 0;
            uint16 temp_commutation_time = TIM4->CNT;
            // ͨ��λ�Ƶķ�ʽ������ƽ���ݴ��������
            gpio_state = (gpio_state << 1) | ((GPIOA->INDR & GPIO_Pin_2) ? 1 : 0);
            if ((motor.step % 2))
            {
                //�½���
                if(gpio_state == 0)
                {
                    motor_next_step_flag = 1;
                    gpio_state = 0x03;
                }
            }
            else
            {
                // ������
                if(gpio_state == 0xFF)
                {
                    motor_next_step_flag = 1;
                    gpio_state = 0xFC;
                }
            }
            if(motor_next_step_flag)
            {
                // ��ת�������
                stall_time_out_check = 0;
                // ��ʱ������ֵ���
                TIM4->CNT = 0;
                // ȥ�����������
                motor.commutation_time_sum -= motor.commutation_time[motor.step];
                // ���滻��ʱ��
                motor.commutation_time[motor.step] = temp_commutation_time;
                // �����µĻ���ʱ�䣬��6�λ�����ʱ��
                motor.commutation_time_sum += motor.commutation_time[motor.step];
                // �����������һ
                motor.commutation_num++;
                // һ�׵�ͨ�˲�
                motor.filter_commutation_time_sum = (motor.filter_commutation_time_sum * 3 + motor.commutation_time_sum * 1) >> 2;
                // �ȴ�15��
                while(TIM4->CNT < (motor.filter_commutation_time_sum / 24));
                // ִ�л���
                motor_next_step(CLOSE_COMP_PWM);
            }
            else
            {
                // ��ת���
                if((10 * BLDC_STALL_TIME_OUT) < stall_time_out_check++)
                {
                    stall_time_out_check = 0;
                    motor.commutation_num = 0;
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }
            // �ջ�״̬�л�
            if((motor.commutation_num > (BLDC_OPEN_LOOP_WRITE * BLDC_POLES)) && (motor.filter_commutation_time_sum < 40000))
            {
                // ���һЩ����
                motor.commutation_num = 0;
                // ��ת���������
                stall_time_out_check = 0;
                // �л�Ϊ�ջ�״̬
                motor.run_flag = MOTOR_CLOSE_LOOP;
                // ��PA2�ⲿ�����ж�
                enable_opama_interrupts();
            }
            // ����20ms + �����ʱ(0-50)msû�м�⵽���䣬��ӽ�������ʼ״̬
            if((TIM4->CNT > (20 + rand()%30)*2000) && (motor.run_flag == MOTOR_OPEN_LOOP))
            {
                TIM4->CNT = 0;
                motor_next_step(CLOSE_COMP_PWM);
                motor.run_flag = MOTOR_START;
            }
        }
        break;
        case MOTOR_CLOSE_LOOP:
        {
            static uint8 oled_pin_state = 0;
            uint8 pin_state = (GPIOA->INDR & GPIO_Pin_2);
            // ͨ��PA2���ŵĵ�ƽ״̬�����ж�ת���
            if(oled_pin_state != pin_state)
            {
                stall_time_out_check = 0;
                oled_pin_state = pin_state;
            }
            else
            {
                // �������һֱ��һ��״̬���ҳ����� BLDC_STALL_TIME_OUT ms ����Ϊ��ת��
                if(stall_time_out_check++ > (10 * BLDC_STALL_TIME_OUT))          // 10KHZ��0.1ms����һ��
                {
                    stall_time_out_check = 0;
                    motor.run_flag = MOTOR_STOP_STALL;
                }
            }

            if(motor.commutation_num < (BLDC_CLOSE_LOOP_WRITE * BLDC_POLES))
            {
                 if (motor.duty_register < BLDC_PWM_DEADTIME)
                 {
                     motor.duty_register = BLDC_PWM_DEADTIME;
                 }
                 if (motor.duty_register > (BLDC_MAX_DUTY / 5 + BLDC_PWM_DEADTIME))
                 {
                     motor.duty_register = BLDC_MAX_DUTY / 5 + BLDC_PWM_DEADTIME;
                 }
            }

            // �����Ӽ���
            if((motor.duty != motor.duty_register))
            {
                if(0 > motor.duty)
                {
                    motor.duty_register = 0;
                }

                if(motor.duty > motor.duty_register)
                {
                    motor.duty_register += BLDC_SPEED_INCREMENTAL;
                }
                else
                {
                    motor.duty_register = motor.duty;
                }

                pwm_duty_update(motor.duty_register);
            }
        }

        break;
        case MOTOR_IDLE:
        case BYTE_LOW_VOLTAGE:
        {
            motor_stop(BLDC_BRAKE_DUTY);
        }
        break;
        case MOTOR_STOP_STALL:
        {
            if (restart_count < BLDC_RESTART_MAX)
            {
                motor.run_flag = MOTOR_RESTART;
            }
            motor_stop(BLDC_BRAKE_DUTY);
        }
        break;
        case MOTOR_RESTART:
        {
            static int16 restart_delay = BLDC_RESTART_DELAY*10;
            if (restart_count < BLDC_RESTART_MAX)
            {
                if(restart_delay-- < 0)
                {
                    restart_delay = BLDC_RESTART_DELAY*10;
                    restart_count++;
                    motor.run_flag = MOTOR_IDLE;
                }
            }
        }
        break;
    }
}

void PID_Init(PID *pid, double setpoint, double kp, double ki, double kd) {
    pid->SetPoint = setpoint;
    pid->Proportion = kp;
    pid->Integral = ki;
    pid->Derivative = kd;
    pid->LastError = 0;
    pid->PreError = 0;
    pid->SumError = 0;
    pid->Output = 0;
}


void PID_Calculate(PID *pid, float current_value) {
    float error = pid->SetPoint - current_value;   // �������
    pid->SumError += error;                         // �ۼ�������ڻ���

    if((error>0 && pid->LastError<0) || (error<0 && pid->LastError>0)){
        pid->SumError = 0;
    }

    // ����PID���
    pid->Output = pid->Proportion * error                     // ������
                 + pid->Integral * pid->SumError              // ������
                 + pid->Derivative * (error - pid->LastError); // ΢����

    if (pid->Output <= 0 && pid->LastOutput <= 0 && clac_flag == 0){
        pid->Output = 0;
    } else if (pid->Output <= 0 && clac_flag == 1){
        pid->Output = 5;
    }


    // ����������
    pid->PreError = pid->LastError;
    pid->LastError = error;
    pid->LastOutput = pid->Output;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       �����жϳ�ʼ��
// @param       void
// @return      void
// @note        ʹ����શ�ʱ�����������ж�
//              ��શ�ʱ�������������жϣ��Ͳ�������Ϊ��ʱʹ����
//-------------------------------------------------------------------------------------------------------------------
void pit_init(void)
{
    // 10Khz
    // 100us ��һ���ж�
    SysTick->CTLR = 0;
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = SystemCoreClock / 10000;
    SysTick->CTLR = 0xF;
    NVIC_SetPriority(SysTicK_IRQn, 0xE0);
    NVIC_EnableIRQ(SysTicK_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ǰ���������ʼ��
// @param       void
// @return      void
// @note        ʹ�ö�ʱ��3
//-------------------------------------------------------------------------------------------------------------------
void advance_commutation_count_init(void)
{
    // 1 us����һ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2 /1000000) - 1;                    // ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM3_IRQn, 0x00);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM_Cmd(TIM3, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ʱ�������ʼ��
// @param       void
// @return      void
// @note        ʹ�ö�ʱ��4
//-------------------------------------------------------------------------------------------------------------------
void commutation_time_count_init(void)
{
    // 1 us����һ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2 / 1000000) - 1;                 // ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                            // �ظ�����������Ϊ0
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM4_IRQn, 0x02);
    NVIC_EnableIRQ(TIM4_IRQn);
    TIM_Cmd(TIM4, ENABLE);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       �����ж����VTF��Ӧ
// @param       void
// @return      void
// @note        ���ж�����Ϊ���ѯ�ж���������ת
//              ������˽�����жϣ������в���оƬ�ֲ�
//-------------------------------------------------------------------------------------------------------------------
void set_vector_table_free(void)
{
    extern void TIM3_IRQHandler(void);
    extern void EXTI2_IRQHandler(void);
    extern void TIM1_UP_IRQHandler(void);
    extern void SysTick_Handler(void);
    SetVTFIRQ((uint32)EXTI2_IRQHandler,   EXTI2_IRQn,     0, ENABLE);
    SetVTFIRQ((uint32)TIM3_IRQHandler,    TIM3_IRQn,      1, ENABLE);
    SetVTFIRQ((uint32)TIM1_UP_IRQHandler, TIM1_UP_IRQn,   2, ENABLE);
    SetVTFIRQ((uint32)SysTick_Handler,    SysTicK_IRQn,   3, ENABLE);
}
