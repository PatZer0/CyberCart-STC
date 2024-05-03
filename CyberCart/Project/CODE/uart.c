#include "headfile.h"
#include "hal.h"

unsigned char   uart1_tx_counter, uart2_tx_counter, uart3_tx_counter, uart4_tx_counter;   // 发送计数
unsigned char   uart1_rx_counter, uart2_rx_counter, uart3_rx_counter, uart4_rx_counter;   // 接收计数
bit             uart1_tx_busy, uart2_tx_busy, uart3_tx_busy, uart4_tx_busy;               // 发送忙标志
unsigned char   uart1_tx_buffer_from_uart2[16], 
                uart1_tx_buffer_from_uart3[16], 
                uart1_tx_buffer_from_uart4[16]; // 发送缓冲
unsigned char   uart1_tx_buffer_from_uart2_index, 
                uart1_tx_buffer_from_uart3_index, 
                uart1_tx_buffer_from_uart4_index; // 发送缓冲索引
unsigned char   uart1_rx_buffer[UART1_BUF_LENGTH];                                        // 接收缓冲
unsigned char   uart2_rx_buffer[UART2_BUF_LENGTH];                                        // 接收缓冲
unsigned char   uart3_rx_buffer[UART3_BUF_LENGTH];                                        // 接收缓冲
unsigned char   uart4_rx_buffer[UART4_BUF_LENGTH];                                        // 接收缓冲
bit             uart1_cr, uart2_cr, uart3_cr, uart4_cr;                                   // 回车标志
bit             uart1_lf, uart2_lf, uart3_lf, uart4_lf;                                   // 换行标志
bit             uart1_rx_rdy, uart2_rx_rdy, uart3_rx_rdy, uart4_rx_rdy;                   // 接收数据准备好标志

void uart_port_init(void)
{
	P_SW1 |= 0xc0;						//UART1/USART1: RxD(P4.3), TxD(P4.4)
	P_SW2 |= 0x01;						//UART2/USART2: RxD2(P4.6), TxD2(P4.7)
}

void uart_initialize(unsigned char uart_num)
{
    switch(uart_num)
    {
        case 1:
        	SCON = 0x50;		// 8位数据,可变波特率
            AUXR |= 0x01;		// 串口1选择定时器2为波特率发生器
            AUXR |= 0x04;		// 定时器时钟1T模式
            T2L = 0xA0;			// 设置定时初始值
            T2H = 0xFC;			// 设置定时初始值
            AUXR |= 0x10;		// 定时器2开始计时
            ES = 1;				// 使能串口1中断
            break;

        case 2:
        	S2CON = 0x50;		// 8位数据,可变波特率
            AUXR |= 0x04;		// 定时器时钟1T模式
            T2L = 0xA0;			// 设置定时初始值
            T2H = 0xFC;			// 设置定时初始值
            AUXR |= 0x10;		// 定时器2开始计时
            IE2 |= 0x01;		// 使能串口2中断
            break;

        default:
            break;
    }
}

void uart_sendstring(unsigned char uart_num, unsigned char *puts)
{
    switch(uart_num)
    {
        case 1:
            for (; *puts != 0;  puts++)                     // 遇到停止符0结束
            {
                SBUF = *puts;
                uart1_tx_busy = 1;
                while(uart1_tx_busy);
            }
            break;
        case 2:
            for (; *puts != 0;  puts++)                     // 遇到停止符0结束
            {
                S2BUF = *puts;
                uart2_tx_busy = 1;
                while(uart2_tx_busy);
            }
            break;
        case 3:
            for (; *puts != 0;  puts++)                     // 遇到停止符0结束
            {
                S3BUF = *puts;
                uart3_tx_busy = 1;
                while(uart3_tx_busy);
            }
            break;
        case 4:
            for (; *puts != 0;  puts++)                     // 遇到停止符0结束
            {
                S4BUF = *puts;
                uart4_tx_busy = 1;
                while(uart4_tx_busy);
            }
            break;
        default:
            break;
    }
}

void uart_sendcmd(unsigned char uart_num, unsigned char *bytes, unsigned char length)
{
    unsigned char i;
    switch(uart_num)
    {
        case 1:
            for (i = 0; i < length; i++)
            {
                SBUF = bytes[i];
                uart1_tx_busy = 1;
                while(uart1_tx_busy);
            }
            break;
        case 2:
            for (i = 0; i < length; i++)
            {
                S2BUF = bytes[i];
                uart2_tx_busy = 1;
                while(uart2_tx_busy);
            }
            break;
        case 3:
            for (i = 0; i < length; i++)
            {
                S3BUF = bytes[i];
                uart3_tx_busy = 1;
                while(uart3_tx_busy);
            }
            break;
        case 4:
            for (i = 0; i < length; i++)
            {
                S4BUF = bytes[i];
                uart4_tx_busy = 1;
                while(uart4_tx_busy);
            }
            break;
        default:
            break;
    }
}

void uart_sendbyte(unsigned char uart_num, unsigned char byte)
{
    switch(uart_num)
    {
        case 1:
            SBUF = byte;
            uart1_tx_busy = 1;
            while(uart1_tx_busy);
            break;
        case 2:
            S2BUF = byte;
            uart2_tx_busy = 1;
            while(uart2_tx_busy);
            break;
        case 3:
            S3BUF = byte;
            uart3_tx_busy = 1;
            while(uart3_tx_busy);
            break;
        case 4:
            S4BUF = byte;
            uart4_tx_busy = 1;
            while(uart4_tx_busy);
            break;
        default:
            break;
    }
}

void uart1_isr(void) interrupt 4
{
	if (TI)
	{
		TI = 0;
        uart1_tx_busy = 0;
	}
	if (RI)
	{
		RI = 0;
        uart1_rx_buffer[uart1_rx_counter] = SBUF;

        // ---------------- 放置专用串口中断处理代码函数 ------------------
        if((uart1_rx_buffer[uart1_rx_counter - 1] == 'A') && (uart1_rx_buffer[uart1_rx_counter] == 'T'))
        {
            memset(uart1_rx_buffer, '\0', sizeof(uart1_rx_buffer));
            uart1_rx_buffer[0] = 'A';
            uart1_rx_buffer[1] = 'T';
            uart1_rx_counter = 1;       // 重置计数器，从缓冲区头部开始写入剩余的命令
        }
        if((uart1_rx_buffer[uart1_rx_counter - 1] == '\r') && (uart1_rx_buffer[uart1_rx_counter] == '\n'))
        {
            host_comm_irqhandler();     // 当检测到\r\n代表命令接收完毕，调用host_comm_irqhandler()处理命令
        }
        // ------------------------ 专用代码结束 --------------------------
        if(++uart1_rx_counter >= UART1_BUF_LENGTH) uart1_rx_counter = 0;     // 缓冲区满, 循环
    }
}


void uart2_isr(void) interrupt 8                                            // 串口2的中断函数
{
	if (S2CON & 0x02)	                                                    // 检测串口2发送中断
	{
		S2CON &= ~0x02; 	                                                // 清除串口2发送中断请求位
        uart2_tx_busy = 0;                                                  // 发送空闲标志位
	}
	if (S2CON & 0x01)	                                                    // 检测串口2接收中断
	{
		S2CON &= ~0x01;	                                                    // 清除串口2接收中断请求位
        uart2_rx_buffer[uart2_rx_counter] = S2BUF;                          // 接收数据存入缓冲区        
        
        // ---------------- 放置专用串口中断处理代码函数 ------------------
        // laser_ranging_irqhandler('x');
        if(uart2_rx_buffer[uart2_rx_counter] == 0x80)
        {
            uart1_tx_buffer_from_uart2[0] = 0x91;
            uart1_tx_buffer_from_uart2[1] = uart2_rx_buffer[uart2_rx_counter];
            uart1_tx_buffer_from_uart2_index = 2;
        }
        uart1_tx_buffer_from_uart2[uart1_tx_buffer_from_uart2_index++] = S2BUF;
        // ------------------------ 专用代码结束 --------------------------

        if(++uart2_rx_counter >= UART2_BUF_LENGTH) uart2_rx_counter = 0;      // 缓冲区满, 循环
	}
}

void uart3_isr(void) interrupt 17                                           // 串口3的中断函数
{
	if (S3CON & 0x02)	                                                    // 检测串口3发送中断
	{
		S3CON &= ~0x02;	                                                    // 清除串口3发送中断请求位
        uart3_tx_busy = 0;                                                  // 发送空闲标志位
	}
	if (S3CON & 0x01)	                                                    // 检测串口3接收中断
	{
		S3CON &= ~0x01;	                                                    // 清除串口3接收中断请求位
        uart3_rx_buffer[uart3_rx_counter] = S3BUF;                          // 接收数据存入缓冲区

        // ---------------- 放置专用串口中断处理代码函数 ------------------
        // laser_ranging_irqhandler('y');
        if(uart3_rx_buffer[uart3_rx_counter] == 0x80)
        {
            uart1_tx_buffer_from_uart3[0] = 0x92;
            uart1_tx_buffer_from_uart3[1] = uart3_rx_buffer[uart3_rx_counter];
            uart1_tx_buffer_from_uart3_index = 2;
        }
        uart1_tx_buffer_from_uart3[uart1_tx_buffer_from_uart3_index++] = S3BUF;
        // ------------------------ 专用代码结束 --------------------------

        if(++uart3_rx_counter >= UART3_BUF_LENGTH) uart3_rx_counter = 0;    // 缓冲区满, 循环
	}
}

void uart4_isr(void) interrupt 18                                           // 串口4的中断函数
{
	if (S4CON & 0x02)	                                                    // 检测串口4发送中断
	{
		S4CON &= ~0x02;	                                                    // 清除串口4发送中断请求位
        uart4_tx_busy = 0;                                                  // 发送空闲标志位
	}
	if (S4CON & 0x01)	                                                    // 检测串口4接收中断
	{
		S4CON &= ~0x01;	                                                    // 清除串口4接收中断请求位
        uart4_rx_buffer[uart4_rx_counter] = S4BUF;                          // 接收数据存入缓冲区
        // ---------------- 放置专用串口中断处理代码函数 ------------------
        // uart1_tx_buffer_from_uart4 = S4BUF;
        if(uart4_rx_buffer[uart4_rx_counter] == 0x55)
        {
            memset(uart4_rx_buffer, '\0', sizeof(uart4_rx_buffer));
            uart4_rx_buffer[0] = 0x55;
            uart4_rx_counter = 0;       // 重置计数器，写入剩余的数据
        }
        if(uart4_rx_counter == 11)
        {
            led_1 = 1;
            wt61_data_parse(uart4_rx_buffer);
        }
        // ------------------------ 专用代码结束 --------------------------
        if(++uart4_rx_counter >= UART4_BUF_LENGTH) uart4_rx_counter = 0;      // 缓冲区满, 循环
	}
}

void uart_running(unsigned char uart_num)
{
    switch(uart_num)
    {
        case 1:
            if((uart1_tx_counter != uart1_rx_counter) && (!uart1_tx_busy))          // 收到数据, 发送空闲
            {
                SBUF = uart1_rx_buffer[uart1_tx_counter];                           // 发送数据
                uart1_tx_busy = 1;                                                  // 标记忙
                if(++uart1_tx_counter >= UART1_BUF_LENGTH) uart1_tx_counter = 0;    // 循环
            }
            break;
        case 2:
            if((uart2_tx_counter != uart2_rx_counter) && (!uart2_tx_busy))          // 收到数据, 发送空闲
            {
                S2BUF = uart2_rx_buffer[uart2_tx_counter];                          // 发送数据
                uart2_tx_busy = 1;                                                  // 标记忙
                if(++uart2_tx_counter >= UART2_BUF_LENGTH) uart2_tx_counter = 0;    // 循环
            }
            break;
        case 3:
            if((uart3_tx_counter != uart3_rx_counter) && (!uart3_tx_busy))          // 收到数据, 发送空闲
            {
                S3BUF = uart3_rx_buffer[uart3_tx_counter];                          // 发送数据
                uart3_tx_busy = 1;                                                  // 标记忙
                if(++uart3_tx_counter >= UART3_BUF_LENGTH) uart3_tx_counter = 0;    // 循环
            }
            break;
        case 4:
            if((uart4_tx_counter != uart4_rx_counter) && (!uart4_tx_busy))          // 收到数据, 发送空闲
            {
                S4BUF = uart4_rx_buffer[uart4_tx_counter];                          // 发送数据
                uart4_tx_busy = 1;                                                  // 标记忙
                if(++uart4_tx_counter >= UART4_BUF_LENGTH) uart4_tx_counter = 0;    // 循环
            }
            break;
        default:
            break;
    }
}

void uart_tx_send_buffer()
{
    if(uart1_tx_buffer_from_uart2_index >= 12)
    {
        uart_sendstring(1, uart1_tx_buffer_from_uart2);
        // uart1_tx_buffer_from_uart2 = [];
    }

    if(uart1_tx_buffer_from_uart3_index >= 12)
    {
        uart_sendstring(1, uart1_tx_buffer_from_uart3);
    }

    // if(uart1_tx_buffer_from_uart4)
    // {
    //     uart_sendbyte(1, uart1_tx_buffer_from_uart4);
    //     uart1_tx_buffer_from_uart4 = 0;
    // }
}
