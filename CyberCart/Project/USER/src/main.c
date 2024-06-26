#include "headfile.h"
#include "hal.h"
#include "ui.h"
#include "key.h"

void gpio_init()
{
    P0M0 = 0xff; P0M1 = 0x00; // 配置P0为推挽输出
    P1M0 = 0xff; P1M1 = 0x00; 
    P6M0 = 0xff; P6M1 = 0x00; 
}

void motor_drivers_pwm_init()
{
    // 参数：模块_端口, 频率, 占空比(÷10000)
    pwm_init(PWMA_CH1P_P60, 30000, 0);   // X方向驱动板PB, 30KHz, 10%占空比
    pwm_init(PWMA_CH2N_P63, 30000, 0);   // X方向驱动板PA，30KHz, 10%占空比
    pwm_init(PWMA_CH4N_P67, 30000, 0);   // Y方向驱动板PB, 30KHz, 10%占空比
    pwm_init(PWMA_CH3P_P14, 30000, 0);   // Y方向驱动板PA, 30KHz, 10%占空比
    wheel_adjust(X_ALL, 0);
    wheel_adjust(Y_ALL, 0);
}

void main()
{   
    led_1 = 0;
    led_2 = 0;
    led_3 = 0;
    led_4 = 0;

	board_init();			        // 初始化寄存器,勿删除此句代码。

    // motor_drivers_pwm_init();	    // 初始化PWM
    // gpio_init();                    // 初始化GPIO
    // oled_init_spi();                // 初始化OLED显示屏
    // wheel_oled_init();              // 初始化速度显示OLED显示屏
    // ui_init();                      // 初始化UI

    // delay_ms(100);
    // laser_ranging_init();           // 初始化测距模块
    host_comm_uart_init();          // 初始化主机通信UART

    // servo_init();                   // 初始化舵机
    stepper_init();
    // stepper_oled_init();            // 初始化步进电机OLED显示屏

    while(1)
    {
        key1_check();
        key2_check();
        key3_check();
        key4_check();
        key5_check();
        // stepper_oled_update();
        host_comm_sender();
        // wheel_dynamic_adjusting();
        uart_tx_send_buffer();
        // wheel_oled_update();

        stepper_adjust(+200);
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        stepper_adjust(-200);
    }
}
