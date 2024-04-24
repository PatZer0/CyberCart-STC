#include "headfile.h"
#include "qmc5883.h"
#include "uart.h"
#define FILTER_SIZE 60

char             qmc5883_data[64];
unsigned char    qmc5883_cmd_buf[32];
int              qmc5883_magx, qmc5883_magy, qmc5883_magz;
float            qmc5883_yaw;
char             qmc5883_char_magx[10], qmc5883_char_magy[10], qmc5883_char_magz[10], qmc5883_char_yaw[10];

/*
ָ��	                ����	                �ظ����ݸ�ʽ
AT	                    ��������Ƿ�����	    OK
AT+UART=0	            ���Ĳ�����Ϊ9600	    OK
AT+UART=1	            ���Ĳ�����Ϊ115200	    OK
AT+UART=2	            ���Ĳ�����Ϊ460800	    OK
AT+ID=?	                ��ѯģ��ID��0��254��    +ID=<ID>
AT+ID=<0-254������>	    �����豸��ַ	        OK
AT+INIT	                �ų�������ʼ��	        INIT SUCCES
AT+PRATE=0	            ����Ϊ���λش�ģʽ	    OK Mag:<x�ᣬy�ᣬz��ų�����> Yaw:<z��ĽǶ�>
AT+PRATE=<10-10000>	    ���ûش��ٶȵ�λms	    OK Mag:<x�ᣬy�ᣬz��ų�����> Yaw:<z��ĽǶ�>
AT+CALI=1	            ��ʼ�ų�У׼	        Calibrating
AT+CALI=0	            �����ų�У׼	        Calibration completed
AT+CALI=2	            ����ų���ƫ	        Reset mag offset param
AT+FILT=<0��1000>	    �ر��˲�	            OK
AT+FILT=<1-999������>	�����˲�ֵ	            OK
	                    ����ָ��	            ERROR
ע�����е�ATָ���Իس����з����������빴ѡ���������ӻ��з�����		
*/

// ָ�
unsigned char qmccmd_check_connection[]             = {"AT"};
unsigned char qmccmd_set_uart_baudrate_9600[]       = {"AT+UART=0"};
unsigned char qmccmd_set_uart_baudrate_115200[]     = {"AT+UART=1"};
unsigned char qmccmd_set_uart_baudrate_460800[]     = {"AT+UART=2"};
unsigned char qmccmd_get_device_id[]                = {"AT+ID=?"};
unsigned char qmccmd_set_device_id[]                = {"AT+ID="};       // ��Ҫ�������
unsigned char qmccmd_init_sensor[]                  = {"AT+INIT"};
unsigned char qmccmd_set_single_measure_mode[]      = {"AT+PRATE=0"};
unsigned char qmccmd_set_measure_rate[]             = {"AT+PRATE="};    // ��Ҫ�������
unsigned char qmccmd_start_calibration[]            = {"AT+CALI=1"};
unsigned char qmccmd_stop_calibration[]             = {"AT+CALI=0"};
unsigned char qmccmd_clear_mag_offset[]             = {"AT+CALI=2"};
unsigned char qmccmd_disable_filter[]               = {"AT+FILT=0"};
unsigned char qmccmd_set_filter_value[]             = {"AT+FILT="};     // ��Ҫ�������


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ַ���ת��Ϊ����(int)
//  @param      const char *str �ַ���
//  @return     int ������
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example	int num = atoi("12345"); // num = 12345
//  *desc       �ú������ַ���ת��Ϊ�����������ڽ��ַ�������ת��Ϊ�������ݡ�
//              C251������û��atoi�����������Ҫ�Լ�ʵ�֡�
//-------------------------------------------------------------------------------------------------------------------
int atoi(const char *str) 
{
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

    return sign * result;  // ���ش�����ȷ���ŵĽ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ַ���ת��Ϊ������(double)
//  @param      const char *s �ַ���
//  @return     double ��������
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example	double num = atof("123.456"); // num = 123.456
//  *desc       �ú������ַ���ת��Ϊ��������
//              C251������û��atof�����������Ҫ�Լ�ʵ�֡�
//-------------------------------------------------------------------------------------------------------------------
double atof(const char *str) 
{
    double integerPart = 0;
    double decimalPart = 0;
    double decimalPlace = 1;
    int sign = 1;
    int i = 0;
    int state = 0; // 0�����������֣�1����С������

    while (str[i] == ' ') {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    for (; str[i]; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            if (state == 0) {
                integerPart = integerPart * 10 + (str[i] - '0');
            } else {
                decimalPart = decimalPart * 10 + (str[i] - '0');
                decimalPlace *= 10;
            }
        } else if (str[i] == '.' && state == 0) {
            state = 1;
        } else {
            break;
        }
    }

    return sign * (integerPart + decimalPart / decimalPlace);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Ӵ��ڷ��ص�QMC5883����
//  @param      unsigned char dat[] �����յ�������
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example    qmc5883_data_parse(qmc5883_data);
//  *desc       �ú��������Ӵ����յ���QMC5883���ݣ�������ת��Ϊ��Ӧ�ı�����
//              ���������ݱ�����ȫ�ֱ����У�ֱ�ӵ��ü��ɡ�
//-------------------------------------------------------------------------------------------------------------------
void qmc5883_data_parse(unsigned char dat[])
{
    int i = 0, j = 0;
    char counter = 0;

    for(i = 0; i < 64; i++)
    {
        if((dat[i] == ':') && (counter == 0))                   // ��⵽��һ��ð��
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // ��ð��֮��ĵ�һ���ַ���ʼд��
            {
                if(dat[j] == ',')
                {
                    qmc5883_char_magx[j - i - 1] = '\0';
                    qmc5883_magx = atoi(qmc5883_char_magx);     // ת��Ϊ����
                    break;
                }
                qmc5883_char_magx[j - i - 1] = dat[j];          // д��x��ų��ַ���
            }
        }
        else if((dat[i] == ':') && (counter == 1))              // ��⵽�ڶ���ð��
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // ��ð��֮��ĵ�һ���ַ���ʼд��
            {
                if(dat[j] == ',')                               // �������Ž���
                {
                    qmc5883_char_magy[j - i - 1] = '\0';        // �ַ�������
                    qmc5883_magy = atoi(qmc5883_char_magy);     // ת��Ϊ����
                    break;
                }
                qmc5883_char_magy[j - i - 1] = dat[j];          // д��y��ų��ַ���
            }
        }
        else if((dat[i] == ':') && (counter == 2))              // ��⵽������ð��
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // ��ð��֮��ĵ�һ���ַ���ʼд��
            {
                if(dat[j] == ',')                               // �������Ž���
                {
                    qmc5883_char_magz[j - i - 1] = '\0';        // �ַ�������
                    qmc5883_magz = atoi(qmc5883_char_magz);     // ת��Ϊ����
                    break;
                }
                qmc5883_char_magz[j - i - 1] = dat[j];          // д��z��ų��ַ���
            }
        }
        else if((dat[i] == ':') && (counter == 3))              // ��⵽���ĸ�ð��
        {
            counter++;                                          // ���ĸ�ð��֮�������Ϊ�Ƕ�
            for(j = i + 1; j < 100; j++)                        // ��ð��֮��ĵ�һ���ַ���ʼд��)
            {
                if(dat[j] == '\r')                              // �����س�������
                {
                    qmc5883_char_yaw[j - i - 1] = '\0';        // �ַ�������
                    qmc5883_yaw = atof(qmc5883_char_yaw);        // ת��Ϊ������
                    break;
                }
                qmc5883_char_yaw[j - i - 1] = dat[j];           // д��Ƕ��ַ���
            }
        }
    }
}

void qmc5883_irqhandler(void)
{
    if((uart3_rx_buffer[uart3_rx_counter - 1] == 0x0D) && (uart3_rx_buffer[uart3_rx_counter] == 0x0A))
    {
        // ������յ�'\r\n'�ַ�����һ�����ݽ�����ɣ��򽫻���ȫ�����沢��ջ�����
        memcpy(qmc5883_data, uart3_rx_buffer, sizeof(uart3_rx_buffer));     // ��������
        uart3_rx_counter = 0;                                               // ��ջ�����
        qmc5883_data_parse(qmc5883_data);                                   // ��������
        // oled_p6x8str_spi(5*6, 2, qmc5883_char_magx);
        // oled_p6x8str_spi(5*6, 3, qmc5883_char_magy);
        // oled_p6x8str_spi(5*6, 4, qmc5883_char_magz);
        // oled_p6x8str_spi(4*6, 5, qmc5883_char_yaw);
        // oled_printf_int32_spi(5*6, 2, qmc5883_magx, 10);
        // oled_printf_int32_spi(5*6, 3, qmc5883_magy, 10);
        // oled_printf_int32_spi(5*6, 4, qmc5883_magz, 10);
        oled_printf_float_spi(4*6, 5, qmc5883_yaw, 4, 2);
        uart3_rx_rdy = 1;
    }
}