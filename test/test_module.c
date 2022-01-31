/**
 * @file test_module.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "unity.h"
#include "driver_module.h"
#include "mock_funtions_STM32.h"


em_bg96_error_handling uart_receive_it_STM32(char* buffer,uint16_t timeout)
{   
    return FT_BG96_ERROR;
    /**buffer="ERROR\r\n";
    for (size_t i = 0; i < timeout; i++)
        {
            if (NULL!=strstr(buffer,"OK\r\n"))
            {
                return FT_BG96_OK;
            }
            else if (NULL!=strstr(buffer,"ERROR\r\n"))
            {
                return FT_BG96_ERROR;
            }    
                return FT_BG96_TIMEOUT;       
        } */

}
em_bg96_error_handling uart_transmit_it_STM32(char* p_data,uint16_t size){
    return FT_BG96_OK;
}

t_device_config bg96_config={    .bg96_uart_transmit_it=NULL,
                               .bg96_uart_receive_it=NULL,
                               .bg96_delay_ms=NULL};
void test_init_driver(void)
{
    bg96_init(&bg96_config,uart_transmit_it_STM32,uart_receive_it_STM32,delay_STM32);                   
    TEST_ASSERT_EQUAL(bg96_config.bg96_uart_transmit_it,uart_transmit_it_STM32);
    TEST_ASSERT_EQUAL(bg96_config.bg96_uart_receive_it,uart_receive_it_STM32);
    TEST_ASSERT_EQUAL(bg96_config.bg96_delay_ms,delay_STM32);
}


void test_send_cmd(void)
{
    
    char *cmd="hola";
    char *resp_expect="OK\r\n";
    char buffer[256];
    TEST_ASSERT_EQUAL(FT_BG96_ERROR,bg96_sendcmdsingle(&bg96_config,cmd,resp_expect,buffer,300));
}