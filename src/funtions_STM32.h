/**
 * @file functions_STM32.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef FUNTIONS_STM32
#define FUNTIONS_STM32

#include "driver_module.h"


/*HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);*/
//em_bg96_error_handling uart_transmit_it_STM32(char* p_data,uint16_t size);
//em_bg96_error_handling uart_receive_it_STM32(char* p_data,uint16_t size);
void delay_STM32(uint16_t delay);


#endif