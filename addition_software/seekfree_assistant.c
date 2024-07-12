/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是CH32V307VCT6 开源库的一部分
*
* CH32V307VCT6 开源库 是免费软件
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
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          seekfree_assiatant
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio V1.8.1
* 适用平台          CH32V307VCT6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期                                      作者                                     备注
* 2023-11-29     大W             first version
********************************************************************************************************************/

#include "seekfree_assistant.h"



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手求和函数
// 参数说明     *buffer         需要校验的数据地址
// 参数说明     length          校验长度
// 返回参数     uint8           和值
// 使用示例
//-------------------------------------------------------------------------------------------------------------------
static uint8 seekfree_assistant_sum (uint8 *buffer, uint32 length)
{
    uint8 temp_sum = 0;

    while(length--)
    {
        temp_sum += *buffer++;
    }

    return temp_sum;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     逐飞助手 虚拟示波器发送函数
// 参数说明     *seekfree_assistant_oscilloscope  示波器数据结构体
// 返回参数     void
// 使用示例     seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_send (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope)
{
    uint8 packet_size;

    if(seekfree_assistant_oscilloscope->channel_num == 0)
    {
        seekfree_assistant_oscilloscope->channel_num = 8;
    }

    // 将高四位清空
    seekfree_assistant_oscilloscope->channel_num &= 0x0f;

    // 帧头
    seekfree_assistant_oscilloscope->head         = SEEKFREE_ASSISTANT_SEND_HEAD;

    // 写入包长度信息
    packet_size                         = sizeof(seekfree_assistant_oscilloscope_struct) - (SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT - seekfree_assistant_oscilloscope->channel_num) * 4;
    seekfree_assistant_oscilloscope->length       = packet_size;

    // 写入功能字与通道数量
    seekfree_assistant_oscilloscope->channel_num |= SEEKFREE_ASSISTANT_OSCILLOSCOPE_FUN;

    // 和校验计算
    seekfree_assistant_oscilloscope->check_sum    = 0;
    seekfree_assistant_oscilloscope->check_sum    = seekfree_assistant_sum((uint8 *)seekfree_assistant_oscilloscope, packet_size);

    // 数据在调用本函数之前，由用户将需要发送的数据写入seekfree_assistant_oscilloscope_data.data[]

    seekfree_assistant_transfer((const uint8 *)seekfree_assistant_oscilloscope, packet_size);
}

