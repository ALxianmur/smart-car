/*
 * e2prom.c
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#include "e2prom.h"

e2prom_data fd;

void e2prom_init_data(e2prom_data *fd, uint8 num, void *addr, uint32 size)
{
    fd->sector_num = num;
    fd->data_addr = (void *)addr;
    fd->data_size = size;
    delay_ms(5);

    e2prom_load_data(fd);
    if ((uint32)smartcar_param.bend_steer_kp == 0xffffffff)
    {
        e2prom_reset_data();
    }
}

void e2prom_reset_data(void)
{
    SMARTCAR_ResetConfig();
}

void e2prom_load_data(e2prom_data *fd)
{
    iap_read_bytes(0x00, (uint8 *)(fd->data_addr), (int16)fd->data_size);
}

void e2prom_save_data(e2prom_data *fd)
{
    extern_iap_write_bytes(0x00, fd->data_addr, (int16)fd->data_size);
    e2prom_load_data(fd);
}

// 方法一
uint8 Is_Little_endian(void)
{
    uint16 i = 1;
    return (*(uint8 *)&i);
}

// 方法二
// uint8 Is_Little_endian(void)
//{
//     union
//     {
//         uint16  big;
//         uint8   lit;
//     } check;
//     check.big = 1;
//     return check.lit;
// }
//

// 方法三
// static union
//{
//     uint8 a[2];
//     uint16 i;
// }endian = {1,0};
// #define Is_Little_endian() ((uint8)endian.i)

void test_endian(void)
{

    if (Is_Little_endian())
    {
        printf("It's Little-endian\r\n");
    }
    else
    {
        printf("It's Big-endian\r\n");
    }
}