#ifndef  STATE_CONTROL
#define  STATE_CONTROL

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BG96_CMD_INFORMATION
#define CM_CMD_MOCK_GENERAL_RESPONSE     "\r\nOK\r\n" 
#define CMD_STATUS_USIM_RESPONSE_1       "+QSIMSTAT: 0,0\r\n\r\nOK\r\n"
#define CMD_STATUS_USIM_RESPONSE_2       "+QSIMSTAT: 0,1\r\n\r\nOK\r\n"
#define CMD_STATUS_USIM_RESPONSE_3       "+QSIMSTAT: 0,2\r\n\r\nOK\r\n"
#define CMD_STATUS                       "AT"
#define CMD_STATUS_USIM                  "AT+QSIMSTAT?"

typedef enum 
{
    MD_STATE_ON,
    MD_STATE_OFF,
}md_state;

typedef enum 
{
    US_STATE_INSERTED,
    US_STATE_REMOVED,
    US_UNKNOWNU,
}us_state;



md_state get_state_modem();
us_state get_status_usim();


#endif
