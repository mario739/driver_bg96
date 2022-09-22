/**
 * @file string_management.h
 * @author your name (you@domain.com)
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
void turn_on_bg96(void);
void parse_string(char *data,char firts_caracter,char second_caracter,char *buffer);

#endif