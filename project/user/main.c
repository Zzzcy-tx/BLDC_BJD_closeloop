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
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio
* 适用平台          CH32V203C8T6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期           	作者         备注
* 2024-01-04     	大W          first version
********************************************************************************************************************/

#include "math.h"
#include "pwm_input.h"
#include "pwm_output.h"
#include "motor.h"
#include "operational_amplifier.h"
#include "battery.h"
#include "iwdg.h"
#include "led.h"
#include "control.h"
#include "usart.h"
#include "seekfree_assistant.h"
#include "stdlib.h"
#include "motor.h"
#include "control.h"


// 电机未运行			RUN闪烁，ERR熄灭
// 正常运行           	RUN常亮, ERR熄灭
// 电池电压过低       	RUN熄灭，ERR常亮
// 电机堵转        		RUN熄灭，ERR闪烁

int main(void)
{
    // LED初始化
    led_init();

    // 设置中断免表VTF响应
    set_vector_table_free();

    // debug串口初始化
    usart_init(921600);

    // 电池电压检测初始化
    battery_voltage_init();

    // 初始化定时器1，用于输出互补PWM
    pwm_complementary_init();

    // 初始化定时器2，用作输入捕获
    pwm_input_init();

    // 初始化定时器3，用作提前换向计数
    advance_commutation_count_init();

    // 初始化定时器4，用作统计换向时间和换向超时判断
    commutation_time_count_init();


#if BLDC_BEEP_ENABLE
    // 电机播放音频
    motor_power_on_beep(BLDC_BEEP_VOLUME);
#endif

    // 运放初始化，用作比较器
    opama_init();

    // 使用PA2引脚的外部中断，作为比较器中断
    pa2_exit_init();

    // 看门狗初始化,0.1ms计数一次,触发时间为200ms
    iwdg_init(200*10);

    // 嘀嗒定时器，用作周期中断,10Khz频率
    // 嘀嗒定时器用作周期中断后，就不能再用于延时
    pit_init();

    PID_Init(&fan, 0, 10, 0.001, 0.03);

//    // 初始化逐飞助手示波器的结构体
   seekfree_assistant_oscilloscope_struct oscilloscope_data = {0};

    while(1)
    {
//        printf("%f,%f,%f\n", fan.SetPoint, 1000000.0/(float)motor.filter_commutation_time_sum, fan.Output);
        IWDG_ReloadCounter();           // 喂狗
//        printf("%u\n",motor.commutation_num);

       oscilloscope_data.data[0] = motor.run_flag;
       oscilloscope_data.data[1] = motor.duty;
       oscilloscope_data.data[2] = fan.SetPoint;
       oscilloscope_data.data[3] = 1000000/motor.filter_commutation_time_sum;
       oscilloscope_data.data[4] = motor.filter_commutation_time_sum;
       oscilloscope_data.data[5] = fan.Output;
       oscilloscope_data.data[6] = motor.commutation_num;
       oscilloscope_data.data[7] = battery.voltage;

       seekfree_assistant_oscilloscope_send(&oscilloscope_data);

    }
}



