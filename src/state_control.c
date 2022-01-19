
#include "state_control.h"

md_state get_state_modem()
{
    char buffer[7];
    char cmd[]=CMD_STATUS;
    char *r_esperada="OK";
    char *r_esperada2="ERROR";
    bg96_SendCmdSingle(cmd,r_esperada,r_esperada2,buffer,100);   
    if(strcmp(buffer,CM_CMD_MOCK_GENERAL_RESPONSE)==0)
    {
        return MD_STATE_ON;
    } 
    else
    {
        return MD_STATE_OFF;
    } 
}

us_state get_status_usim()
{
    char buffer[23];
    char cmd[]=CMD_STATUS_USIM;
    char *r_esperada="OK";
    char *r_esperada2="ERROR";
    bg96_SendCmdSingle(cmd,r_esperada,r_esperada2,buffer,100);
    if (buffer[13]=='0')
    {
        return US_STATE_INSERTED;
    }
    else if (buffer[13]=='1')
    {
        return US_STATE_REMOVED;
    }
    else if(buffer[13]=='2')
    {
        return US_UNKNOWNU;
    }
    
    return 1;
}