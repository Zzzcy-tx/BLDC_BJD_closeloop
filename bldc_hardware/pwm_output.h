#include "headfile.h"
#include "bldc_config.h"/*********************************************************************************************************************
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
* 文件名称          pwm_output
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
