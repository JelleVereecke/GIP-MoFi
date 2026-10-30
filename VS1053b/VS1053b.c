#include "VS1053b.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_gpio.h"
#include "mx_gpio_default.h"
#include "mx_spi1.h"
#include "stm32c5xx_hal_spi.h"
#include <stdio.h>

void vs1053b_init(vs1053b_handle_t *handle) {

    HAL_GPIO_WritePin(VSRST, 1);
    HAL_GPIO_WritePin(VSDCS, 1);
    HAL_GPIO_WritePin(VSCS, 1);

    // reset
    printf("Resetting VS1053b..");
    HAL_GPIO_WritePin(VSDCS, 0);
    HAL_GPIO_WritePin(VSCS, 0);
    HAL_Delay(500);
    printf("Reset complete...");

    HAL_GPIO_WritePin(VSDCS, 1);
    HAL_GPIO_WritePin(VSCS, 1);
    HAL_Delay(500);

    HAL_SPI_SetBaudRatePrescaler(mx_spi1_gethandle(), HAL_SPI_BAUD_RATE_PRESCALER_128);
    HAL_Delay(20);

    vs1053b_write_reg(0x5, 44101); // 44.1kHz stereo
    vs1053b_write_reg(0x3, 6<<12); // Normal clock settings multiplier 3.0 = 12.2 MHz; or smth

    HAL_SPI_SetBaudRatePrescaler(mx_spi1_gethandle(), HAL_SPI_BAUD_RATE_PRESCALER_4);

    vs1053b_write_reg(0x0, 1<<14 | 1<<11); // SM_SDINEW | SM_LINE1
    HAL_Delay(10);
    
    vs1053b_await_dreq();
    
    handle->endFillByte = vs1053b_wram_read(0x1E06) & 0xFF;

    HAL_Delay(100);
}

void vs1053b_soft_rst() {
    vs1053b_write_reg(0x0, 1<<11 | 1<<2);
    HAL_Delay(10);
    
    vs1053b_await_dreq();
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void vs1053b_output(vs1053b_handle_t *handle) {
    sdi_send_fillers(handle, 10);
    vs1053b_wram_write(0xc017, 3);
    vs1053b_wram_write(0xc019, 0);
    HAL_Delay(100);
    vs1053b_soft_rst();

    uint16_t value = (map(100, 0, 100, 0xFE, 0x00) << 8) | map(100, 0, 100, 0xFE, 0x00);

    vs1053b_write_reg(0xB, value);
}

void vs1053b_ctrl(int on) {
    HAL_GPIO_WritePin(VSCS, on?0:1);
    HAL_GPIO_WritePin(VSDCS, 1);
}

void vs1053b_data(int on) {
    HAL_GPIO_WritePin(VSCS, 1);
    HAL_GPIO_WritePin(VSDCS, on?0:1);
}

void vs1053b_write_reg(uint8_t reg, uint16_t value) {
    vs1053b_ctrl(1);
    uint8_t code = 2;
    HAL_SPI_Transmit(mx_spi1_gethandle(), &code, sizeof(uint8_t), HAL_MAX_DELAY);
    HAL_SPI_Transmit(mx_spi1_gethandle(), &reg, sizeof(uint8_t), HAL_MAX_DELAY);
    HAL_SPI_Transmit(mx_spi1_gethandle(), &value, sizeof(uint16_t), HAL_MAX_DELAY);

    vs1053b_await_dreq();
    
    vs1053b_ctrl(0);
}

uint16_t vs1053b_read_reg(uint8_t reg) {
    uint16_t res;

    vs1053b_ctrl(1);
    uint8_t code = 3;
    HAL_SPI_Transmit(mx_spi1_gethandle(), &code, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(mx_spi1_gethandle(), &reg, 1, HAL_MAX_DELAY);

    HAL_SPI_Receive(mx_spi1_gethandle(), &res, 2, HAL_MAX_DELAY);

    vs1053b_await_dreq();

    vs1053b_ctrl(0);
    return res;
}

uint16_t vs1053b_wram_read(uint16_t addr) {
    vs1053b_write_reg(0x7, addr);
    return vs1053b_read_reg(0x6);
}

uint16_t vs1053b_wram_write(uint16_t addr, uint16_t data) {
    vs1053b_write_reg(0x7, addr);
    vs1053b_write_reg(0x6, data);
}

void sdi_send_fillers(vs1053b_handle_t *handle , size_t len) {
    size_t chunk_length;

    vs1053b_data(1);
    while (len) {
        vs1053b_await_dreq();

        chunk_length = len;
        if (len > 512) {
            chunk_length = 512;
        }
        len -= chunk_length;
        while(chunk_length--) {
            HAL_SPI_Transmit(mx_spi1_gethandle(), &(handle->endFillByte), sizeof(uint8_t), HAL_MAX_DELAY);
        }
    }
    vs1053b_data(0);
}

void vs1053b_sdi_send_buffer(uint8_t *data, size_t len) {
    size_t chunk_length;

    vs1053b_data(1);
    while(len) {
        printf("Buffer remaining: %i\n", len);
        vs1053b_await_dreq();

        chunk_length = len;
        if (len > 512) {
            chunk_length = 512;
        }
        len -= chunk_length;
        HAL_SPI_Transmit(mx_spi1_gethandle(), data, chunk_length, HAL_MAX_DELAY);
        data += chunk_length;
    }
    vs1053b_data(0);
}

void vs1053b_await_dreq() {
    while(!HAL_GPIO_ReadPin(VSDREQ)) {
        HAL_Delay(1);
    }
}