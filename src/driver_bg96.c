/**
 * @file driver_bg96.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "driver_bg96.h"


em_bg96_error_handling set_sms_format(st_bg96_data_sms *obj,em_bg96_sms_mode mode)
{
    em_bg96_error_handling ft_status;
    char cmd[12]="AT+CMGF=";
    char *cmd_get="AT+CMGF?";
    strcat(cmd,(mode>0)?"1":"0");
    strcat(cmd,"\r");
    
    char *expectResponde_OK="OK";
    char *expectResponde_ERROR="ERROR";
    char buffer_responde[30];
    ft_status=bg96_SendCmdSingle(cmd,expectResponde_OK,expectResponde_ERROR,buffer_responde,300);   
    if (FT_BG96_OK==ft_status)
    {
        ft_status=bg96_SendCmdSingle(cmd_get,expectResponde_OK,expectResponde_ERROR,buffer_responde,300);
        if (FT_BG96_OK==ft_status)
        {
            if (strstr(buffer_responde,"0")!= NULL)
            {
                obj->sms_mode=0;
            }
            else if (strstr(buffer_responde,"1")!= NULL)
            {
                obj->sms_mode=1;
            }
        }
    }
    return ft_status;

}

em_bg96_error_handling  send_sms(char *number,char *msg)
{
    em_bg96_error_handling ft_status;
    char *cmd="AT+CMGS=\"";
    char *cmd_cpl="\"\r";
    char buffer_cmd[30];
    strcpy(buffer_cmd,cmd);
    strcat(buffer_cmd,number);
    strcat(buffer_cmd,cmd_cpl);

    char buffer_sms[200];
    strcpy(buffer_sms,msg);    
    strcat(buffer_sms,"\r");

    char *expectResponde_OK="OK";
    char *expectResponde_sg=">";
    char *expectResponde_ERROR="ERROR";
    char buffer_responde[100];

    ft_status=bg96_SendCmdSingle(buffer_cmd,expectResponde_sg,expectResponde_ERROR,buffer_responde,120); 
    if (FT_BG96_OK==ft_status)
    {
       if (strstr(buffer_responde,">")!= NULL)
       {
            ft_status=bg96_SendCmdSingle(buffer_sms,expectResponde_OK,expectResponde_ERROR,buffer_responde,120);
            if (FT_BG96_OK==ft_status)
            {
                ft_status=bg96_SendCmdSingle("\x1A\r",expectResponde_OK,expectResponde_ERROR,buffer_responde,120);
                if (FT_BG96_OK==ft_status)
                {
                    if (strstr(buffer_responde,"OK")!= NULL)
                    {
                        return ft_status;
                    }
                    else
                    {
                        return FT_BG96_ERROR;
                    }
                }
            }
       }
       else
       {
           ft_status=FT_BG96_ERROR;
       }
    }
    return ft_status;
}
