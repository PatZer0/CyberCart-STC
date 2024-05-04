#ifndef STEPPER_H_
#define STEPPER_H_
#define STEPPER_STP P0^4
#define STEPPER_DIR P0^3
#define STEPPER_EN P0^5
#define PWM_STEPPER_FREQ 1000
#define PWM_STEPPER_PORT PWMB_CH4_P03
#define STEPPER_STEP_RATIO 4    // 每4个电平变化，会让步进电机走一步

sbit stepper_en = STEPPER_EN;
sbit stepper_dir = STEPPER_DIR;
sbit stepper_stp = STEPPER_STP;

void stepper_init(void);
void stepper_adjust(int step);
void stepper_oled_init();
void stepper_oled_update();

#endif /* STEPPER_H_ */