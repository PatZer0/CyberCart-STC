#ifndef __UART_H__
#define __UART_H__

#define UART1_BUF_LENGTH 16                        // ������ջ���������
#define UART2_BUF_LENGTH 16                        // ������ջ���������
#define UART3_BUF_LENGTH 16                        // ������ջ���������
#define UART4_BUF_LENGTH 16                        // ������ջ���������

extern unsigned char uart1_tx_counter, uart2_tx_counter, uart3_tx_counter, uart4_tx_counter;   // ���ͼ���
extern unsigned char uart1_rx_counter, uart2_rx_counter, uart3_rx_counter, uart4_rx_counter;   // ���ռ���
extern bit           uart1_tx_busy, uart2_tx_busy, uart3_tx_busy, uart4_tx_busy;               // ����æ��־
extern unsigned char uart1_rx_buffer[UART1_BUF_LENGTH];                                        // ���ջ���
extern unsigned char uart2_rx_buffer[UART2_BUF_LENGTH];                                        // ���ջ���
extern unsigned char uart3_rx_buffer[UART3_BUF_LENGTH];                                        // ���ջ���
extern unsigned char uart4_rx_buffer[UART4_BUF_LENGTH];                                        // ���ջ���


void uart_port_init(void);
void uart_initialize(unsigned char uart_num);
void uart_sendstring(unsigned char uart_num, unsigned char *puts);
void uart_sendcmd(unsigned char uart_num, unsigned char *bytes, unsigned char length);
void uart_running(unsigned char uart_num);

#endif