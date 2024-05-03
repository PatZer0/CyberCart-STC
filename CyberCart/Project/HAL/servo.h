#ifndef SERVO_H_
#define SERVO_H_

void servo_init();
void servo_pan_set_angle(float angle);
void servo_claw_set_angle(float angle);
void servo_pitch_set_angle(float angle);

#endif /* SERVO_H_ */