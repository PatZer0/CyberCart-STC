#include "qmc5883.h"

char qmc5883_data[];


        // // 接收Magx数据：
        // if(uart3_rx_buffer[0] == 'M' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'g' && uart3_rx_buffer[3] == 'x' && uart3_rx_buffer[4] == ':')
        // {
        //     if(uart3_rx_buffer[uart3_rx_counter] != ',')
        //     {
        //         qmc5883_char_magx[uart3_rx_counter - 5] = uart3_rx_buffer[uart3_rx_counter];
        //     }
        //     else
        //     {
        //         uart3_rx_counter = 0;
        //     }
        // }
        // // 接收Magy数据：
        // if(uart3_rx_buffer[0] == 'M' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'g' && uart3_rx_buffer[3] == 'y' && uart3_rx_buffer[4] == ':')
        // {
        //     if(uart3_rx_buffer[uart3_rx_counter] != ',')
        //     {
        //         qmc5883_char_magy[uart3_rx_counter - 5] = uart3_rx_buffer[uart3_rx_counter];
        //     }
        //     else
        //     {
        //         uart3_rx_counter = 0;
        //     }
        // }
        // // 接收Magz数据：
        // if(uart3_rx_buffer[0] == 'M' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'g' && uart3_rx_buffer[3] == 'z' && uart3_rx_buffer[4] == ':')
        // {
        //     if(uart3_rx_buffer[uart3_rx_counter] != ',')
        //     {
        //         qmc5883_char_magz[uart3_rx_counter - 5] = uart3_rx_buffer[uart3_rx_counter];
        //     }
        //     else
        //     {
        //         uart3_rx_counter = 0;
        //     }
        // }
        // // 接收Yaw数据：
        // if(uart3_rx_buffer[0] == 'Y' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'w' && uart3_rx_buffer[3] == ':' && uart3_rx_buffer[4] == '1')
        // {
        //     if(uart3_rx_buffer[uart3_rx_counter] != 'M')
        //     {
        //         qmc5883_char_yaw[uart3_rx_counter - 4] = uart3_rx_buffer[uart3_rx_counter];
        //     }
        //     else
        //     {
        //         uart3_rx_counter = 0;
        //     }
        // }

        // // 接收完毕，开始解析数据：


        // // // 接收数据示例：Magx:2180,Magy:-1285,Magz:-3412,Yaw:120.51
        // // // 对数据进行处理：

        // // // 检查数据头部是否为"Magx:"
        // // if(uart3_rx_buffer[0] == 'M' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'g' && uart3_rx_buffer[3] == 'x' && uart3_rx_buffer[4] == ':')
        // // {
        // //     // 找到数据头部，开始解析数据
        // //     // 遍历数据中的每个字符，记第一个":"出现的位置+1为qmc5883_char_x_start，记第一个","出现的位置-1为qmc5883_char_x_end；
        // //                     // 同理，记第二个":"出现的位置+1为qmc5883_char_y_start，记第二个","出现的位置-1为qmc5883_char_y_end；
        // //                     // 同理，记第三个":"出现的位置+1为qmc5883_char_z_start，记第三个","出现的位置-1为qmc5883_char_z_end；
        // //                     // 同理，记第四个":"出现的位置+1为qmc5883_char_yaw_start，记buffer最后一位的位置为qmc5883_char_yaw_end;
        // //     for(qmc5883_char_counter = 0; qmc5883_char_counter < UART3_BUF_LENGTH; qmc5883_char_counter++)
        // //     {
        // //         if(uart3_rx_buffer[qmc5883_char_counter] == ':')
        // //         {
        // //             if(qmc5883_char_x_start != 0)
        // //             {
        // //                 qmc5883_char_x_start = qmc5883_char_counter + 1;
        // //             }
        // //             else if(qmc5883_char_y_start != 0)
        // //             {
        // //                 qmc5883_char_y_start = qmc5883_char_counter + 1;
        // //             }
        // //             else if(qmc5883_char_z_start != 0)
        // //             {
        // //                 qmc5883_char_z_start = qmc5883_char_counter + 1;
        // //             }
        // //             else if(qmc5883_char_yaw_start != 0)
        // //             {
        // //                 qmc5883_char_yaw_start = qmc5883_char_counter + 1;
        // //             }
        // //         }
        // //         else if(uart3_rx_buffer[qmc5883_char_counter] == ',')
        // //         {
        // //             if(qmc5883_char_x_end != 0)
        // //             {
        // //                 qmc5883_char_x_end = qmc5883_char_counter - 1;
        // //             }
        // //             else if(qmc5883_char_y_end != 0)
        // //             {
        // //                 qmc5883_char_y_end = qmc5883_char_counter - 1;
        // //             }
        // //             else if(qmc5883_char_z_end != 0)
        // //             {
        // //                 qmc5883_char_z_end = qmc5883_char_counter - 1;
        // //             }
        // //         }
        // //         else
        // //         {
        // //             qmc5883_char_yaw_end = qmc5883_char_counter;
        // //         }
        // //     }
        // //     // 头尾位置检测结束，将数据保存到变量中
        // //     for (qmc5883_char_counter = qmc5883_char_x_start; qmc5883_char_counter <= qmc5883_char_x_end; qmc5883_char_counter++)
        // //     {
        // //         qmc5883_char_magx[qmc5883_char_counter - qmc5883_char_x_start] = uart3_rx_buffer[qmc5883_char_counter];
        // //     }
        // //     for (qmc5883_char_counter = qmc5883_char_y_start; qmc5883_char_counter <= qmc5883_char_y_end; qmc5883_char_counter++)
        // //     {
        // //         qmc5883_char_magy[qmc5883_char_counter - qmc5883_char_y_start] = uart3_rx_buffer[qmc5883_char_counter];
        // //     }
        // //     for (qmc5883_char_counter = qmc5883_char_z_start; qmc5883_char_counter <= qmc5883_char_z_end; qmc5883_char_counter++)
        // //     {
        // //         qmc5883_char_magz[qmc5883_char_counter - qmc5883_char_z_start] = uart3_rx_buffer[qmc5883_char_counter];
        // //     }
        // //     for (qmc5883_char_counter = qmc5883_char_yaw_start; qmc5883_char_counter <= qmc5883_char_yaw_end; qmc5883_char_counter++)
        // //     {
        // //         qmc5883_char_yaw[qmc5883_char_counter - qmc5883_char_yaw_start] = uart3_rx_buffer[qmc5883_char_counter];
        // //     }
        // //     // 数据保存完毕，清空缓冲区、计数器

        // //     uart3_rx_counter = 0;
        // //     uart3_rx_buffer[0] = 0;
        // // }
        // // else
        // // {
        // //     // 如果没有检测到数据开始，清空缓冲区和计数器
        // //     uart3_rx_counter = 0;
        // //     uart3_rx_buffer[0] = 0;
        // //     qmc5883_char_x_start = 0;
        // //     qmc5883_char_y_start = 0;
        // //     qmc5883_char_z_start = 0;
        // //     qmc5883_char_yaw_start = 0;
        // //     qmc5883_char_x_end = 0;
        // //     qmc5883_char_y_end = 0;
        // //     qmc5883_char_z_end = 0;
        // //     qmc5883_char_yaw_end = 0;
        // //     qmc5883_char_counter = 0;
        // // }