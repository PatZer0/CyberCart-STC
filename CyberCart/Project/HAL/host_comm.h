#ifndef HOST_COMM_H_
#define HOST_COMM_H_

void host_comm_uart_init(void);
void host_comm_irqhandler(void);
void host_comm_sender(void);
void host_comm_send_checksum_err(void);
void host_comm_send_data(const char *buffer);

#endif /* HOST_COMM_H_ */