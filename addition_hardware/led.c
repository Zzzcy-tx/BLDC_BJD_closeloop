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
* 文件名称          led
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

#include "led.h"
#include "motor.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       led控制事件
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
// @brief       运行LED控制
// @param       void
// @return      LED_STATUS_enum         LED_ON-亮 LED_OFF-灭
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
// @brief       错误LED控制
// @param       void
// @return      LED_STATUS_enum         LED_ON-亮 LED_OFF-灭
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
// @brief       运行LED翻转
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
// @brief       错误LED翻转
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
// @brief       LED初始化
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

