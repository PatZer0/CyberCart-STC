#ifndef WT61_H_
#define WT61_H_

extern bit wt61_oled_disp_flag;

extern float acc_x, acc_y, acc_z, temperature;
extern float gyro_x, gyro_y, gyro_z;
extern float x_roll, y_pitch, z_yaw;

void wt61_data_parse(const char *buf);
void wt61_init();
void wt61_oled_init();
void wt61_oled_disp();

#endif /* WT61_H_ */