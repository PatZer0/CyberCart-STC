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
}

void host_comm_irqhandler()
{
    uint8 i;
    uint8 oled_row;
    uint8 oled_txt[64];

    if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   // ���յ�AT����
    {
        led_1 = !led_1;
        if((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
        {
            if(uart1_rx_buffer[7] == '?') // ���յ�SYS?����
            {
                sendok = 1;
            }
            else senderr = 1;
        }
        else if((uart1_rx_buffer[3] == 'O') && (uart1_rx_buffer[4] == 'L') && (uart1_rx_buffer[5] == 'E') && (uart1_rx_buffer[6] == 'D')) // ���յ�OLED����
        {
            if((uart1_rx_buffer[8] == 'T') && (uart1_rx_buffer[9] == 'X') && (uart1_rx_buffer[10] == 'T')) // ���յ�TXT����
            {
                // ��13���ֽڴ���д����кţ���13���ֽ���"="
                oled_row = uart1_rx_buffer[12];
                // �ӵ�14���ֽڿ�ʼ��д��oled_txtֱ��uart1_rx_counter
                for (i == 0; i < uart1_rx_counter - 13; i++)
                {
                    oled_txt[i] = uart1_rx_buffer[13 + i];
                }
                // ��ʾoled_txt
                oled_p6x8str_spi(0, oled_row, oled_txt);
                // ����OK
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
    // ����QMC5883L����
    // if(uart3_rx_rdy)
    // {
    //     uart_sendstring(1, qmc5883_char_yaw);
    //     uart_sendstring(1, "\r\n");
    //     uart3_rx_rdy = 0;
    // }
}