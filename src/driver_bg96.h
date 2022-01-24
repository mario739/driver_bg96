/**
 * @file driver_bg96.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DRIVER_BG96
#define DRIVER_BG96

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "send_commands.h"

#define CMD_RESPONDE_SMS "+CMGF:0\r\n\r\nOK\r\n"
#define CMD_RESPONDE_SMS2 "+CMGF:1\r\n\r\nOK\r\n"

typedef enum
{
    MODE_PDU=0,
    MODE_TEXT,
}em_bg96_sms_mode;

typedef struct 
{
    uint8_t sms_mode;
}st_bg96_data_sms;


em_bg96_error_handling set_sms_format(st_bg96_data_sms *obj,em_bg96_sms_mode mode);
em_bg96_error_handling send_sms(char *number,char *msg);

#endif