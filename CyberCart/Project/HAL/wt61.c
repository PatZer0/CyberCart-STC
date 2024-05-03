#include "headfile.h"
#include "hal.h"

#define g 9.8 // 重力加速度

unsigned char wt61_cmd_set_axis_z_zero    [] = {0xFF, 0xAA, 0x52};
unsigned char wt61_cmd_calibrate_acc      [] = {0xFF, 0xAA, 0x67};
unsigned char wt61_cmd_switch_power       [] = {0xFF, 0xAA, 0x60};
unsigned char wt61_cmd_set_uart_mode      [] = {0xFF, 0xAA, 0x61};
unsigned char wt61_cmd_set_iic_mode       [] = {0xFF, 0xAA, 0x62};
unsigned char wt61_cmd_set_uart_115200    [] = {0xFF, 0xAA, 0x63};
unsigned char wt61_cmd_set_uart_9600      [] = {0xFF, 0xAA, 0x64};
unsigned char wt61_cmd_set_band_265Hz     [] = {0xFF, 0xAA, 0x81};
unsigned char wt61_cmd_set_band_188Hz     [] = {0xFF, 0xAA, 0x82};
unsigned char wt61_cmd_set_band_98Hz      [] = {0xFF, 0xAA, 0x83};
unsigned char wt61_cmd_set_band_42Hz      [] = {0xFF, 0xAA, 0x84};
unsigned char wt61_cmd_set_band_20Hz      [] = {0xFF, 0xAA, 0x85};
unsigned char wt61_cmd_set_band_10Hz      [] = {0xFF, 0xAA, 0x86};
unsigned char wt61_cmd_set_band_5Hz       [] = {0xFF, 0xAA, 0x87};
unsigned char wt61_cmd_set_horizontal     [] = {0xFF, 0xAA, 0x65};
unsigned char wt61_cmd_set_vertical       [] = {0xFF, 0xAA, 0x66};

bit wt61_oled_disp_flag = 1;

float acc_x, acc_y, acc_z, temperature;
float gyro_x, gyro_y, gyro_z;
float x_roll, y_pitch, z_yaw;

void wt61_init(void)
{
    uart_init(UART_4, UART4_RX_P52, UART4_TX_P53, 115200, TIM_4);
    delay_ms(100);
    uart_sendcmd(4, wt61_cmd_calibrate_acc, 3);
    delay_ms(100);
    uart_sendcmd(4, wt61_cmd_set_axis_z_zero, 3);
    delay_ms(100);
}

void wt61_data_parse(const char *buf)
{
    if(buf[0] == 0x55)
    {
        switch (buf[1])
        {
            case 0x51: // 加速度数据解析
                acc_x = ((short)(((short)buf[3] << 8) | buf[2])) * 16 * g / 32768 ;
                acc_y = ((short)(((short)buf[5] << 8) | buf[4])) * 16 * g / 32768 ;
                acc_z = ((short)(((short)buf[7] << 8) | buf[6])) * 16 * g / 32768 ;
                temperature = ((buf[9] << 8) | buf[8]) / 32768 * 96.38 +36.53;
               if(wt61_oled_disp_flag) oled_printf_float_spi(2*6, 2, x_roll, 3, 2);
                break;
            case 0x52: // 陀螺仪数据解析
                gyro_x = ((short)(((short)buf[3] << 8) | buf[2])) * 2000 / 32768 ;
                gyro_y = ((short)(((short)buf[5] << 8) | buf[4])) * 2000 / 32768 ;
                gyro_z = ((short)(((short)buf[7] << 8) | buf[6])) * 2000 / 32768 ;
               if(wt61_oled_disp_flag) oled_printf_float_spi(2*6, 3, y_pitch, 3, 2);
                break;
            case 0x53: // 角度数据解析
                x_roll  = ((short)(((short)buf[3] << 8) | buf[2])) * 180 / 32768 ;
                y_pitch = ((short)(((short)buf[5] << 8) | buf[4])) * 180 / 32768 ;
                z_yaw   = ((short)(((short)buf[7] << 8) | buf[6])) * 180 / 32768 ;
               if(wt61_oled_disp_flag) oled_printf_float_spi(2*6, 4, z_yaw, 3, 2);
                break;
            default:
                break;
        }
    }
}

void wt61_oled_disp(void)
{
}

void wt61_oled_init()
{
    oled_p6x8str_spi(0, 2, "X:");
    oled_p6x8str_spi(0, 3, "Y:");
    oled_p6x8str_spi(0, 4, "Z:");
}