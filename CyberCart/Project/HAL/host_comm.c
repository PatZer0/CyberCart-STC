#include "headfile.h"
#include "hal.h"

#define HOST_COMM_BUFFER_SIZE 16

unsigned char host_comm_tx_buffer[HOST_COMM_BUFFER_SIZE];

bit     sendok = 0;
bit     senderr = 0;
bit     sendwrongcmd = 0;
bit     sendwheelok = 0;
bit     sendwheelwarnexceed = 0;

// �ڲ�ʹ�õ��ַ���ת��������
int internal_char_to_int(const char *str)
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

void host_comm_send_checksum_err()
{
    senderr = 1;
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
    unsigned char stepper_step_buffer[5];
    int stepper_step;

    // oled_p6x8str_spi(0, 1, uart1_rx_buffer);

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
                    if(uart1_rx_buffer[10+i] == '\r')
                    {
                        wheel_speed_buffer[i] = '\0';
                        break;
                    }
                    wheel_speed_buffer[i] = uart1_rx_buffer[10+i];
                }
                // �����ַ���
                wheel_speed = internal_char_to_int(wheel_speed_buffer);
                host_comm_send_data(wheel_speed_buffer);       // ����host_comm_send_data()��������
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
                    if(uart1_rx_buffer[11+i] == '\r')
                    {
                        servo_angle_buffer[i] = '\0';
                        break;
                    }
                    servo_angle_buffer[i] = uart1_rx_buffer[11+i];
                }
                // host_comm_send_data(servo_angle_buffer);       // ����host_comm_send_data()��������
                // �����ַ���
                servo_angle = internal_char_to_int(servo_angle_buffer);

                // �����ǶȽ�����д��Ƕ�����
                if(uart1_rx_buffer[7] == 'C' && uart1_rx_buffer[8] == 'L' && uart1_rx_buffer[9] == 'W')
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
            // ���յ�STP����
            else if ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'T') && (uart1_rx_buffer[5] == 'P'))
            {
                // AT+STP=+/-xxxx �����������
                // ��Index7��ʼ��д�뻺����
                for(i = 0; i < 5; i++)
                {
                    if(uart1_rx_buffer[7+i] == '\r')
                    {
                        stepper_step_buffer[i] = '\0';
                        break;
                    }
                    stepper_step_buffer[i] = uart1_rx_buffer[7+i];
                }
                // �����ַ���
                stepper_step = internal_char_to_int(stepper_step_buffer);
                stepper_adjust(stepper_step);
                sendok = 1;
            }
            else sendwrongcmd = 1;
        }
        else sendwrongcmd = 1;
    }
    else sendwrongcmd = 1;
}

void host_comm_send_data(const char *buffer)
{
    unsigned int i;
    for(i = 0; i < 64; i++)
    {
        host_comm_tx_buffer[i] = buffer[i];
    }
}

void host_comm_sender(void)
{
    if(host_comm_tx_buffer != NULL)
    {
        uart_sendstring(1, host_comm_tx_buffer);
        host_comm_tx_buffer[0] = '\0';
    }
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