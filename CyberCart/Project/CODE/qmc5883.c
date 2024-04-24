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
指令	                功能	                回复内容格式
AT	                    检测连接是否正常	    OK
AT+UART=0	            更改波特率为9600	    OK
AT+UART=1	            更改波特率为115200	    OK
AT+UART=2	            更改波特率为460800	    OK
AT+ID=?	                查询模块ID（0—254）    +ID=<ID>
AT+ID=<0-254的数字>	    更改设备地址	        OK
AT+INIT	                磁场感器初始化	        INIT SUCCES
AT+PRATE=0	            设置为单次回传模式	    OK Mag:<x轴，y轴，z轴磁场数据> Yaw:<z轴的角度>
AT+PRATE=<10-10000>	    设置回传速度单位ms	    OK Mag:<x轴，y轴，z轴磁场数据> Yaw:<z轴的角度>
AT+CALI=1	            开始磁场校准	        Calibrating
AT+CALI=0	            结束磁场校准	        Calibration completed
AT+CALI=2	            清除磁场零偏	        Reset mag offset param
AT+FILT=<0或1000>	    关闭滤波	            OK
AT+FILT=<1-999的数字>	设置滤波值	            OK
	                    错误指令	            ERROR
注：所有的AT指令以回车换行符结束（必须勾选“额外增加换行符”）		
*/

// 指令集
unsigned char qmccmd_check_connection[]             = {"AT"};
unsigned char qmccmd_set_uart_baudrate_9600[]       = {"AT+UART=0"};
unsigned char qmccmd_set_uart_baudrate_115200[]     = {"AT+UART=1"};
unsigned char qmccmd_set_uart_baudrate_460800[]     = {"AT+UART=2"};
unsigned char qmccmd_get_device_id[]                = {"AT+ID=?"};
unsigned char qmccmd_set_device_id[]                = {"AT+ID="};       // 需要补充参数
unsigned char qmccmd_init_sensor[]                  = {"AT+INIT"};
unsigned char qmccmd_set_single_measure_mode[]      = {"AT+PRATE=0"};
unsigned char qmccmd_set_measure_rate[]             = {"AT+PRATE="};    // 需要补充参数
unsigned char qmccmd_start_calibration[]            = {"AT+CALI=1"};
unsigned char qmccmd_stop_calibration[]             = {"AT+CALI=0"};
unsigned char qmccmd_clear_mag_offset[]             = {"AT+CALI=2"};
unsigned char qmccmd_disable_filter[]               = {"AT+FILT=0"};
unsigned char qmccmd_set_filter_value[]             = {"AT+FILT="};     // 需要补充参数


//-------------------------------------------------------------------------------------------------------------------
//  @brief      将字符串转换为整型(int)
//  @param      const char *str 字符串
//  @return     int 整型数
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example	int num = atoi("12345"); // num = 12345
//  *desc       该函数将字符串转换为整型数，用于将字符串数据转换为整型数据。
//              C251编译器没有atoi函数，因此需要自己实现。
//-------------------------------------------------------------------------------------------------------------------
int atoi(const char *str) 
{
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

    return sign * result;  // 返回带有正确符号的结果
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      将字符串转换为浮点型(double)
//  @param      const char *s 字符串
//  @return     double 浮点型数
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example	double num = atof("123.456"); // num = 123.456
//  *desc       该函数将字符串转换为浮点数。
//              C251编译器没有atof函数，因此需要自己实现。
//-------------------------------------------------------------------------------------------------------------------
double atof(const char *str) 
{
    double integerPart = 0;
    double decimalPart = 0;
    double decimalPlace = 1;
    int sign = 1;
    int i = 0;
    int state = 0; // 0代表整数部分，1代表小数部分

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
//  @brief      解析从串口发回的QMC5883数据
//  @param      unsigned char dat[] 串口收到的数据
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example    qmc5883_data_parse(qmc5883_data);
//  *desc       该函数解析从串口收到的QMC5883数据，并将其转换为相应的变量。
//              解析的数据保存在全局变量中，直接调用即可。
//-------------------------------------------------------------------------------------------------------------------
void qmc5883_data_parse(unsigned char dat[])
{
    int i = 0, j = 0;
    char counter = 0;

    for(i = 0; i < 64; i++)
    {
        if((dat[i] == ':') && (counter == 0))                   // 检测到第一个冒号
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // 从冒号之后的第一个字符开始写入
            {
                if(dat[j] == ',')
                {
                    qmc5883_char_magx[j - i - 1] = '\0';
                    qmc5883_magx = atoi(qmc5883_char_magx);     // 转换为整型
                    break;
                }
                qmc5883_char_magx[j - i - 1] = dat[j];          // 写入x轴磁场字符串
            }
        }
        else if((dat[i] == ':') && (counter == 1))              // 检测到第二个冒号
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // 从冒号之后的第一个字符开始写入
            {
                if(dat[j] == ',')                               // 遇到逗号结束
                {
                    qmc5883_char_magy[j - i - 1] = '\0';        // 字符串结束
                    qmc5883_magy = atoi(qmc5883_char_magy);     // 转换为整型
                    break;
                }
                qmc5883_char_magy[j - i - 1] = dat[j];          // 写入y轴磁场字符串
            }
        }
        else if((dat[i] == ':') && (counter == 2))              // 检测到第三个冒号
        {
            counter++;
            for(j = i + 1; j < 100; j++)                        // 从冒号之后的第一个字符开始写入
            {
                if(dat[j] == ',')                               // 遇到逗号结束
                {
                    qmc5883_char_magz[j - i - 1] = '\0';        // 字符串结束
                    qmc5883_magz = atoi(qmc5883_char_magz);     // 转换为整型
                    break;
                }
                qmc5883_char_magz[j - i - 1] = dat[j];          // 写入z轴磁场字符串
            }
        }
        else if((dat[i] == ':') && (counter == 3))              // 检测到第四个冒号
        {
            counter++;                                          // 第四个冒号之后的数据为角度
            for(j = i + 1; j < 100; j++)                        // 从冒号之后的第一个字符开始写入)
            {
                if(dat[j] == '\r')                              // 遇到回车符结束
                {
                    qmc5883_char_yaw[j - i - 1] = '\0';        // 字符串结束
                    qmc5883_yaw = atof(qmc5883_char_yaw);        // 转换为浮点型
                    break;
                }
                qmc5883_char_yaw[j - i - 1] = dat[j];           // 写入角度字符串
            }
        }
    }
}

void qmc5883_irqhandler(void)
{
    if((uart3_rx_buffer[uart3_rx_counter - 1] == 0x0D) && (uart3_rx_buffer[uart3_rx_counter] == 0x0A))
    {
        // 如果接收到'\r\n'字符代表一行数据接收完成，则将缓存全部保存并清空缓存区
        memcpy(qmc5883_data, uart3_rx_buffer, sizeof(uart3_rx_buffer));     // 保存数据
        uart3_rx_counter = 0;                                               // 清空缓存区
        qmc5883_data_parse(qmc5883_data);                                   // 解析数据
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