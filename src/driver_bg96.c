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

em_bg96_error_handling init_driver(st_bg96_config *obj,pf_send_data ft_send_data_device)
{
    obj->send_data_device=ft_send_data_device;
    return FT_BG96_OK;
}
em_bg96_error_handling get_status_modem(st_bg96_config*obj)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30];
    
    ft_resp=obj->send_data_device(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer_receive,100);
    return ft_resp;
}
em_bg96_error_handling get_status_sim(st_bg96_config *obj,st_config_sim *sim_comfig)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char buffer_parse[10]={0};
    
    ft_resp=obj->send_data_device(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==ft_resp)
    {
        parse_string(buffer_receive,',','\r',buffer_parse);
        uint8_t un_data=(uint8_t)atoi(buffer_parse);
        if (un_data==0)
        {
            sim_comfig->sim_state=US_STATE_INSERTED;
        }
        else if (un_data==1)
        {
            sim_comfig->sim_state=US_STATE_REMOVED;
        }
        else if (un_data==2)
        {
            sim_comfig->sim_state=US_STATE_UNKNOWNU;
        }    
    }
    return ft_resp;
}

em_bg96_error_handling set_mode_echo(st_bg96_config *obj,em_status_echo mode)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    
    ft_resp=obj->send_data_device(mode>0?CMD_BG96_MODE_ECHO_OFF:CMD_BG96_MODE_ECHO_ON,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==ft_resp)
    {
       obj->mode_echo=mode;
    }   
    return ft_resp;
}

em_bg96_error_handling set_format_response(st_bg96_config *obj,em_format_response mode)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    
    ft_resp=obj->send_data_device(mode==0? CMD_BG96_MODE_RESPONSE_SHORT:CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==ft_resp)
    {
        obj->format_response=mode;
    }
    return ft_resp;
}

em_bg96_error_handling set_format_error(st_bg96_config *obj,em_format_error mode)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd[20];
    sprintf(cmd,"AT+CMEE=%u\r",mode);

    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==ft_resp)
    {
        obj->format_error=mode;
    }
    return ft_resp;
}
em_bg96_error_handling set_sms_format(st_bg96_config *obj,em_sms_mode mode)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd[12]="AT+CMGF=";
    strcat(cmd,(mode>0)?"1":"0");
    strcat(cmd,"\r");

    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,300);   
    if (FT_BG96_OK==ft_resp)
    {
        obj->mode_sms=mode;
    }
    return ft_resp;
}

em_bg96_error_handling send_sms_bg96(st_bg96_config *obj,char*number,char*message)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char buffer_message[256];
    char *expect_responde_sg=">";
    char cmd[30]="AT+CMGS=\"";
    strcat(cmd,number);
    strcat(cmd,"\"\r");
    strcpy(buffer_message,message);
    strcat(buffer_message,"\r");
    
    ft_resp=obj->send_data_device(cmd,expect_responde_sg,buffer_receive,12000);
    if (FT_BG96_OK==ft_resp)
    {
        ft_resp=obj->send_data_device(buffer_message,NULL,buffer_receive,12000);
        if (FT_BG96_OK==ft_resp)
        {
           ft_resp=obj->send_data_device("\x1A\r",RS_BG96_OK,buffer_receive,12000);
        }
    }
    return ft_resp;
}

em_bg96_error_handling set_parameter_context_tcp(st_bg96_config *obj,st_config_context_tcp *obj_tcp)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    //char cmd[35]="AT+QICSGP=1,1,\"";
    char cmd[100];
    sprintf(cmd,"AT+QICSGP=%u,%u,\"%s\",\"%s\",\"%s\",%u\r",obj_tcp->context_id,obj_tcp->context_type,obj_tcp->tcp_apn,obj_tcp->tcp_username,obj_tcp->tcp_password,obj_tcp->method_authentication);
    /*strcat(cmd,obj_tcp->tcp_apn);
    strcat(cmd,"\",\"");
    strcat(cmd,obj_tcp->tcp_user_name);
    strcat(cmd,"\",\"");
    strcat(cmd,obj_tcp->tcp_password);
    strcat(cmd,"\",1\r");*/
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,300);
    return ft_resp;
}

em_bg96_error_handling activate_context_pdp(st_bg96_config *obj,st_config_context_tcp *obj_tcp)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    //char cmd[12]="AT+QIACT=1\r";
    char cmd[15];
    sprintf(cmd,"AT+QIACT=%u\r",obj_tcp->context_id);
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,150000);
    return ft_resp;
}
em_bg96_error_handling desactivate_context_pdp(st_bg96_config *obj,st_config_context_tcp *obj_tcp)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    //char cmd[15]="AT+QIDEACT=1\r";
    char cmd[15];
    sprintf(cmd,"AT+QIDEACT=%u\r",obj_tcp->context_id);
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,150000);
    return ft_resp;
}
em_bg96_error_handling set_parameters_mqtt(st_bg96_config *obj,st_config_parameters_mqtt *obj_mqtt,st_config_context_tcp *obj_tcp)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd_pdpid[30];
    sprintf(cmd_pdpid,"AT+QMTCFG=\"pdpid\",%u,%u\r",obj_mqtt->identifier_socket_mqtt,obj_tcp->context_id);
    ft_resp=obj->send_data_device(cmd_pdpid,RS_BG96_OK,buffer_receive,300);
    return ft_resp;
}
em_bg96_error_handling open_client_mqtt(st_bg96_config *obj,st_config_parameters_mqtt *obj_mqtt)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd[50];
    sprintf(cmd,"AT+QMTOPEN=%u,%s,%u\r",obj_mqtt->identifier_socket_mqtt,obj_mqtt->host_name,obj_mqtt->port);
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,75000);
    return ft_resp;
}
em_bg96_error_handling close_client_mqtt(st_bg96_config *obj,st_config_parameters_mqtt *obj_mqtt)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd[50];
    sprintf(cmd,"AT+QMTCLOSE=%u\r",obj_mqtt->identifier_socket_mqtt);
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,300);
    return ft_resp;
}
em_bg96_error_handling connect_server_mqtt(st_bg96_config *obj,st_config_parameters_mqtt *obj_mqtt)
{
    em_bg96_error_handling ft_resp=FT_BG96_ERROR;
    char buffer_receive[30]={0};
    char cmd[50];
    sprintf(cmd,"AT+QMTCONN=%u,\"%s\",\"%s\",\"%s\"\r",obj_mqtt->identifier_socket_mqtt,obj_mqtt->mqtt_client_id,obj_mqtt->mqtt_username,obj_mqtt->mqtt_password);
    ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,5000);
    return ft_resp;
}