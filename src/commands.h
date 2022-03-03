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


#define BG96_NO_ERROR            0x0U   
#define BG96_ERROR_INIT          0x1U
#define BG96_ERROR_STATUS_MODEM  0x2U
#define BG96_ERROR_SET_MODE_ECHO 0x3U
#define BG96_ERROR_SET_FORMAT_RESPONSE 0x4U 
#define BG96_ERROR_SET_PARAMETER_CONTEXT_TCP 0x5U
#define BG96_ERROR_ACTIVATE_CONTEXT_PDP  0x6U
#define BG96_ERROR_OPEN_CLIENT_MQTT 0x7U
#define BG96_ERROR_DESACTIVATE_CONTEXT_PDP 0x8U
#define BG96_ERROR_CONNECT_SERVER_MQTT 0x9U
#define BG96_ERROR_PUBLISH_MESSAGE 0x10U
#define BG96_ERROR_DISCONNECT_SERVER_MQTT 0x11U
#define BG96_ERROR_CLOSE_CLIENT_MQTT 0x12U



#define CMD_BG96_STATUS_MODEM   "AT\r"
#define CMD_BG96_STATUS_SIM     "AT+QSIMSTAT?\r"  
#define CMD_BG96_MODE_ECHO_OFF  "ATE0\r"
#define CMD_BG96_MODE_ECHO_ON   "ATE1\r"
#define CMD_BG96_MODE_RESPONSE_LONG  "ATV1\r"
#define CMD_BG96_MODE_RESPONSE_SHORT "ATV0\r"




#define RS_BG96_OK              "\r\nOK\r\n"
#define RS_BG96_ERROR           "\r\nERROR\r\n"
#define RS_BG96_SIGNAL          ">"
#define RS_BG96_STATUS_SIM      "\r\n+QSIMSTAT: 0,1\r\n\r\nOK\r\n"
#define RS_BG96_CERO            "0\r\n"

#define CMD_TEST_BG96_FORMAT_ERROR "AT+CMEE=2\r"
#define CMD_TEST_BG96_MODE_SMS     "AT+CMGF=1\r"

#define CMD_RESPONDE_SMS "+CMGF:0\r\n\r\nOK\r\n"
#define CMD_RESPONDE_SMS2 "+CMGF:1\r\n\r\nOK\r\n"

#endif
