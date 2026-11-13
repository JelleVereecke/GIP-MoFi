/** main.c
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "VS1053b.h"
#include "mx_usart2.h"
#include "stdio.h"
#include "fat_sd_spi.h"
#include "stm32c5xx_hal.h"
#include "stm32c5xx_hal_spi.h"
#include "mx_spi1.h"
#include "stm32c5xx_hal_uart.h"
#include "sd.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

sdcard_t card;
vs1053b_handle_t player;
uint8_t buffer[32];

/* Serial Port */
int _write(int file, char *ptr, int len)
{
    // ST-approved way to retrieve the modern static handle pointer in CubeMX2
    hal_uart_handle_t *p_huart2 = mx_usart2_uart_gethandle(); 

    if (p_huart2 != NULL) {
        // Transmit your print data using the validated handle pointer
        HAL_UART_Transmit(p_huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    }
    return len;
}

int main(void)
{
  if (mx_system_init() != SYSTEM_OK)
    return (-1);

  SDCard(&card);

  HAL_Delay(1000);
  vs1053b_init(&player);
  vs1053b_output(&player);
  HAL_Delay(1000);

  // stream read test
  printf("ShadowAssasin.mp3 Reading...\n");
  unsigned long offset = 0;
  int eof = 0;

  f_open(&(card.file), "songs/ShadowAssasin.mp3", FA_READ);

  HAL_Delay(10);

  while (f_read(&(card.file), buffer, sizeof(buffer), &(card.br)) == FR_OK && card.br > 0) {
    vs1053b_sdi_send_buffer(buffer, card.br);
    vs1053b_await_dreq();
  }

  memset(&buffer, 0, sizeof(buffer));

  for(int i = 0; i < 2080/sizeof(buffer); i++) {
    vs1053b_sdi_send_buffer(buffer, card.br);
    vs1053b_await_dreq();
  }

  HAL_Delay(10);

  f_close(&(card.file));
  printf("\nFile played; EOF\n");

  while (1) {
    HAL_Delay(1000);
  }
} /* end main */