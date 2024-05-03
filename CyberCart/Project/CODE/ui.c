#include "headfile.h"
#include "ui.h"

unsigned char ui_loading_index = 0;

void ui_init()
{
    oled_p6x8str_spi(0, 0, "CYBERCART");
    oled_p6x8str_spi(0, 1, "DEBUG");
    oled_p6x8str_spi(6*18, 0, "VER");
    oled_p6x8str_spi(6*18, 1, "1.0");
    
    // oled_p6x8str_spi(0, 2, "DISTX:");
    // oled_p6x8str_spi(0, 3, "DISTY:");
    // oled_p6x8str_spi(0, 4, "ANGLE:");

    ui_keys_init();                 // 初始化按键UI
}

void ui_keys_init()
{
    // 初始化按键UI
    oled_p6x8str_spi(0, 7, "[1][2][3][4]");
}

void ui_running()
{
    ui_loading_index++;
    if(ui_loading_index > 3) ui_loading_index = 0;
    switch (ui_loading_index)
    {
    case 0:
        oled_p6x8str_spi(20*6, 7, '1');
        break;
    case 1:
        oled_p6x8str_spi(20*6, 7, '2');
        break;
    case 2:
        oled_p6x8str_spi(20*6, 7, '3');
        break;
    case 3:
        oled_p6x8str_spi(20*6, 7, '4');
        break;
    default:
        break;
    }
}