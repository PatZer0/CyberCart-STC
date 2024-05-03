#include "motor_driver_boards.h"
#include "qmc5883.h"

int     wheel_x_front_speed = 0, wheel_x_rear_speed = 0, wheel_y_front_speed = 0, wheel_y_rear_speed = 0;
float   wheel_target_yaw;
bit     wheel_yaw_calibrating_flag = 0;

// void motor_drivers_pwm_init()
// {
//     // 参数：模块_端口, 频率, 占空比(÷10000)
//     pwm_init(PWMA_CH1P_P60, 30000, 1000);   // X方向驱动板PB, 3KHz, 10%占空比
//     pwm_init(PWMA_CH2N_P63, 30000, 1000);   // X方向驱动板PA，3KHz, 10%占空比
//     pwm_init(PWMA_CH4N_P67, 30000, 1000);   // Y方向驱动板PB, 3KHz, 10%占空比
//     pwm_init(PWMA_CH3P_P14, 30000, 1000);   // Y方向驱动板PA, 3KHz, 10%占空比
// }

//-------------------------------------------------------------------------------------------------------------------
//  @brief      调整轮子方向
//  @param      <X/Y>_<FRONT/REAR/ALL>      指定轮子
//  @param      <FORWARD/BACKWARD/STOP>     指定方向
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.18
//  *example	wheel_direction(X_FRONT, FORWARD); // X方向前轮正转
//  *desc       无补充描述
//-------------------------------------------------------------------------------------------------------------------
void wheel_direction(WHEEL_SEL_enum wheel_select, WHEEL_DIR_enum wheel_direction)
{
    switch (wheel_select)
    {
        case X_FRONT:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_x_front_forward = 1;
                gpio_wheel_x_front_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 0;
            }
            break;
        case X_REAR:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_x_rear_forward = 1;
                gpio_wheel_x_rear_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 0;
            }
            break;
        case X_ALL:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_x_front_forward = 1;
                gpio_wheel_x_front_backward = 0;
                gpio_wheel_x_rear_forward = 1;
                gpio_wheel_x_rear_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 1;
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 0;
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 0;
            }
            break;
        case Y_FRONT:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_y_front_forward = 1;
                gpio_wheel_y_front_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 0;
            }
            break;
        case Y_REAR:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_y_rear_forward = 1;
                gpio_wheel_y_rear_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 0;
            }
            break;
        case Y_ALL:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_y_front_forward = 1;
                gpio_wheel_y_front_backward = 0;
                gpio_wheel_y_rear_forward = 1;
                gpio_wheel_y_rear_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 1;
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 0;
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 0;
            }
            break;
        case ALL:
            if (wheel_direction == FORWARD)
            {
                gpio_wheel_x_front_forward = 1;
                gpio_wheel_x_front_backward = 0;
                gpio_wheel_x_rear_forward = 1;
                gpio_wheel_x_rear_backward = 0;
                gpio_wheel_y_front_forward = 1;
                gpio_wheel_y_front_backward = 0;
                gpio_wheel_y_rear_forward = 1;
                gpio_wheel_y_rear_backward = 0;
            }
            else if (wheel_direction == BACKWARD)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 1;
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 1;
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 1;
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 1;
            }
            else if (wheel_direction == STOP)
            {
                gpio_wheel_x_front_forward = 0;
                gpio_wheel_x_front_backward = 0;
                gpio_wheel_x_rear_forward = 0;
                gpio_wheel_x_rear_backward = 0;
                gpio_wheel_y_front_forward = 0;
                gpio_wheel_y_front_backward = 0;
                gpio_wheel_y_rear_forward = 0;
                gpio_wheel_y_rear_backward = 0;
            }
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      调整电机转速和方向
//  @param      <X/Y>_<FRONT/REAR/ALL>      指定轮子
//  @param      speed                       转速值，范围负10000~正10000
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.18
//  *example	wheel_adjust(X_FRONT, 5000); // X方向前轮向前转, 转速50%
//  *desc       一般情况下，直接使用此函数即可，除非需要保持转速调整方向，否则不需要使用换向函数
//-------------------------------------------------------------------------------------------------------------------
void wheel_adjust(WHEEL_SEL_enum wheel_select, int speed)
{
    unsigned int pwm_duty_value;

    // 更改轮子方向
    if      (speed > 0) wheel_direction(wheel_select, FORWARD);
    else if (speed < 0) wheel_direction(wheel_select, BACKWARD);
    else                wheel_direction(wheel_select, STOP);

    // 确定PWM占空比
    if (speed >= 0) pwm_duty_value = speed;
    else            pwm_duty_value = -speed;

    // 校准速度输入量，限制在范围内
    if      (speed > WHEEL_PWM_MAX_VAL)     speed = WHEEL_PWM_MAX_VAL;
    else if (speed < -WHEEL_PWM_MAX_VAL)    speed = -WHEEL_PWM_MAX_VAL;

    switch (wheel_select)
    {
        case X_FRONT:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            break;

        case X_REAR:
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            break;

        case X_ALL:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            break;

        case Y_FRONT:
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            break;

        case Y_REAR:
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;

        case Y_ALL:
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;

        case ALL:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;
            
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      调整电机转速
//  @param      <X/Y>_<FRONT/REAR/ALL>      指定轮子
//  @param      speed                       转速值，范围0~10000
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.18
//  *example	wheel_speed(X_FRONT, 5000); // X方向前轮向前转, 转速50%
//  *desc       一般情况下，直接使用此函数即可，除非需要保持转速调整方向，否则不需要使用换向函数
//-------------------------------------------------------------------------------------------------------------------
void wheel_speed(WHEEL_SEL_enum wheel_select, unsigned int speed)
{
    unsigned int pwm_duty_value;
    if (speed > WHEEL_PWM_MAX_VAL) pwm_duty_value = WHEEL_PWM_MAX_VAL;

    switch (wheel_select)
    {
        case X_FRONT:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            break;
        case X_REAR:
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            break;
        case X_ALL:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            break;
        case Y_FRONT:
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            break;
        case Y_REAR:
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;
        case Y_ALL:
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;
        case ALL:
            pwm_duty(WHEEL_X_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_X_REAR_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_FRONT_PWMCH, pwm_duty_value);
            pwm_duty(WHEEL_Y_REAR_PWMCH, pwm_duty_value);
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据全局变量动态调整电机转速
//  @param      void
//  @since      v1.0 by PatZer0 on 2024.04.24
//  *example	void
//  *desc       使用四个全局变量，动态调整轮子转速，将此函数放在while(1)中运行，然后修改全局变量即可
//-------------------------------------------------------------------------------------------------------------------
void wheel_dynamic_adjusting()
{
    wheel_adjust(X_FRONT, wheel_x_front_speed);
    wheel_adjust(X_REAR, wheel_x_rear_speed);
    wheel_adjust(Y_FRONT, wheel_y_front_speed);
    wheel_adjust(Y_REAR, wheel_y_rear_speed);
}

void wheel_dynamic_calibrating()
{
    
}

void wheel_oled_init()
{
    oled_p6x8str_spi(0, 2, "XF");
    oled_p6x8str_spi(6*8, 2, "YF");
    oled_p6x8str_spi(0, 3, "XR");
    oled_p6x8str_spi(6*8, 3, "YR");
}

void wheel_oled_update()
{
    oled_printf_int32_spi(2*6, 2, wheel_x_front_speed, 4);
    oled_printf_int32_spi(2*6, 3, wheel_x_rear_speed, 4);
    oled_printf_int32_spi(10*6, 2, wheel_y_front_speed, 4);
    oled_printf_int32_spi(10*6, 3, wheel_y_rear_speed, 4);
}

#define WHEEL_YAW_CALIBRATING_THRESHOLD 0.5
#define WHEEL_TURNING_SPEED_FAST 5000
#define WHEEL_TURNING_SPEED_SLOW 1000

void wheel_yaw_calibrating()
{
    if(wheel_yaw_calibrating_flag)
    {
        // 向左转偏转角+，向右转偏转角-
        // 当前角度-偏转角度<0，说明偏转角度大于当前角度，需要左转
        if((qmc5883_yaw - wheel_target_yaw < 0) && (qmc5883_yaw - wheel_target_yaw > WHEEL_YAW_CALIBRATING_THRESHOLD))  
        {
            wheel_y_front_speed = WHEEL_TURNING_SPEED_FAST;         // 当还未达到目标角度时，加速转向
        }
        else if((qmc5883_yaw - wheel_target_yaw < 0) && (qmc5883_yaw - wheel_target_yaw < WHEEL_YAW_CALIBRATING_THRESHOLD))
        {
            wheel_y_front_speed = WHEEL_TURNING_SPEED_SLOW;        // 当达到目标角度时，停止转向
        }

        // 当前角度-偏转角度>0，说明偏转角度小于当前角度，需要右转
        if((qmc5883_yaw - wheel_target_yaw > 0) && (qmc5883_yaw - wheel_target_yaw < -WHEEL_YAW_CALIBRATING_THRESHOLD))
        {
            wheel_y_front_speed = -WHEEL_TURNING_SPEED_FAST;        // 当还未达到目标角度时，加速转向
        }
        else if((qmc5883_yaw - wheel_target_yaw > 0) && (qmc5883_yaw - wheel_target_yaw > -WHEEL_YAW_CALIBRATING_THRESHOLD))
        {
            wheel_y_front_speed = -WHEEL_TURNING_SPEED_SLOW;       // 当达到目标角度时，停止转向
        }

        // 角度误差小于阈值时，停止转向
        if(qmc5883_yaw - wheel_target_yaw < WHEEL_YAW_CALIBRATING_THRESHOLD)
        {
            wheel_yaw_calibrating_flag = 0;         // 标志位清零，停止校准
            wheel_y_front_speed = 0;
            wheel_y_rear_speed = 0;
        }
    }
}