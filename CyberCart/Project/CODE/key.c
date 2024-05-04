
#include "key.h"
#include "hal.h"

unsigned char key1_up_store = 1; // ����1״̬�ݴ�
unsigned char key2_up_store = 1; // ����2״̬�ݴ�
unsigned char key3_up_store = 1; // ����3״̬�ݴ�
unsigned char key4_up_store = 1; // ����4״̬�ݴ�
unsigned char key5_up_store = 1; // ����5״̬�ݴ�

void keys_ui_init()
{
    // ��ʼ������
    oled_p6x8str_spi(0, 7, "[1][2][3][4][5]");
}

void key1_short_press()
{
    // �̰���1����
    led_1 = !led_1; // �л�
    stepper_adjust(+50);
    wheel_x_front_speed = 2000;
    // servo_claw_set_angle(0);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_1 = !led_1; // �л�
    // servo_claw_set_angle(30);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_1 = !led_1; // �л�
    // servo_claw_set_angle(60);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);    
    // led_1 = !led_1; // �л�
    // servo_claw_set_angle(90);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_1 = !led_1; // �л�
    // servo_claw_set_angle(120);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
}

void key2_short_press()
{
    // �̰���2����
    led_2 = !led_2; // �л�
    stepper_adjust(-50);
    // servo_pitch_set_angle(0);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_2 = !led_2; // �л�
    // servo_pitch_set_angle(45);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_2 = !led_2; // �л�
    // servo_pitch_set_angle(90);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);    
    // led_2 = !led_2; // �л�
    // servo_pitch_set_angle(135);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_2 = !led_2; // �л�
    // servo_pitch_set_angle(180);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // servo_pitch_set_angle(0);
}
#define PWM_CH_SERVO_PAN        PWMB_CH1_P00

void key3_short_press()
{
    // �̰���3����
    led_3 = !led_3; // �л�
    stepper_stp = 1;
    delay_us(10);
    stepper_stp = 0;
    // // servo_pan_set_angle(0);
    // pwm_duty(PWM_CH_SERVO_PAN, 250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_3 = !led_3; // �л�
    // // servo_pan_set_angle(90);
    // pwm_duty(PWM_CH_SERVO_PAN, 500);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_3 = !led_3; // �л�
    // // servo_pan_set_angle(180);
    // pwm_duty(PWM_CH_SERVO_PAN, 750);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);    
    // led_3 = !led_3; // �л�
    // // servo_pan_set_angle(270);
    // pwm_duty(PWM_CH_SERVO_PAN, 1000);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // led_3 = !led_3; // �л�
    // // servo_pan_set_angle(360);
    // pwm_duty(PWM_CH_SERVO_PAN, 1250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // delay_ms(250);
    // pwm_duty(PWM_CH_SERVO_PAN, 250);
    // // servo_pan_set_angle(0);
}

void key4_short_press()
{
    // �̰���4����
    led_4 = !led_4; // �л�
}

void key5_short_press()
{
    // �̰���5����
    pwm_duty(PWMB_CH1_P00, 0);
}

void key1_check() 
{
    if(key1 == 0 && key1_up_store == 1)
    {
        key1_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(0, 7, ">1<");
    }
    if(key1 == 1 && key1_up_store == 0)
    {
        key1_up_store = 1; // ��ǰ������ͷ�
        key1_short_press(); // �����̰���1����
        oled_p6x8str_spi(0, 7, "[1]");
    }
}

void key2_check()
{
    if(key2 == 0 && key2_up_store == 1)
    {
        key2_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(3*6, 7, ">2<");
    }
    if(key2 == 1 && key2_up_store == 0)
    {
        key2_up_store = 1; // ��ǰ������ͷ�
        key2_short_press(); // �����̰���2����
        oled_p6x8str_spi(3*6, 7, "[2]");
    }
}

void key3_check()
{
    if(key3 == 0 && key3_up_store == 1)
    {
        key3_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(6*6, 7, ">3<");
    }
    if(key3 == 1 && key3_up_store == 0)
    {
        key3_up_store = 1; // ��ǰ������ͷ�
        key3_short_press(); // �����̰���3����
        oled_p6x8str_spi(6*6, 7, "[3]");
    }
}

void key4_check()
{
    if(key4 == 0 && key4_up_store == 1)
    {
        key4_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(9*6, 7, ">4<");
    }
    if(key4 == 1 && key4_up_store == 0)
    {
        key4_up_store = 1; // ��ǰ������ͷ�
        key4_short_press(); // �����̰���4����
        oled_p6x8str_spi(9*6, 7, "[4]");
    }
}

void key5_check()
{
    if(key5 == 0 && key5_up_store == 1)
    {
        key5_up_store = 0; // ��ǰ���������
        oled_p6x8str_spi(12*6, 7, ">5<");
    }
    if(key5 == 1 && key5_up_store == 0)
    {
        key5_up_store = 1; // ��ǰ������ͷ�
        // �����̰���5����
        oled_p6x8str_spi(12*6, 7, "[5]");
    }
}