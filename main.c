#include "stc32g.h"
#include "stc32_stc8_usb.h"

#define FOSC 24000000UL // ISP 下载时需将工作频率设置为24MHz

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";  // 不停电自动ISP 下载命令

void main() 
{
    WTST = 0;
    CKCON = 0;
    EAXFR = 1;

    P0M1 = 0x00;    P0M0 = 0x00;
    P1M1 = 0x00;    P1M0 = 0x00;
    P2M1 = 0x00;    P2M0 = 0x00;
    P3M1 = 0x00;    P3M0 = 0x00;
    P4M1 = 0x00;    P4M0 = 0x00;
    P5M1 = 0x00;    P5M0 = 0x00;
    P6M1 = 0x00;    P6M0 = 0x00;
    P7M1 = 0x00;    P7M0 = 0x00;

    P3M0 &= ~0x03;  // P3.0/P3.1和USB的D-/D+共用PIN脚
    P3M1 = 0x03;    // 需要将 P3.0/P3.1 设置为高阻输入模式

    IRC48MCR = 0x80;    // 使能内部48M 的USB专用IRC
    while (!(RC48MCR & 0x01));  
    USBCLK = 0x00;  // 设置USB 时钟源为内部48M 的USB 专用IRC
    USBCON = 0x90;  // 使能 USB 功能

    usb_init0();    // 调用 USB CDC 初始化库函数

    EUSB = 1;   // 使能 USB 中断

    EA = 1;

    while (DeviceState != DEVSTATE_CONFIGURED);     //等待 USB 完成配置

    while (1) 
    {
        if (bUsbOutReady)   // 当硬件接收完成上位机通过串口助手发送数据后
                            // 会自动将bUsbOutReady置1
                            // 接收的数据字节数保存在 OutNumber 变量中
                            // 接收的数据保存在 UsbOutBuffer 缓冲区
        {
            USB_SendData(UsbOutBuffer, OutNumber);  // 使用 USB_SendData 库函数可向上位机发送数据
                                                    // 这里的测试代码为将接收数据原样返回
                                                    // 处理完成接收的数据后
                                                    // 调用usb_OUT_done 准备接收下一笔数据
        }
    }
}
