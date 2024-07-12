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
* �ļ�����          battery
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

#include "battery.h"

battery_struct battery = {0};

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ص�ѹ��ȡ
// @param       void
// @return      uint8         1-��ѹ���� 0-��ѹ����
// @note        ��ص�ѹ�����趨��ֵ���ͷ���1
//-------------------------------------------------------------------------------------------------------------------
uint8 battery_voltage_get(void)
{
    static uint16 low_power_num = 0;
    // �����5.7�Ƿ�ѹ����ı���
    battery.voltage = (float)battery.adc_reg_value / 4095 * 5.7 * 3300;
    if((BLDC_MIN_VOLTAGE > battery.voltage))
    {
        // 10000�ζ�С����ֵ����˵����ѹ����
        // 0.1ms�ɼ�һ�Σ�һ��1000ms��
        if(10000 < low_power_num++)
        {
            return 1;
        }
    }
    else
    {
        low_power_num = 0;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ص�ѹ����ʼ��
// @param       void
// @return      void
// @note        ͨ����ʱ��3ȥ����DMA������ADת��
//-------------------------------------------------------------------------------------------------------------------
void battery_voltage_init()
{
    ADC_InitTypeDef ADC_InitStructure= {0};
    GPIO_InitTypeDef GPIO_InitStructure= {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    DMA_InitTypeDef DMA_InitStructure = {0};
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)&ADC1->RDATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)&battery.adc_reg_value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//    Delay_Ms(50);
//    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//    ADC_ExternalTrigConvCmd(ADC1, ENABLE); //�����ⲿ����ģʽʹ��
}
