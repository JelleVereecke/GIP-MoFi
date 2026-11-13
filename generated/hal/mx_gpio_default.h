/**
  ******************************************************************************
  * @file           : mx_gpio_default.h
  * @brief          : Header for mx_gpio_default.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_GPIO_DEFAULT_H
#define MX_GPIO_DEFAULT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/******************************************************************************/
/* Exported defines for gpio_default in HAL layer                             */
/******************************************************************************/

/* Primary aliases for GPIO PA8 pin SDCS */
#define PA8_PORT                                        HAL_GPIOA
#define PA8_PIN                                         HAL_GPIO_PIN_8
#define PA8_INIT_STATE                                  HAL_GPIO_PIN_SET
#define PA8_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PA8_INACTIVE_STATE                              HAL_GPIO_PIN_RESET


/* Primary aliases for GPIO PB10 pin VSCS */
#define PB10_PORT                                        HAL_GPIOB
#define PB10_PIN                                         HAL_GPIO_PIN_10
#define PB10_INIT_STATE                                  HAL_GPIO_PIN_SET
#define PB10_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PB1O_INACTIVE_STATE                              HAL_GPIO_PIN_RESET

/* Primary aliases for GPIO PC6 pin VSDREQ */
#define PC6_PORT                                        HAL_GPIOC
#define PC6_PIN                                         HAL_GPIO_PIN_6
#define PC6_INIT_STATE                                  HAL_GPIO_PIN_SET
#define PC6_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PC6_INACTIVE_STATE                              HAL_GPIO_PIN_RESET

/* Primary aliases for GPIO PB4 pin VSRST */
#define PB4_PORT                                        HAL_GPIOB
#define PB4_PIN                                         HAL_GPIO_PIN_4
#define PB4_INIT_STATE                                  HAL_GPIO_PIN_SET
#define PB4_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PB4_INACTIVE_STATE                              HAL_GPIO_PIN_RESET


/* Primary aliases for GPIO PB5 pin VSDCS */
#define PC1_PORT                                        HAL_GPIOC
#define PC1_PIN                                         HAL_GPIO_PIN_1
#define PC1_INIT_STATE                                  HAL_GPIO_PIN_SET
#define PC1_ACTIVE_STATE                                HAL_GPIO_PIN_SET
#define PC1_INACTIVE_STATE                              HAL_GPIO_PIN_RESET

#define VSCS PB10_PORT, PB10_PIN
#define VSDCS PC1_PORT, PC1_PIN
#define VSRST PB4_PORT, PB4_PIN

#define VSDREQ PC6_PORT, PC6_PIN

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for gpio_default in HAL layer                           */
/******************************************************************************/
/**
  * @brief mx_gpio_default init function
  * This function configures the hardware resources used in this example
  * @retval 0  GPIO group correctly initialized
  * @retval -1 Issue detected during GPIO group initialization
  */
system_status_t mx_gpio_default_init(void);

/**
  * @brief  De-initialize gpio_default instance.
  */
system_status_t mx_gpio_default_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GPIO_DEFAULT_H */
