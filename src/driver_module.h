/**
 * @file driver_module.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DRIVER_MODULE
#define DRIVER_MODULE

#include <stdint.h>
#include <string.h>

typedef enum
{
  FT_BG96_OK=0,
  FT_BG96_ERROR,
  FT_BG96_BUSY,
  FT_BG96_TIMEOUT,
} em_bg96_error_handling;


/*HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
*/
typedef em_bg96_error_handling(*uart_transmit_it)(char *,uint16_t);
typedef em_bg96_error_handling(*uart_receive_it )(char *,uint16_t); 
typedef void (*delay1ms_t)(uint16_t);

typedef struct 
{
    uart_transmit_it bg96_uart_transmit_it;
    uart_receive_it  bg96_uart_receive_it;
    delay1ms_t       bg96_delay_ms;    
}t_device_config;

em_bg96_error_handling bg96_init(t_device_config *obj,uart_transmit_it ft_uart_transmit_port,uart_receive_it ft_uart_receive_port,delay1ms_t delay_port );

em_bg96_error_handling bg96_sendcmdsingle(t_device_config *obj,char *cmd,char *resp_expect,char *buffer,uint16_t timeout);


#endif