#include "qmc5883.h"

char qmc5883_data[];


        // // ����Magx���ݣ�
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
        // // ����Magy���ݣ�
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
        // // ����Magz���ݣ�
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
        // // ����Yaw���ݣ�
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

        // // ������ϣ���ʼ�������ݣ�


        // // // ��������ʾ����Magx:2180,Magy:-1285,Magz:-3412,Yaw:120.51
        // // // �����ݽ��д���

        // // // �������ͷ���Ƿ�Ϊ"Magx:"
        // // if(uart3_rx_buffer[0] == 'M' && uart3_rx_buffer[1] == 'a' && uart3_rx_buffer[2] == 'g' && uart3_rx_buffer[3] == 'x' && uart3_rx_buffer[4] == ':')
        // // {
        // //     // �ҵ�����ͷ������ʼ��������
        // //     // ���������е�ÿ���ַ����ǵ�һ��":"���ֵ�λ��+1Ϊqmc5883_char_x_start���ǵ�һ��","���ֵ�λ��-1Ϊqmc5883_char_x_end��
        // //                     // ͬ���ǵڶ���":"���ֵ�λ��+1Ϊqmc5883_char_y_start���ǵڶ���","���ֵ�λ��-1Ϊqmc5883_char_y_end��
        // //                     // ͬ���ǵ�����":"���ֵ�λ��+1Ϊqmc5883_char_z_start���ǵ�����","���ֵ�λ��-1Ϊqmc5883_char_z_end��
        // //                     // ͬ���ǵ��ĸ�":"���ֵ�λ��+1Ϊqmc5883_char_yaw_start����buffer���һλ��λ��Ϊqmc5883_char_yaw_end;
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
        // //     // ͷβλ�ü������������ݱ��浽������
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
        // //     // ���ݱ�����ϣ���ջ�������������

        // //     uart3_rx_counter = 0;
        // //     uart3_rx_buffer[0] = 0;
        // // }
        // // else
        // // {
        // //     // ���û�м�⵽���ݿ�ʼ����ջ������ͼ�����
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