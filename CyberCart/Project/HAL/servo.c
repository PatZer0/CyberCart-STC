#include "headfile.h"
#include "hal.h"
#define SERVO_PWM_FREQ          50 // ���PWMƵ��
#define PWM_CH_SERVO_PAN        PWMB_CH2_P01  
#define PWM_CH_SERVO_CLAW       PWMB_CH1_P00
#define PWM_CH_SERVO_PITCH      PWMB_CH3_P02
#define SERVO_PAN_MAX_ANGLE     360 // ��̨������Ƕ�
#define SERVO_CLAW_MAX_ANGLE    120 // ��еצ������Ƕ�
#define SERVO_PITCH_MAX_ANGLE   180 // ����������Ƕ�

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������Ƕȣ�������Ӧ�Ķ��ռ�ձȣ���еצ180������
//  @param      float angle ����Ƕ�
//  @return     unsigned int duty ������
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_claw_get_duty(90); // ����90���Ӧ��PWMռ�ձ�
//  *desc       ע�⣺��еצ���ת���Ƕ�̫��ᵼ�³�����ϣ��������Ƕ�Ϊ120�㡣
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_claw_get_duty(float angle)
{
    // 250/10000ռ�ձ�Ϊ0�㣬1250/10000ռ�ձ�Ϊ180�㣬�м��ռ�ձȶ�Ӧ�Ƕ�
    unsigned int duty;                                                      // ���ռ�ձ�
    if      (angle < 0)                     angle = 0;                      // ����ǶȲ���Ϊ����
    else if (angle > SERVO_CLAW_MAX_ANGLE)  angle = SERVO_CLAW_MAX_ANGLE;   // ����ǶȲ��ܳ������Ƕ�
    duty = 250 + ((1250 - 250) * angle) / 180;
    return duty;
} 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������Ƕȣ�������Ӧ�Ķ��ռ�ձȣ�����180������
//  @param      float angle ����Ƕ�
//  @return     unsigned int duty ������
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_pitch_get_duty(90); // ����90���Ӧ��PWMռ�ձ�
//  *desc       �����Ƕ�û�����ơ�
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_pitch_get_duty(float angle)
{
    // 250/10000ռ�ձ�Ϊ0�㣬1250/10000ռ�ձ�Ϊ180�㣬�м��ռ�ձȶ�Ӧ�Ƕ�
    unsigned int duty;                                                              // ���ռ�ձ�
    if      (angle < 0)                         angle = 0;                          // ����ǶȲ���Ϊ����
    else if (angle > SERVO_PITCH_MAX_ANGLE)     angle = SERVO_PITCH_MAX_ANGLE;      // ����ǶȲ��ܳ������Ƕ�
    duty = 250 + ((1250 - 250) * angle) / 180;
    return duty;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������Ƕȣ�������Ӧ�Ķ��ռ�ձȣ���̨360������
//  @param      float angle ����Ƕ�
//  @return     unsigned int duty ������
//  @since      v1.0 by PatZer0 on 2024.05.03
//  *example	servo_pan_get_duty(90); // ����90���Ӧ��PWMռ�ձ�
//  *desc       ���̽Ƕ�û�����ơ�
//-------------------------------------------------------------------------------------------------------------------
unsigned int servo_pan_get_duty(float angle)
{
    // 250/10000ռ�ձ�Ϊ0�㣬1250/10000ռ�ձ�Ϊ360�㣬�м��ռ�ձȶ�Ӧ�Ƕ�
    unsigned int duty;                                                              // ���ռ�ձ�
    if      (angle < 0)                         angle = 0;                          // ����ǶȲ���Ϊ����
    else if (angle > SERVO_PAN_MAX_ANGLE)       angle = SERVO_PAN_MAX_ANGLE;        // ����ǶȲ��ܳ������Ƕ�
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
