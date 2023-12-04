/*
 * e2prom.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _E2PROM_H
#define _E2PROM_H

#include "headfile.h"

typedef struct
{
    uint8 sector_num;
    uint8 *data_addr;
    uint32 data_size;
} e2prom_data;

extern e2prom_data fd;

void e2prom_init_data(e2prom_data *fd, uint8 num, void *addr, uint32 size);
void e2prom_reset_data(void);
void e2prom_load_data(e2prom_data *fd);
void e2prom_save_data(e2prom_data *fd);

void test_endian(void);

#endif