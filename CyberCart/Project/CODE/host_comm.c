#include "headfile.h"
#include "host_comm.h"
#include "uart.h"
#include "qmc5883.h"
#include "key.h"
#include "motor_driver_boards.h"

#define HOST_COMM_BUFFER_SIZE 64

bit     sendok = 0;
bit     senderr = 0;
bit     sendwrongcmd = 0;
bit     sendwheelok = 0;
bit     sendwheelwarnexceed = 0;

int wheel_speed_parser(const char *speed_char)
{

}

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

    uart_sendstring(1, "STC32G12K128 READY. \r\n");
}

void host_comm_irqhandler()
{
    unsigned int i;
    unsigned int oled_row;
    unsigned int oled_txt[64];
    unsigned char wheel_speed_buffer[6];
    int wheel_speed;

    if ((uart1_rx_buffer[uart1_rx_counter] == '\n'))
    {
        // 接收到AT命令
        if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   
        {
            led_1 = !led_1;
            // 接收到SYS命令
            if((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
            {
                // 接收到SYS?命令
                if(uart1_rx_buffer[6] == '?') sendok = 1;
                else sendwrongcmd = 1;
            }
            // 接收到WHL命令
            else if((uart1_rx_buffer[3] == 'W') && (uart1_rx_buffer[4] == 'H') && (uart1_rx_buffer[5] == 'L'))
            {
                if(uart1_rx_buffer[7] == 'X')
                {
                    // if(uart1_rx_buffer[8] == '=')
                    // {
                    //     // 从index=9开始，写入wheel_speed_buffer
                    //     for (i = 0; i < 6; i++)
                    //     {
                    //         if(uart1_rx_buffer[9 + i] == '\r') break;
                    //         wheel_speed_buffer[i] = uart1_rx_buffer[9 + i];
                    //     }
                    //     wheel_speed = atoi(wheel_speed_buffer);
                    //     wheel_x_front_speed = wheel_speed;
                    //     wheel_x_rear_speed = wheel_speed;
                    //     // 发送OK
                    //     sendwheelok = 1;
                    // }
                    // else
                    // {
                    //     // 从index=10开始，写入wheel_speed_buffer
                    //     for (i = 0; i < 6; i++)
                    //     {
                    //         wheel_speed_buffer[i] = uart1_rx_buffer[10 + i];
                    //     }
                    //     wheel_speed = atoi(wheel_speed_buffer);
                    //     if(uart1_rx_buffer[8] == 'F')
                    //     {
                    //         wheel_x_front_speed = wheel_speed;
                    //         sendwheelok = 1;
                    //     }
                    //     else if (uart1_rx_buffer[8] == 'R')
                    //     {
                    //         wheel_x_rear_speed = wheel_speed;
                    //         sendwheelok = 1;
                    //     }
                    //     else sendwrongcmd = 1;
                    // }
                }
            }
            else sendwrongcmd = 1;
        }
        else sendwrongcmd = 1;
    }
    else sendwrongcmd = 1;
}

void host_comm_sender(void)
{
    if (sendok)
    {
        uart_sendstring(1, "OK:SYS\r\n");
        sendok = 0;
        uart1_rx_counter = 0;
    }
    if (senderr)
    {
        uart_sendstring(1, "ERROR\r\n");
        senderr = 0;
        uart1_rx_counter = 0;
    }
    if(sendwrongcmd)
    {
        uart_sendstring(1, "ERROR:CMD\r\n");
        sendwrongcmd = 0;
        uart1_rx_counter = 0;
    }
    if(sendwheelok)
    {
        uart_sendstring(1, "OK:WHEEL\r\n");
        sendwheelok = 0;
        uart1_rx_counter = 0;
    }
    if(sendwheelwarnexceed)
    {
        uart_sendstring(1, "WARNING:WHEEL_SPEED_EXCEEDED\r\n");
        sendwheelwarnexceed = 0;
        uart1_rx_counter = 0;
    }
    // 发送QMC5883L数据
    // if(uart3_rx_rdy)
    // {
    //     uart_sendstring(1, qmc5883_char_yaw);
    //     uart_sendstring(1, "\r\n");
    //     uart3_rx_rdy = 0;
    // }
}