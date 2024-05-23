#include "headfile.h"
#include "hal.h"

#define HOST_COMM_BUFFER_SIZE 16

unsigned char host_comm_tx_buffer[HOST_COMM_BUFFER_SIZE];

bit     sendok = 0;
bit     senderr = 0;
bit     sendwrongcmd = 0;
bit     sendwheelok = 0;
bit     sendwheelwarnexceed = 0;

// 内部使用的字符串转整数函数
int internal_char_to_int(const char *str)
{
    // 将字符串转化为整数
    int result = 0;  // 初始化结果为0
    int sign = 1;    // 符号标记，默认为正数
    int i = 0;

    // 检查是否为空字符串
    if (str[0] == '\0') 
    {
        return 0;
    }

    // 跳过前导空格（如果需要处理空格的情况）
    while (str[i] == ' ') 
    {
        i++;
    }

    // 检查符号
    if (str[i] == '-') 
    {
        sign = -1;   // 如果是负号，设置符号为负
        i++;         // 移动到下一个字符
    } else if (str[i] == '+') 
    {
        i++;         // 如果是正号，仅移动到下一个字符（sign保持为1）
    }

    // 遍历字符串直到遇到结束符'\0'
    while (str[i] != '\0') 
    {
        if (str[i] >= '0' && str[i] <= '9') {  // 确保字符是数字
            result = result * 10 + (str[i] - '0');  // 将字符转换为相应的整数并加到结果中
        } else {
            break; // 如果遇到非数字字符，停止解析
        }
        i++;
    }
    if(result > WHEEL_PWM_MAX_VAL)
    {
        result = WHEEL_PWM_MAX_VAL;
        sendwheelwarnexceed = 1; // 限幅到PWM最大值
    }
    return sign * result;  // 返回带有正确符号的结果
}

void host_comm_send_checksum_err()
{
    senderr = 1;
}

void host_comm_uart_init(void)
{
	// P_SW1 &= ~0xc0;						//UART1/USART1: RxD(P3.0), TxD(P3.1)
	// P_SW1 = (P_SW1 & ~0xc0) | 0x40;		//UART1/USART1: RxD(P3.6), TxD(P3.7)
    
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
    unsigned char wheel_speed_buffer[7];
    int wheel_speed;
    unsigned char servo_angle_buffer[3];
    int servo_angle;
    unsigned char stepper_step_buffer[5];
    int stepper_step;

    // oled_p6x8str_spi(0, 1, uart1_rx_buffer);

    if ((uart1_rx_buffer[uart1_rx_counter] == '\n'))
    {
        // 接收到AT命令
        if ((uart1_rx_buffer[0] == 'A') && (uart1_rx_buffer[1] == 'T'))   
        {
            led_1 = !led_1;
            // 接收到SYS命令
            if      ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'Y') && (uart1_rx_buffer[5] == 'S'))
            {
                // 接收到SYS?命令
                if(uart1_rx_buffer[6] == '?') sendok = 1;
                else sendwrongcmd = 1;
            }
            // 接收到WHL命令
            else if ((uart1_rx_buffer[3] == 'W') && (uart1_rx_buffer[4] == 'H') && (uart1_rx_buffer[5] == 'L'))
            {
                // 从Index10开始，写入缓冲区
                for(i = 0; i < 7; i++)
                {
                    // 检查到\r结束
                    if(uart1_rx_buffer[10+i] == '\r')
                    {
                        wheel_speed_buffer[i] = '\0';
                        break;
                    }
                    wheel_speed_buffer[i] = uart1_rx_buffer[10+i];
                }
                // 解析字符串
                wheel_speed = internal_char_to_int(wheel_speed_buffer);
                host_comm_send_data(wheel_speed_buffer);       // 调用host_comm_send_data()发送数据
                // 解析速度结束，写入速度数据
                if(uart1_rx_buffer[7] == 'X')
                {
                    if(uart1_rx_buffer[8] == 'A')       // 设置所有轮子速度
                    {
                        wheel_x_front_speed = wheel_speed;
                        wheel_x_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'F')  // 设置前轮子速度
                    {
                        wheel_x_front_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'R')  // 设置后轮子速度
                    {
                        wheel_x_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else sendwrongcmd = 1;
                }
                else if(uart1_rx_buffer[7] == 'Y')
                {
                    if(uart1_rx_buffer[8] == 'A')       // 设置所有轮子速度
                    {
                        wheel_y_front_speed = wheel_speed;
                        wheel_y_rear_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'F')  // 设置前轮子速度
                    {
                        wheel_y_front_speed = wheel_speed;
                        sendwheelok = 1;
                    }
                    else if(uart1_rx_buffer[8] == 'R')  // 设置后轮子速度
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
            // 接收到SVO命令
            else if ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'V') && (uart1_rx_buffer[5] == 'O'))
            {
                // AT+SVO+CLW=xxx   机械爪角度1-3位数
                // AT+SVO+PTH=xxx   机械爪俯仰1-3位数
                // AT+SVO+PAN=xxx   云台水平旋转1-3位数
                // 从Index11开始，写入缓冲区
                for(i = 0; i < 3; i++)
                {
                    if(uart1_rx_buffer[11+i] == '\r')
                    {
                        servo_angle_buffer[i] = '\0';
                        break;
                    }
                    servo_angle_buffer[i] = uart1_rx_buffer[11+i];
                }
                // host_comm_send_data(servo_angle_buffer);       // 调用host_comm_send_data()发送数据
                // 解析字符串
                servo_angle = internal_char_to_int(servo_angle_buffer);

                // 解析角度结束，写入角度数据
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
            // 接收到STP命令
            else if ((uart1_rx_buffer[3] == 'S') && (uart1_rx_buffer[4] == 'T') && (uart1_rx_buffer[5] == 'P'))
            {
                // AT+STP=+/-xxxx 步进电机步数
                // 从Index7开始，写入缓冲区
                for(i = 0; i < 5; i++)
                {
                    if(uart1_rx_buffer[7+i] == '\r')
                    {
                        stepper_step_buffer[i] = '\0';
                        break;
                    }
                    stepper_step_buffer[i] = uart1_rx_buffer[7+i];
                }
                // 解析字符串
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