#include "VS1053b.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_gpio.h"
#include "mx_gpio_default.h"
#include "mx_spi1.h"
#include "stm32c5xx_hal_spi.h"
#include <stdio.h>

void vs1053b_init(vs1053b_handle_t *handle) {

    

    // reset
    printf("Resetting VS1053b..");
    HAL_GPIO_WritePin(VSRST, 0);
    HAL_GPIO_WritePin(VSRST, 1);
    HAL_GPIO_WritePin(VSDCS, 1);
    HAL_GPIO_WritePin(VSCS, 1);
    HAL_Delay(2);
    HAL_GPIO_WritePin(VSRST, 1);
    printf("Reset complete...");

    HAL_SPI_SetBaudRatePrescaler(mx_spi1_gethandle(), HAL_SPI_BAUD_RATE_PRESCALER_256);
    HAL_Delay(20);

    vs1053b_write_reg(0x0, 0x0804); // SM_SDINEW | SM_LINE1

    //vs1053b_write_reg(0x5, 44101); // 44.1kHz stereo
    vs1053b_write_reg(0x3, 0x6000); // Normal clock settings multiplier 3.0 = 12.2 MHz; or smth

    HAL_SPI_SetBaudRatePrescaler(mx_spi1_gethandle(), HAL_SPI_BAUD_RATE_PRESCALER_32);
    
    handle->endFillByte = vs1053b_wram_read(0x1E06) & 0xFF;
}

void vs1053b_soft_rst() {
    vs1053b_write_reg(0x0, 1<<11 | 1<<2);
    HAL_Delay(10);
    
    vs1053b_await_dreq();
}

void vs1053b_output(vs1053b_handle_t *handle) {
    vs1053b_write_reg(0xB, 0x2020);
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
    value = value<<8 | (value>>8); // 0x1234 -> 0x3412 (byte order) WAT AL DE HELE TIJD HET PROBLEEM WAS FDTYGUKHILMGULFYCTKVJHB
    HAL_SPI_Transmit(mx_spi1_gethandle(), &code, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(mx_spi1_gethandle(), &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(mx_spi1_gethandle(), &value, 2, HAL_MAX_DELAY);
    vs1053b_await_dreq();

    vs1053b_ctrl(0);
}

uint16_t vs1053b_read_reg(uint8_t reg) {
    uint16_t res;

    vs1053b_ctrl(1);
    uint8_t code = 3;
    uint8_t write[2] = {code, reg};
    HAL_SPI_Transmit(mx_spi1_gethandle(), &write, 2, HAL_MAX_DELAY);
    HAL_SPI_Receive(mx_spi1_gethandle(), &res, 2, HAL_MAX_DELAY);
    vs1053b_await_dreq();

    vs1053b_ctrl(0);
    return res;
}

uint16_t vs1053b_wram_read(uint16_t addr) {
    vs1053b_write_reg(0x7, addr);
    return vs1053b_read_reg(0x6);
}

void vs1053b_wram_write(uint16_t addr, uint16_t data) {
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
    vs1053b_data(1);
    while (len) {
        vs1053b_await_dreq();
        size_t chunk = (len > 32) ? 32 : len;
        HAL_SPI_Transmit(mx_spi1_gethandle(), data, chunk, HAL_MAX_DELAY);
        data += chunk;
        len  -= chunk;
    }
    vs1053b_data(0);
}

void vs1053b_await_dreq() {
    while(HAL_GPIO_ReadPin(VSDREQ) == HAL_GPIO_PIN_RESET) { // had het omgekeerd gedaan (dreq moet hoog zijn om verder te gaan)
        HAL_Delay(0);
    }
}