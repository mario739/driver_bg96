/**
 * @file string_management.h
 * @author Mario Aguilar Montoya (fernandoaguilar731010@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef STRING_MANAGEMENT
#define STRING_MANAGEMENT

#include "driver_bg96.h"

em_bg96_error_handling send_data(char *cmd,char*expect,char* resp,uint32_t timeout);
#endif