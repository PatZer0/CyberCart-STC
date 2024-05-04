#include "headfile.h"
#include "hal.h"

#define HOST_COMM_BUFFER_SIZE 64

bit     sendok = 0;
bit     senderr = 0;
bit     sendwrongcmd = 0;
bit     sendwheelok = 0;
bit     sendwheelwarnexceed = 0;

// �ٶ��ַ��������������ڲ�ʹ��
int wheel_speed_parser(const char *str)
{
    // ���ַ���ת��Ϊ����
    int result = 0;  // ��ʼ�����Ϊ0
    int sign = 1;    // ���ű�ǣ�Ĭ��Ϊ����
    int i = 0;

    // ����Ƿ�Ϊ���ַ���
    if (str[0] == '\0') 
    {
        return 0;
    }

    // ����ǰ���ո������Ҫ����ո�������
    while (str[i] == ' ') 
    {
        i++;
    }

    // ������
    if (str[i] == '-') 
    {
        sign = -1;   // ����Ǹ��ţ����÷���Ϊ��
        i++;         // �ƶ�����һ���ַ�
    } else if (str[i] == '+') 
    {
        i++;         // ��������ţ����ƶ�����һ���ַ���sign����Ϊ1��
    }

    // �����ַ���ֱ������������'\0'
    while (str[i] != '\0') 
    {
        if (str[i] >= '0' && str[i] <= '9') {  // ȷ���ַ�������
            result = result * 10 + (str[i] - '0');  // ���ַ�ת��Ϊ��Ӧ���������ӵ������
        } else {
            break; // ��������������ַ���ֹͣ����
        }
        i++;
    }
    if(result > WHEEL_PWM_MAX_VAL)
    {
        result = WHEEL_PWM_MAX_VAL;
        sendwheelwarnexceed = 1; // �޷���PWM���ֵ
    }
    return sign * result;  // ���ش�����ȷ���ŵĽ��
}

void host_comm_uart_init(void)
{
	// P_SW1 &= ~0xc0;						//UART1/USART1: RxD(P3.0), TxD(P3.1)
	// P_SW1 = (P_SW1 & ~0xc0) | 0x40;		//UART1/USART1: RxD(P3.6), TxD(P3.7)
    
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
    unsigned char wheel_speed_buffer[7];
    int wheel_speed;
    unsigned char servo_angle_buffer[3];
    int servo_angle;

    if ((uart1_rx_buffer[uart1_rx_counter] == '\n'))
    {
        // ���յ�AT����
        if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   
        {
            led_1 = !led_1;
            // ���յ�SYS����
            if      ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
            {
                // ���յ�SYS?����
                if(uart1_rx_buffer[6] == '?') sendok = 1;
                else sendwrongcmd = 1;
            }
            // ���յ�WHL����
            else if ((uart1_rx_buffer[3] == 'W') && (uart1_rx_buffer[4] == 'H') && (uart1_rx_buffer[5] == 'L'))
            {
                // ��Index10��ʼ��д�뻺����
                for(i = 0; i < 7; i++)
                {
                    // ��鵽\r����
                    if(uart1_rx_buffer[10+i] == '\r') break;
                    wheel_speed_buffer[i] = uart1_rx_buffer[10+i];
                }
                // �����ַ���
                wheel_speed = wheel_speed_parser(wheel_speed_buffer);

                // �����ٶȽ�����д���ٶ�����
                if(uart1_rx_buffer[7] == 'X')
                {
                    if(uart1_rx_buffer[8] == 'A')       // �������������ٶ�
                    {
                        wheel_x_front_speed = wheel_speed;
                        wheel_x_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'F')  // ����ǰ�����ٶ�
                    {
                        wheel_x_front_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'R')  // ���ú������ٶ�
                    {
                        wheel_x_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else sendwrongcmd = 1;
                }
                else if(uart1_rx_buffer[7] == 'Y')
                {
                    if(uart1_rx_buffer[8] == 'A')       // �������������ٶ�
                    {
                        wheel_y_front_speed = wheel_speed;
                        wheel_y_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'F')  // ����ǰ�����ٶ�
                    {
                        wheel_y_front_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'R')  // ���ú������ٶ�
                    {
                        wheel_y_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else sendwrongcmd = 1;
                }
                else if(uart1_rx_buffer[7] == 'R' && uart1_rx_buffer[8] == 'R')
                {
                    wheel_x_front_speed = -wheel_speed;
                    wheel_x_rear_speed = wheel_speed;
                    wheel_y_front_speed = wheel_speed;
                    wheel_y_rear_speed = -wheel_speed;
                    sendwheelok = 1;
                }
            }
            // ���յ�SVO����
            else if ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'V') && (uart1_rx_buffer[5] == 'O'))
            {
                // AT+SVO+CLW=xxx   ��еצ�Ƕ�1-3λ��
                // AT+SVO+PTH=xxx   ��еצ����1-3λ��
                // AT+SVO+PAN=xxx   ��̨ˮƽ��ת1-3λ��
                // ��Index11��ʼ��д�뻺����
                for(i = 0; i < 3; i++)
                {
                    servo_angle_buffer[i] = uart1_rx_buffer[11+i] - '0';
                }
                // �����ַ���
                servo_angle = (servo_angle_buffer[0] * 100 + servo_angle_buffer[1] * 10 + servo_angle_buffer[2]);

                // �����ǶȽ�����д��Ƕ�����
                if(uart1_rx_buffer[7] == 'C')
                {
                    servo_claw_set_angle(servo_angle);
                    sendok = 1;
                }
                else if(uart1_rx_buffer[7] == 'P' && uart1_rx_buffer[8] == 'T' && uart1_rx_buffer[9] == 'H')
                {
                    servo_pitch_set_angle(servo_angle);
                    sendok = 1;
                }
                else if(uart1_rx_buffer[7] == 'P' && uart1_rx_buffer[8] == 'A' && uart1_rx_buffer[9] == 'N')
                {
                    servo_pan_set_angle(servo_angle);
                    sendok = 1;
                }
                else sendwrongcmd = 1;
            }
            else sendwrongcmd = 1;
        }
        else sendwrongcmd = 1;
    }
    else sendwrongcmd = 1;
}

void host_comm_sender(void)
{
    // unsigned char host_comm_header = 0x90;
    if (sendok)
    {
        // uart_sendbyte(1, host_comm_header);
        uart_sendstring(1, "OK\r\n");
        sendok = 0;
        uart1_rx_counter = 0;
    }
    if (senderr)
    {
        // uart_sendbyte(1, host_comm_header);
        uart_sendstring(1, "ERROR\r\n");
        senderr = 0;
        uart1_rx_counter = 0;
    }
    if(sendwrongcmd)
    {
        // uart_sendbyte(1, host_comm_header);
        uart_sendstring(1, "ERROR:CMD\r\n");
        sendwrongcmd = 0;
        uart1_rx_counter = 0;
    }
    if(sendwheelok)
    {
        // uart_sendbyte(1, host_comm_header);
        uart_sendstring(1, "OK\r\n");
        sendwheelok = 0;
        uart1_rx_counter = 0;
    }
    if(sendwheelwarnexceed)
    {
        // uart_sendbyte(1, host_comm_header);
        uart_sendstring(1, "WARNING:WHEEL_SPEED_EXCEEDED\r\n");
        sendwheelwarnexceed = 0;
        uart1_rx_counter = 0;
    }
}