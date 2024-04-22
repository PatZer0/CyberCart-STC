/*
ע�⣺  �����򴮿�ͨ��ʹ��˽�п⣬û�����SEEKFREE_LIBRARY�޸ģ�����ʹ��SEEKFREE_LIBRARY���еĴ��ں�����������ɳ�ͻ
        ����������������ģ��ͬʱʹ�õ�������˴������������Ҫ�����޸�
*/
#include <STDLIB.H>
#include "laser_ranging.h"
#include "uart.h"

unsigned char laser_ranging_cmd_arr_single[]                = {0x80, 0x06, 0x02, 0x78};             // ���β���
unsigned char laser_ranging_cmd_arr_continous[]             = {0x80, 0x06, 0x03, 0x77};             // ��������
unsigned char laser_ranging_cmd_arr_shutdown[]              = {0x80, 0x04, 0x02, 0x7A};             // �ػ�
unsigned char laser_ranging_cmd_arr_setfreq_5hz[]           = {0xFA, 0x04, 0x0A, 0x05, 0xF3};       // ����Ƶ��5hz
unsigned char laser_ranging_cmd_arr_setfreq_10hz[]          = {0xFA, 0x04, 0x0A, 0x0A, 0xEE};       // ����Ƶ��10hz
unsigned char laser_ranging_cmd_arr_setfreq_20hz[]          = {0xFA, 0x04, 0x0A, 0x14, 0xE4};       // ����Ƶ��20hz
unsigned char laser_ranging_cmd_arr_setrange_5m[]           = {0xFA, 0x04, 0x09, 0x05, 0xF4};       // ���þ���ֵ5m
unsigned char laser_ranging_cmd_arr_setrange_10m[]          = {0xFA, 0x04, 0x09, 0x0A, 0xEF};       // ���þ���ֵ10m
unsigned char laser_ranging_cmd_arr_setrange_30m[]          = {0xFA, 0x04, 0x09, 0x1E, 0xDB};       // ���þ���ֵ30m
unsigned char laser_ranging_cmd_arr_setrange_50m[]          = {0xFA, 0x04, 0x09, 0x32, 0xC7};       // ���þ���ֵ50m
unsigned char laser_ranging_cmd_arr_setrange_80m[]          = {0xFA, 0x04, 0x09, 0x50, 0xA9};       // ���þ���ֵ80m
unsigned char laser_ranging_cmd_arr_setresolution_1mm[]     = {0xFA, 0x04, 0x0C, 0x01, 0xF5};       // ���÷ֱ���1mm
unsigned char laser_ranging_cmd_arr_setresolution_0_1mm[]   = {0xFA, 0x04, 0x0C, 0x02, 0xF4};       // ���÷ֱ���0.1mm
unsigned char laser_ranging_cmd_arr_single_broadcast[]      = {0xFA, 0x06, 0x06, 0xFA};             // ���β������㲥��
unsigned char laser_ranging_cmd_arr_readcache[]             = {0x80, 0x06, 0x07, 0x73};             // ��ȡ����
unsigned char laser_ranging_cmd_arr_laseron[]               = {0x80, 0x06, 0x05, 0x01, 0x74};       // ���Ƽ��⿪��
unsigned char laser_ranging_cmd_arr_laseroff[]              = {0x80, 0x06, 0x05, 0x00, 0x75};       // ���Ƽ���ر�

LASER_RANGING_CMD_DEF lrcmd_single              = {laser_ranging_cmd_arr_single,                4};
LASER_RANGING_CMD_DEF lrcmd_continous           = {laser_ranging_cmd_arr_continous,             4};
LASER_RANGING_CMD_DEF lrcmd_shutdown            = {laser_ranging_cmd_arr_shutdown,              5};
LASER_RANGING_CMD_DEF lrcmd_setfreq_5hz         = {laser_ranging_cmd_arr_setfreq_5hz,           5};
LASER_RANGING_CMD_DEF lrcmd_setfreq_10hz        = {laser_ranging_cmd_arr_setfreq_10hz,          5};
LASER_RANGING_CMD_DEF lrcmd_setfreq_20hz        = {laser_ranging_cmd_arr_setfreq_20hz,          5};
LASER_RANGING_CMD_DEF lrcmd_setrange_5m         = {laser_ranging_cmd_arr_setrange_5m,           5};
LASER_RANGING_CMD_DEF lrcmd_setrange_10m        = {laser_ranging_cmd_arr_setrange_10m,          5};
LASER_RANGING_CMD_DEF lrcmd_setrange_30m        = {laser_ranging_cmd_arr_setrange_30m,          5};
LASER_RANGING_CMD_DEF lrcmd_setrange_50m        = {laser_ranging_cmd_arr_setrange_50m,          5};
LASER_RANGING_CMD_DEF lrcmd_setrange_80m        = {laser_ranging_cmd_arr_setrange_80m,          5};
LASER_RANGING_CMD_DEF lrcmd_setresolution_1mm   = {laser_ranging_cmd_arr_setresolution_1mm,     5};
LASER_RANGING_CMD_DEF lrcmd_setresolution_0_1mm = {laser_ranging_cmd_arr_setresolution_0_1mm,   5};
LASER_RANGING_CMD_DEF lrcmd_single_broadcast    = {laser_ranging_cmd_arr_single_broadcast,      4};
LASER_RANGING_CMD_DEF lrcmd_readcache           = {laser_ranging_cmd_arr_readcache,             4};
LASER_RANGING_CMD_DEF lrcmd_laseron             = {laser_ranging_cmd_arr_laseron,               5};
LASER_RANGING_CMD_DEF lrcmd_laseroff            = {laser_ranging_cmd_arr_laseroff,              5};

LASER_RANGING_DATA_DEF lrdata;   // �洢���ֵ, ���÷�����lrdata.x/y.valuedata/chardata

void laser_ranging_irqhandler(unsigned char lr_axis)
{
    unsigned int i;
    switch (lr_axis)
    {
    case 'x':
        if (uart1_rx_buffer[0] == 0x80)
        {
            if(uart1_rx_counter > 2)
            {
                if(uart1_rx_buffer[1] == 0x06 && uart1_rx_buffer[2] == 0x83)
                {
                    if(uart1_rx_counter >= 11)
                    {
                        uart1_rx_counter=0;
                        if( (uart1_rx_buffer[3]<0x34) && (uart1_rx_buffer[3]!='E') && (uart1_rx_buffer[4]!='R') && (uart1_rx_buffer[5]!='R') && (uart1_rx_buffer[10] == (unsigned char)(~(0x80+0x06+0x83+uart1_rx_buffer[3]+uart1_rx_buffer[4]+uart1_rx_buffer[5]+0x2E+uart1_rx_buffer[7]+uart1_rx_buffer[8]+uart1_rx_buffer[9])+1)) ) 
                        {
                            // laser_ranging_value = (uart1_rx_buffer[4]-0x30)*10000+(uart1_rx_buffer[5]-0x30)*1000 + (uart1_rx_buffer[7]-0x30)*100 + (uart1_rx_buffer[8]-0x30)*10 + (uart1_rx_buffer[9]-0x30);                 
                            // sprintf(laser_ranging_char, "%f", laser_ranging_value);
                            // ��uart1_rx_buffer[3]~uart1_rx_buffer[10]�洢��laser_ranging_char������
                            for(i=3;i<11;i++)
                            {
                                lrdata.x.chardata[i-3] = uart1_rx_buffer[i];
                            }
                            // lrdata.x.valuedata = atof(lrdata.x.chardata);
                            // oled_printf_float_spi(6*6, 2, lrdata.x.valuedata, 2, 5);
                            // oled_printf_float_spi(6*6, 2, 0.1, 2, 5);
                            oled_p6x8str_spi(6*6, 2, lrdata.x.chardata);
                        }
                        else
                        {
                            lrdata.x.valuedata = -1.0;
                            lrdata.x.chardata[0] = 'E';
                            lrdata.x.chardata[1] = 'R';
                            lrdata.x.chardata[2] = 'R';
                        }
                    }
                }
            }
        }

        break;
    case 'y':
        if (uart2_rx_buffer[0] == 0x80)
        {
            if(uart2_rx_counter > 2)
            {
                if(uart2_rx_buffer[1] == 0x06 && uart2_rx_buffer[2] == 0x83)
                {
                    if(uart2_rx_counter >= 11)
                    {
                        uart2_rx_counter=0;

                        if( (uart2_rx_buffer[3]<0x34) && (uart2_rx_buffer[3]!='E') && (uart2_rx_buffer[4]!='R') && (uart2_rx_buffer[5]!='R') && (uart2_rx_buffer[10] == (unsigned char)(~(0x80+0x06+0x83+uart2_rx_buffer[3]+uart2_rx_buffer[4]+uart2_rx_buffer[5]+0x2E+uart2_rx_buffer[7]+uart2_rx_buffer[8]+uart2_rx_buffer[9])+1)) ) 
                        {
                            // laser_ranging_value = (uart2_rx_buffer[4]-0x30)*10000+(uart2_rx_buffer[5]-0x30)*1000 + (uart2_rx_buffer[7]-0x30)*100 + (uart2_rx_buffer[8]-0x30)*10 + (uart2_rx_buffer[9]-0x30);                 
                            // sprintf(laser_ranging_char, "%f", laser_ranging_value);
                            // ��uart2_rx_buffer[3]~uart2_rx_buffer[10]�洢��laser_ranging_char������
                            for(i=3;i<11;i++)
                            {
                                lrdata.y.chardata[i-3+16] = uart2_rx_buffer[i];
                            }
                            lrdata.y.valuedata = atof(lrdata.y.chardata);
                            // oled_printf_float_spi(6*6, 3, lrdata.y.valuedata, 2, 5);
                            // oled_p6x8str_spi(6*6, 3, lrdata.y.chardata);
                        }
                        else
                        {
                            lrdata.x.valuedata = -1.0;
                            lrdata.x.chardata[0] = 'E';
                            lrdata.x.chardata[1] = 'R';
                            lrdata.x.chardata[2] = 'R';
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

// ������lr_axis ����'x'��'y'��'z'
// ���أ�����������ļ�����UART���ں�
unsigned char laser_ranging_get_uart_value(unsigned char lr_axis)
{
    switch (lr_axis)
    {
    case 'x':
        return 1;
        break;
    
    case 'y':
        return 2;
        break;
    
    case 'z':
        return 3;
        break;
    
    default:
        return 1;
        break;
    }
}

// ������lr_axis ����'x'��'y'��'z'�� specific_cmd �ض�����ṹ��
void laser_ranging(unsigned char lr_axis, LASER_RANGING_CMD_DEF *specific_cmd)
{
    uart_sendcmd(laser_ranging_get_uart_value(lr_axis), specific_cmd->command, specific_cmd->length);
}

void laser_ranging_init()
{
    laser_ranging('x', &lrcmd_setfreq_5hz);
    laser_ranging('x', &lrcmd_setrange_80m);
    laser_ranging('x', &lrcmd_setresolution_0_1mm);
    laser_ranging('y', &lrcmd_setfreq_5hz);
    laser_ranging('y', &lrcmd_setrange_80m);
    laser_ranging('y', &lrcmd_setresolution_0_1mm);
}