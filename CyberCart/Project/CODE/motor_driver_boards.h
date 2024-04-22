#ifndef MOTOR_DRIVER_BOARDS_H
#define MOTOR_DRIVER_BOARDS_H

#include "headfile.h"

#define WHEEL_X_FRONT_PWMCH PWMA_CH1P_P60
#define WHEEL_X_REAR_PWMCH  PWMA_CH2N_P63
#define WHEEL_Y_FRONT_PWMCH PWMA_CH3P_P14
#define WHEEL_Y_REAR_PWMCH  PWMA_CH4N_P67

// ���������1��X����ǰ�֡�����
sbit gpio_wheel_x_front_forward     = P6^1;     // X ����ǰ��˳ʱ��ת���ź�
sbit gpio_wheel_x_front_backward    = P6^2;     // X ����ǰ����ʱ��ת���ź�
sbit gpio_wheel_x_rear_forward      = P6^4;     // X �������˳ʱ��ת���ź�
sbit gpio_wheel_x_rear_backward     = P6^5;     // X ���������ʱ��ת���ź�

// ���������2��Y����ǰ�֡�����
sbit gpio_wheel_y_front_forward     = P1^0;     // Y ����ǰ��˳ʱ��ת���ź�
sbit gpio_wheel_y_front_backward    = P1^1;     // Y ����ǰ����ʱ��ת���ź�
sbit gpio_wheel_y_rear_forward      = P1^6;     // Y �������˳ʱ��ת���ź�
sbit gpio_wheel_y_rear_backward     = P1^5;     // Y ���������ʱ��ת���ź�

typedef enum
{
    X_FRONT,
    X_REAR,
    X_ALL,
    Y_FRONT,
    Y_REAR,
    Y_ALL,
    ALL,
}WHEEL_SEL_enum;

typedef enum
{
    FORWARD,
    BACKWARD,
    STOP,
}WHEEL_DIR_enum;

// ��ʼ�����������
// void motor_drivers_pwm_init();                                                      // ��ʼ�����������
void wheel_adjust(WHEEL_SEL_enum wheel_select, int speed);                          // ͬʱ���õ��ת���ٶȺͷ���
void wheel_direction(WHEEL_SEL_enum wheel_select, WHEEL_DIR_enum wheel_direction);  // ���õ��ת������
void wheel_speed(WHEEL_SEL_enum wheel_select, unsigned int speed);                  // ���õ��ת���ٶ�
#endif // MOTOR_DRIVER_BOARDS_H
