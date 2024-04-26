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
    // д��Ĵ��������ô���1ʹ��TIMER1��������921600

    SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xF7;			//���ö�ʱ��ʼֵ
	TH1 = 0xFF;			//���ö�ʱ��ʼֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	ES = 1;				//ʹ��

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
        // ���յ�AT����
        if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   
        {
            led_1 = !led_1;
            // ���յ�SYS����
            if((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
            {
                // ���յ�SYS?����
                if(uart1_rx_buffer[6] == '?') sendok = 1;
                else sendwrongcmd = 1;
            }
            // ���յ�WHL����
            else if((uart1_rx_buffer[3] == 'W') && (uart1_rx_buffer[4] == 'H') && (uart1_rx_buffer[5] == 'L'))
            {
                if(uart1_rx_buffer[7] == 'X')
                {
                    // if(uart1_rx_buffer[8] == '=')
                    // {
                    //     // ��index=9��ʼ��д��wheel_speed_buffer
                    //     for (i = 0; i < 6; i++)
                    //     {
                    //         if(uart1_rx_buffer[9 + i] == '\r') break;
                    //         wheel_speed_buffer[i] = uart1_rx_buffer[9 + i];
                    //     }
                    //     wheel_speed = atoi(wheel_speed_buffer);
                    //     wheel_x_front_speed = wheel_speed;
                    //     wheel_x_rear_speed = wheel_speed;
                    //     // ����OK
                    //     sendwheelok = 1;
                    // }
                    // else
                    // {
                    //     // ��index=10��ʼ��д��wheel_speed_buffer
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
    // ����QMC5883L����
    // if(uart3_rx_rdy)
    // {
    //     uart_sendstring(1, qmc5883_char_yaw);
    //     uart_sendstring(1, "\r\n");
    //     uart3_rx_rdy = 0;
    // }
}