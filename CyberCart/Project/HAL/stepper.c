#include "headfile.h"
#include "hal.h"


sbit stepper_en = STEPPER_EN;

void stepper_init(void)
{
    stepper_en = 1;
    pwm_init(PWM_STEPPER_PORT, PWM_STEPPER_FREQ, 0);
}

// void stepper_adjust(int step)
// {

// }