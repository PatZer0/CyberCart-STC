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


// 电机驱动板1：X方向前轮、后轮
sbit gpio_wheel_x_front_forward     = P6^1;     // X 方向前轮顺时针转动信号
sbit gpio_wheel_x_front_backward    = P6^2;     // X 方向前轮逆时针转动信号
sbit gpio_wheel_x_rear_forward      = P6^4;     // X 方向后轮顺时针转动信号
sbit gpio_wheel_x_rear_backward     = P6^5;     // X 方向后轮逆时针转动信号

// 电机驱动板2：Y方向前轮、后轮
sbit gpio_wheel_y_front_forward     = P1^0;     // Y 方向前轮顺时针转动信号
sbit gpio_wheel_y_front_backward    = P1^1;     // Y 方向前轮逆时针转动信号
sbit gpio_wheel_y_rear_forward      = P1^6;     // Y 方向后轮顺时针转动信号
sbit gpio_wheel_y_rear_backward     = P1^5;     // Y 方向后轮逆时针转动信号

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


// void motor_drivers_pwm_init();                                                   // 初始化电机驱动板
void wheel_adjust(WHEEL_SEL_enum wheel_select, int speed);                          // 同时设置电机转动速度和方向
void wheel_direction(WHEEL_SEL_enum wheel_select, WHEEL_DIR_enum wheel_direction);  // 设置电机转动方向
void wheel_speed(WHEEL_SEL_enum wheel_select, unsigned int speed);                  // 设置电机转动速度
void wheel_dynamic_adjusting();                                                     // 动态调整电机转动速度和方向
void wheel_dynamic_calibrating();                                                   // 动态校准电机转动速度和方向
void wheel_yaw_calibrating();                                                       // 校准电机转动方向

#endif // MOTOR_DRIVER_BOARDS_H
