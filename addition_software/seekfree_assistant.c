/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library ����CH32V307VCT6 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ���CH32V307VCT6 ��Դ���һ����
*
* CH32V307VCT6 ��Դ�� ��������
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
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          seekfree_assiatant
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MounRiver Studio V1.8.1
* ����ƽ̨          CH32V307VCT6
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����                                      ����                                     ��ע
* 2023-11-29     ��W             first version
********************************************************************************************************************/

#include "seekfree_assistant.h"



//-------------------------------------------------------------------------------------------------------------------
// �������     ���������ͺ���
// ����˵��     *buffer         ��ҪУ������ݵ�ַ
// ����˵��     length          У�鳤��
// ���ز���     uint8           ��ֵ
// ʹ��ʾ��
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
// �������     ������� ����ʾ�������ͺ���
// ����˵��     *seekfree_assistant_oscilloscope  ʾ�������ݽṹ��
// ���ز���     void
// ʹ��ʾ��     seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_assistant_oscilloscope_send (seekfree_assistant_oscilloscope_struct *seekfree_assistant_oscilloscope)
{
    uint8 packet_size;

    if(seekfree_assistant_oscilloscope->channel_num == 0)
    {
        seekfree_assistant_oscilloscope->channel_num = 8;
    }

    // ������λ���
    seekfree_assistant_oscilloscope->channel_num &= 0x0f;

    // ֡ͷ
    seekfree_assistant_oscilloscope->head         = SEEKFREE_ASSISTANT_SEND_HEAD;

    // д���������Ϣ
    packet_size                         = sizeof(seekfree_assistant_oscilloscope_struct) - (SEEKFREE_ASSISTANT_SET_OSCILLOSCOPE_COUNT - seekfree_assistant_oscilloscope->channel_num) * 4;
    seekfree_assistant_oscilloscope->length       = packet_size;

    // д�빦������ͨ������
    seekfree_assistant_oscilloscope->channel_num |= SEEKFREE_ASSISTANT_OSCILLOSCOPE_FUN;

    // ��У�����
    seekfree_assistant_oscilloscope->check_sum    = 0;
    seekfree_assistant_oscilloscope->check_sum    = seekfree_assistant_sum((uint8 *)seekfree_assistant_oscilloscope, packet_size);

    // �����ڵ��ñ�����֮ǰ�����û�����Ҫ���͵�����д��seekfree_assistant_oscilloscope_data.data[]

    seekfree_assistant_transfer((const uint8 *)seekfree_assistant_oscilloscope, packet_size);
}

