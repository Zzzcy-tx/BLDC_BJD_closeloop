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

#include "pwm_output.h"

uint8 comp_pwm = 1;

//-------------------------------------------------------------------------------------------------------------------
// @brief       互补PWM占空比更新
// @param       void
// @return      duty    占空比
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty_update(uint16 duty)
{
    if (BLDC_MAX_DUTY < duty)
    {
        duty = BLDC_MAX_DUTY;
    }

    TIM1->ATRLR = BLDC_MAX_DUTY - 1;
    TIM1->CH1CVR = duty;
    TIM1->CH2CVR = duty;
    TIM1->CH3CVR = duty;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       A相输出PWM，B相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_a_bn_output(uint8 comp_pwm)
{
    // A相上桥PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    if(comp_pwm)
    {
        // A相下桥关
        PHASE_A_GPIO_PORT_LOW->CFGLR &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR |= (0xb<<28);
    }
    else
    {
        // A相下桥关
        PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
        PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    }
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B相下桥开
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;//high close
    // C相下桥关
    PHASE_C_GPIO_PORT_LOW->CFGLR &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       A相输出PWM，C相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_a_cn_output(uint8 comp_pwm)
{
    // A相上桥PWM
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0xb<<0);
    if(comp_pwm)
    {
        // A相下桥PWM
        PHASE_A_GPIO_PORT_LOW->CFGLR &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR |= (0xb<<28);
    }
    else
    {
        // A相下桥关
        PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
        PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
        PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    }
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C相下桥开
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH; //high close
    // B相下桥关
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       B相输出PWM，C相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_b_cn_output(uint8 comp_pwm)
{
    // B相上桥PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    if(comp_pwm)
    {
        // B相下桥PWM
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0xb<<0);
    }
    else
    {
        // B相下桥关
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
        PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
    }
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C相下桥开
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_C_GPIO_LOW;
    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A相下桥关
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       B相输出PWM，A相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_b_an_output(uint8 comp_pwm)
{
    // B相上桥PWM
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0xb<<4);
    if(comp_pwm)
    {
        // B相下桥PWM
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0xb<<0);
    }
    else
    {
        // B相下桥关
        PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
        PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
        PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
    }

    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH; //high close
    // A相下桥开
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;   // low on
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;//high close
    // C相下桥关
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       C相输出PWM，A相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_c_an_output(uint8 comp_pwm)
{
    // C相上桥PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    if(comp_pwm)
    {
        // C相下桥PWM
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0xb<<4);
    }
    else
    {
        // C相下桥关
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
        PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
    }

    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH; //high close
    // A相下桥开
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_A_GPIO_LOW;   // low on
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH; //high close
    // B相下桥关
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;  //low close
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       C相输出PWM，B相下桥开
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_c_bn_output(uint8 comp_pwm)
{
    // C相上桥PWM
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0xb<<8);
    if(comp_pwm)
    {
        // C相下桥PWM
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0xb<<4);
    }
    else
    {
        // C相下桥关
        PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
        PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
        PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;  //low close
    }
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B相下桥开
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_ON = PHASE_B_GPIO_LOW;
    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A相下桥关
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       三相全关
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_an_bn_cn_off()
{
    // A相上桥关
    PHASE_A_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<0);
    PHASE_A_GPIO_PORT_HIGH->CFGHR |= (0x3<<0);
    PHASE_A_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_A_GPIO_HIGH;
    // A相下桥关
    PHASE_A_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<28);
    PHASE_A_GPIO_PORT_LOW->CFGLR|= (0x3<<28);
    PHASE_A_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_A_GPIO_LOW;
    // B相上桥关
    PHASE_B_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<4);
    PHASE_B_GPIO_PORT_HIGH->CFGHR |= (0x3<<4);
    PHASE_B_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_B_GPIO_HIGH;
    // B相下桥关
    PHASE_B_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<0);
    PHASE_B_GPIO_PORT_LOW->CFGLR|= (0x3<<0);
    PHASE_B_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_B_GPIO_LOW;
    // C相上桥关
    PHASE_C_GPIO_PORT_HIGH->CFGHR &= ~(0xf<<8);
    PHASE_C_GPIO_PORT_HIGH->CFGHR |= (0x3<<8);
    PHASE_C_GPIO_PORT_HIGH->HIGH_BITREG_OFF = PHASE_C_GPIO_HIGH;
    // C相下桥关
    PHASE_C_GPIO_PORT_LOW->CFGLR  &= ~(0xf<<4);
    PHASE_C_GPIO_PORT_LOW->CFGLR |= (0x3<<4);
    PHASE_C_GPIO_PORT_LOW->LOW_BITREG_OFF = PHASE_C_GPIO_LOW;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       PWM互补输出初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pwm_complementary_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);            // 重映射
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = PHASE_A_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_A_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_B_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_B_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_C_GPIO_LOW;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_C_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_A_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_A_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_B_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_B_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = PHASE_C_GPIO_HIGH;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PHASE_C_GPIO_PORT_HIGH, &GPIO_InitStruct);
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_BDTRInitTypeDef     TIM_BDTRInitStructure = {0};
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = BLDC_MAX_DUTY - 1;                   // 设置定时器周期值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                                // 定时器时钟不分频
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;             // 中心对齐模式  PWM频率=定时器输入时钟频率/(2*周期值)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                        // 占空比的更新频率=定时器输入时钟频率/((TIM_RepetitionCounter+1)*周期值)
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           // PWM模式1，定时器计数值小于比较值的时候，引脚输出有效电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;               // CHx通道使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;             //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   // CHx  有效电平为高电平 无效电平为低电平
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;                // CHx  空闲状态输出低电平
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;                 // CHxN 有效电平为高电平 无效电平为低电平
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;              // CHxN 空闲状态输出低电平
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC1FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC2FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC3FastConfig(TIM1, TIM_OCFast_Disable);
//    TIM_OC4FastConfig(TIM1, TIM_OCFast_Disable);
//    TIM_CCPreloadControl(TIM1, ENABLE);
    TIM_BDTRStructInit(&TIM_BDTRInitStructure);
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = BLDC_PWM_DEADTIME;                          // PWM死区时间
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                        // 刹车失能
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;           // 刹车极性为高，高电平的时候刹车
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;    // 当刹车信号无效的时候自动开启输出
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Break,    DISABLE);                   // 关闭刹车中断
    TIM_ITConfig(TIM1, TIM_IT_Update,   DISABLE);                   // 关闭更新中断
    TIM_ITConfig(TIM1, TIM_IT_Trigger,  DISABLE);                   // 允许定时器更新中断
    TIM_ITConfig(TIM1, TIM_IT_COM,      DISABLE);
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);                             // 使能自动重装载预装载使能
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update);

}
