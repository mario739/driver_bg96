/**
 * @file commands.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef COMMANDS
#define COMMANDS

#define CMD_BG96_STATUS_MODEM   "AT\r"
#define CMD_BG96_STATUS_SIM     "AT+QSIMSTAT?\r"  
#define CMD_BG96_MODE_ECHO_OFF  "ATE0\r"
#define CMD_BG96_MODE_ECHO_ON   "ATE1\r"
#define CMD_BG96_MODE_RESPONSE_LONG  "ATV1\r"
#define CMD_BG96_MODE_RESPONSE_SHORT "ATV0\r"




#define RS_BG96_OK  "\r\nOK\r\n"
#define RS_BG96_STATUS_SIM "\r\n+QSIMSTAT: 0,1\r\n\r\nOK\r\n"

#define CMD_TEST_BG96_FORMAT_ERROR "AT+CMEE=2\r"
#define CMD_TEST_BG96_MODE_SMS     "AT+CMGF=1\r"

#define CMD_RESPONDE_SMS "+CMGF:0\r\n\r\nOK\r\n"
#define CMD_RESPONDE_SMS2 "+CMGF:1\r\n\r\nOK\r\n"
#endif
