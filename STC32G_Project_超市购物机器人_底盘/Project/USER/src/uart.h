#ifndef __UART_H__
#define __UART_H__

#define UART1_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define UART2_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define UART3_BUF_LENGTH 16                        // 定义接收缓冲区长度
#define UART4_BUF_LENGTH 16                        // 定义接收缓冲区长度

extern unsigned char uart1_tx_counter, uart2_tx_counter, uart3_tx_counter, uart4_tx_counter;   // 发送计数
extern unsigned char uart1_rx_counter, uart2_rx_counter, uart3_rx_counter, uart4_rx_counter;   // 接收计数
extern bit           uart1_tx_busy, uart2_tx_busy, uart3_tx_busy, uart4_tx_busy;               // 发送忙标志
extern unsigned char uart1_rx_buffer[UART1_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char uart2_rx_buffer[UART2_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char uart3_rx_buffer[UART3_BUF_LENGTH];                                        // 接收缓冲
extern unsigned char uart4_rx_buffer[UART4_BUF_LENGTH];                                        // 接收缓冲


void uart_port_init(void);
void uart_initialize(unsigned char uart_num);
void uart_sendstring(unsigned char uart_num, unsigned char *puts);
void uart_sendcmd(unsigned char uart_num, unsigned char *bytes, unsigned char length);
void uart_running(unsigned char uart_num);

#endif