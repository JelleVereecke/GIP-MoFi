#ifndef VS1053_H
#define VS1053_H

#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_gpio.h"
#include "stm32c5xx_hal_spi.h"
#include "mx_gpio_default.h"
#include "mx_spi1.h"
#include <stdint.h>

typedef struct vs1053b_handle_s {
    uint8_t endFillByte;
} vs1053b_handle_t;

// test, remove later
void vs1053b_dump_regs(void);
// end test

void vs1053b_init(vs1053b_handle_t *handle);

void vs1053b_soft_rst();

void vs1053b_output(vs1053b_handle_t *handle);

void vs1053b_ctrl(int on);

void vs1053b_data(int on);

void vs1053b_write_reg(uint8_t reg, uint16_t value);

uint16_t vs1053b_read_reg(uint8_t reg);

uint16_t vs1053b_wram_read(uint16_t addr);

void vs1053b_wram_write(uint16_t addr, uint16_t data);

void sdi_send_fillers(vs1053b_handle_t *handle , size_t len);

void vs1053b_sdi_send_buffer(uint8_t *data, size_t len);

void vs1053b_await_dreq();

#endif