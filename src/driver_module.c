/**
 * @file driver_module.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include"driver_module.h"


em_bg96_error_handling bg96_init(t_device_config *obj,uart_transmit_it ft_uart_transmit_port,uart_receive_it ft_uart_receive_port,delay1ms_t delay_port )
{
    obj->bg96_uart_transmit_it=ft_uart_transmit_port;
    obj->bg96_uart_receive_it=ft_uart_receive_port;
    obj->bg96_delay_ms=delay_port;
}

em_bg96_error_handling bg96_sendcmdsingle(t_device_config *obj,char *cmd,char *resp_expect,char *buffer,uint16_t timeout)
{
    em_bg96_error_handling resp=FT_BG96_ERROR;
    memset(buffer,0,sizeof(buffer));  
    resp=obj->bg96_uart_transmit_it(cmd,strlen(cmd));
    if (FT_BG96_OK==resp)
    {
        resp=obj->bg96_uart_receive_it(buffer,timeout);
    }
    return resp;
}