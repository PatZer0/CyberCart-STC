#include "headfile.h"
#include "hal.h"
#define SERVO_PWM_FREQ          50 // 舵机PWM频率
#define PWM_CH_SERVO_PAN        PWMB_CH2_P01  
#define PWM_CH_SERVO_CLAW       PWMB_CH1_P00
#define PWM_CH_SERVO_PITCH      PWMB_CH3_P02
#define SERVO_PAN_MAX_ANGLE     360 // 云台舵机最大角度
#define SERVO_CLAW_MAX_ANGLE    120 // 机械爪舵机最大角度
#define SERVO_PITCH_MAX_ANGLE   180 // 俯仰舵机最大角度

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据输入角度，给出对应的舵机占空比（机械爪180°舵机）
//  @param      float angle 输入角度
//  @return     unsigned int duty 整型数
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_claw_get_duty(90); // 返回90°对应的PWM占空比
//  *desc       注意：机械爪舵机转动角度太大会导致齿轮离合，限制最大角度为120°。
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_claw_get_duty(float angle)
{
    // 250/10000占空比为0°，1250/10000占空比为180°，中间的占空比对应角度
    unsigned int duty;                                                      // 舵机占空比
    if      (angle < 0)                     angle = 0;                      // 舵机角度不能为负数
    else if (angle > SERVO_CLAW_MAX_ANGLE)  angle = SERVO_CLAW_MAX_ANGLE;   // 舵机角度不能超过最大角度
    duty = 250 + ((1250 - 250) * angle) / 180;
    return duty;
} 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据输入角度，给出对应的舵机占空比（俯仰180°舵机）
//  @param      float angle 输入角度
//  @return     unsigned int duty 整型数
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_pitch_get_duty(90); // 返回90°对应的PWM占空比
//  *desc       俯仰角度没有限制。
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_pitch_get_duty(float angle)
{
    // 250/10000占空比为0°，1250/10000占空比为180°，中间的占空比对应角度
    unsigned int duty;                                                              // 舵机占空比
    if      (angle < 0)                         angle = 0;                          // 舵机角度不能为负数
    else if (angle > SERVO_PITCH_MAX_ANGLE)     angle = SERVO_PITCH_MAX_ANGLE;      // 舵机角度不能超过最大角度
    duty = 250 + ((1250 - 250) * angle) / 180;
    return duty;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据输入角度，给出对应的舵机占空比（云台360°舵机）
//  @param      float angle 输入角度
//  @return     unsigned int duty 整型数
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_pan_get_duty(90); // 返回90°对应的PWM占空比
//  *desc       底盘角度没有限制。
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_pan_get_duty(float angle)
{
    // 250/10000占空比为0°，1250/10000占空比为360°，中间的占空比对应角度
    unsigned int duty;                                                              // 舵机占空比
    if      (angle < 0)                         angle = 0;                          // 舵机角度不能为负数
    else if (angle > SERVO_PAN_MAX_ANGLE)       angle = SERVO_PAN_MAX_ANGLE;        // 舵机角度不能超过最大角度
    duty = 250 + (1000 * angle) / 360;
    return duty;
}

void servo_init()
{
    pwm_init(PWM_CH_SERVO_PAN,   50, 250);
    pwm_init(PWM_CH_SERVO_CLAW,  50, 250);
    pwm_init(PWM_CH_SERVO_PITCH, 50, 250);
}

void servo_claw_set_angle(float angle)
{
    unsigned int duty = servo_claw_get_duty(angle);
    pwm_duty(PWM_CH_SERVO_CLAW, duty);
}

void servo_pitch_set_angle(float angle)
{
    unsigned int duty = servo_pitch_get_duty(angle);
    pwm_duty(PWM_CH_SERVO_PITCH, duty);
}

void servo_pan_set_angle(float angle)
{
    unsigned int duty = servo_pitch_get_duty(angle);
    pwm_duty(PWM_CH_SERVO_PAN, duty);
}
