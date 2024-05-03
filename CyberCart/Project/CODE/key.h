#ifndef __KEY_H__
#define __KEY_H__

#include "headfile.h"

sbit key5 = P4^4;
sbit key4 = P4^2;
sbit key3 = P4^3;
sbit key2 = P7^3;
sbit key1 = P4^1;

sbit led_1 = P4^5;
sbit led_2 = P7^7;
sbit led_3 = P7^6;
sbit led_4 = P7^5;

void keys_ui_init();
void key1_check();
void key2_check();
void key3_check();
void key4_check();
void key5_check();

#endif // __KEY_H__