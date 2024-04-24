#include "headfile.h"
#include "host_comm.h"
#include "uart.h"
#include "qmc5883.h"
#include "key.h"

#define HOST_COMM_BUFFER_SIZE 64

bit     sendok = 0;
bit     senderr = 0;

void host_comm_uart_init(void)
{
    // 写入寄存器，设置串口1使用TIMER1，波特率921600

    SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xF7;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能
}

void host_comm_irqhandler()
{
    uint8 i;
    uint8 oled_row;
    uint8 oled_txt[64];

    if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   // 接收到AT命令
    {
        led_1 = !led_1;
        if((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
        {
            if(uart1_rx_buffer[7] == '?') // 接收到SYS?命令
            {
                sendok = 1;
            }
            else senderr = 1;
        }
        else if((uart1_rx_buffer[3] == 'O') && (uart1_rx_buffer[4] == 'L') && (uart1_rx_buffer[5] == 'E') && (uart1_rx_buffer[6] == 'D')) // 接收到OLED命令
        {
            if((uart1_rx_buffer[8] == 'T') && (uart1_rx_buffer[9] == 'X') && (uart1_rx_buffer[10] == 'T')) // 接收到TXT命令
            {
                // 第13个字节代表写入的行号，第13个字节是"="
                oled_row = uart1_rx_buffer[12];
                // 从第14个字节开始，写入oled_txt直到uart1_rx_counter
                for (i == 0; i < uart1_rx_counter - 13; i++)
                {
                    oled_txt[i] = uart1_rx_buffer[13 + i];
                }
                // 显示oled_txt
                oled_p6x8str_spi(0, oled_row, oled_txt);
                // 发送OK
                sendok = 1;
            }
            else senderr = 1;
        }
        else senderr == 1;
        sendok = 1;
    }
    else senderr = 1;
}

void host_comm_sender(void)
{
    if (sendok)
    {
        uart_sendstring(1, "OK\r\n");
        sendok = 0;
    }
    if (senderr)
    {
        uart_sendstring(1, "ERROR\r\n");
        senderr = 0;
    }
    // 发送QMC5883L数据
    // if(uart3_rx_rdy)
    // {
    //     uart_sendstring(1, qmc5883_char_yaw);
    //     uart_sendstring(1, "\r\n");
    //     uart3_rx_rdy = 0;
    // }
}