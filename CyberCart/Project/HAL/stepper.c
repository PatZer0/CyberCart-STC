#include "headfile.h"
#include "hal.h"
#include "STDLIB.H"

#define STEPPER_FAST_CNT 10
#define STEPPER_FAST_THRES 50

unsigned int    timer0_counter              = 0;
unsigned int    stepper_pulses_remaining    = 0;
unsigned char   stepper_pulses_counter      = 0;
unsigned int    stepper_steps_remaining     = 0;
unsigned int    stepper_steps_setted        = 0;

void Timer0_Isr(void) interrupt 1
{
    unsigned int stepper_timer_period = 10;
    int stepper_steps_diff = stepper_steps_setted - stepper_steps_remaining;
    timer0_counter++;

    // 停止前减速
    if (stepper_steps_remaining <= STEPPER_FAST_THRES)
    {
        stepper_timer_period = (STEPPER_FAST_CNT + STEPPER_FAST_THRES) - stepper_steps_remaining;
    }
    // 启动后加速
    else if (stepper_steps_diff < STEPPER_FAST_THRES)
    {
        stepper_timer_period = (STEPPER_FAST_CNT + STEPPER_FAST_THRES) - stepper_steps_diff;
    }
    // 正常速度
    else
    {
        stepper_timer_period = STEPPER_FAST_CNT;
    }

    if(timer0_counter >= stepper_timer_period)
    {
        if(stepper_pulses_remaining > 0)
        {
            stepper_stp = !stepper_stp;
            stepper_pulses_remaining--;
            stepper_pulses_counter++;
        }
        if(stepper_pulses_counter >= 4)
        {
            stepper_pulses_counter = 0;
            stepper_steps_remaining--;
        }
        timer0_counter = 0;
    }
}

void Timer0_Init(void)		//0.1毫秒=100微秒@33.1776MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x0A;				//设置定时初始值
	TH0 = 0xF3;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

void stepper_init(void)
{
    stepper_en = 1;
    Timer0_Init();
    // pwm_init(PWM_STEPPER_PORT, 1000, 8000);
}

void stepper_oled_init()
{
    oled_p6x8str_spi(0, 4, "STP");
}

void stepper_oled_update()
{
    oled_printf_int32_spi(4*6, 4, stepper_steps_remaining, 6);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      让步进电机走指定的步数
//  @param      int 带符号整型数
//  @return     void
//  @since      v1.0 by PatZer0 on 2024.04.15
//  *example	stepper_adjust(+100); // 让步进电机向上走100步
//-------------------------------------------------------------------------------------------------------------------
void stepper_adjust(int step)
{
    if(step > 0)    stepper_dir = 1;
    else            stepper_dir = 0;

    stepper_pulses_remaining = abs(step) * 4;
    stepper_steps_remaining = abs(step);
    stepper_steps_setted = abs(step);
}