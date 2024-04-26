#ifndef MOTOR_DRIVER_BOARDS_H
#define MOTOR_DRIVER_BOARDS_H

#include "headfile.h"

#define WHEEL_X_FRONT_PWMCH PWMA_CH1P_P60
#define WHEEL_X_REAR_PWMCH  PWMA_CH2N_P63
#define WHEEL_Y_FRONT_PWMCH PWMA_CH3P_P14
#define WHEEL_Y_REAR_PWMCH  PWMA_CH4N_P67

#define WHEEL_PWM_MAX_VAL 9800

extern int    wheel_x_front_speed, wheel_x_rear_speed, wheel_y_front_speed, wheel_y_rear_speed;
extern float  wheel_target_yaw;
extern bit    wheel_yaw_calibrating_flag;


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


// void motor_drivers_pwm_init();                                                   // ��ʼ�����������
void wheel_adjust(WHEEL_SEL_enum wheel_select, int speed);                          // ͬʱ���õ��ת���ٶȺͷ���
void wheel_direction(WHEEL_SEL_enum wheel_select, WHEEL_DIR_enum wheel_direction);  // ���õ��ת������
void wheel_speed(WHEEL_SEL_enum wheel_select, unsigned int speed);                  // ���õ��ת���ٶ�
void wheel_dynamic_adjusting();                                                     // ��̬�������ת���ٶȺͷ���
void wheel_dynamic_calibrating();                                                   // ��̬У׼���ת���ٶȺͷ���
void wheel_yaw_calibrating();                                                       // У׼���ת������

#endif // MOTOR_DRIVER_BOARDS_H
