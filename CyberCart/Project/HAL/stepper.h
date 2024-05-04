#ifndef STEPPER_H_
#define STEPPER_H_
#define STEPPER_STP P0^3
#define STEPPER_DIR P0^4
#define STEPPER_EN P0^5
#define PWM_STEPPER_FREQ 1000
#define PWM_STEPPER_PORT PWMB_CH4_P03

void stepper_init(void);
void stepper_adjust(int step);

#endif /* STEPPER_H_ */