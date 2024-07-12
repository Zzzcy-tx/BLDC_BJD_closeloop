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
* 文件名称          operational_amplifier
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

#include "operational_amplifier.h"
#include "pwm_output.h"
#include "motor.h"
#include "control.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       失去能PA2外部引脚中断
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void disable_opama_interrupts(void)
{
    EXTI->INTENR &= ~(1 << 2);          // 关闭PA2外部引脚中断
    EXTI->INTFR = EXTI_Line2;           // 清除中断标志位
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       失去能PA2外部引脚中断
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void enable_opama_interrupts(void)
{
//    EXTI->INTFR = EXTI_Line2;           // 清除中断标志位
    EXTI->INTENR |= (1 << 2);           // 打开PA2外部引脚中断
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       外部引脚中断初始化
// @param       void
// @return      void
// @note        PA2、PA3和PA4为运放输出引脚
//              在原理图上面PA2和PA3和PA4已经连接在一起
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
// @brief      运放初始化
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


