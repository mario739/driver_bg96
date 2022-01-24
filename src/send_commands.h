
#ifndef SEND_COMMANDS
#define SEND_COMMANDS

#include <stdint.h>

typedef enum
{
  FT_BG96_OK=0,
  FT_BG96_ERROR,
  FT_BG96_BUSY,
  FT_BG96_TIMEOUT,
} em_bg96_error_handling;

em_bg96_error_handling bg96_SendCmdSingle( char *pCmd, char *expectResponde_OK, char *expectResponde_ERROR,char *md_Responde,uint32_t timeout);

#endif