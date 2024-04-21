#ifndef __QMC5883_H__
#define __QMC5883_H__

#include "headfile.h"

extern char             qmc5883_data[64];
extern unsigned char    qmc5883_cmd_buf[32];
extern int              qmc5883_magx, qmc5883_magy, qmc5883_magz;
extern float            qmc5883_yaw;
extern char             qmc5883_char_magx[10], qmc5883_char_magy[10], qmc5883_char_magz[10], qmc5883_char_yaw[10];

void qmc5883_data_parse(unsigned char dat[]);
void qmc5883_irqhandler(void);

#endif //__QMC5883_H__