/*********************************************************************************************************************
* CH32V203C8T6_BLDC_ESC 即（CH32V203C8T6 无感无刷电调）是一个基于官方 SDK 接口的第三方开源项目
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* CH32V203C8T6_BLDC_ESC 开源项目的一部分
*
* CH32V203C8T6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 bldc_config/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 bldc_config 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          battery
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio
* 适用平台          CH32V203C8T6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期            作者         备注
* 2024-01-04        大W          first version
********************************************************************************************************************/

#include "battery.h"

battery_struct battery = {0};

//-------------------------------------------------------------------------------------------------------------------
// @brief       电池电压获取
// @param       void
// @return      uint8         1-电压过低 0-电压正常
// @note        电池电压低于设定阈值，就返回1
//-------------------------------------------------------------------------------------------------------------------
uint8 battery_voltage_get(void)
{
    static uint16 low_power_num = 0;
    // 这里的5.7是分压电阻的倍数
    battery.voltage = (float)battery.adc_reg_value / 4095 * 5.7 * 3300;
    if((BLDC_MIN_VOLTAGE > battery.voltage))
    {
        // 10000次都小于阈值，则说明电压过低
        // 0.1ms采集一次，一共1000ms。
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
// @brief       电池电压检测初始化
// @param       void
// @return      void
// @note        通过定时器3去触发DMA，进行AD转换
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
//    ADC_ExternalTrigConvCmd(ADC1, ENABLE); //设置外部触发模式使能
}
