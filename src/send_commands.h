
#include <stdint.h>

typedef enum
{
    bg96_OK=0,
    bg96_ERROR,
    bg96_TIMEOUT,
}status_send_cmd;


status_send_cmd bg96_SendCmdSingle( char *pCmd, char *expectResponde_OK, char *expectResponde_ERROR,char *md_Responde,uint8_t timeout);

