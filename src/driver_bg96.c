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
    if (obj->send_data_device!=NULL)
    {
        obj->ft_resp=FT_BG96_OK;
    }            
    else obj->code_error=BG96_ERROR_INIT;
    return obj->ft_resp;
}
em_bg96_error_handling get_status_modem(st_bg96_config*obj)
{
    char buffer_receive[30]; 
    obj->ft_resp=obj->send_data_device(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer_receive,100);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_STATUS_MODEM;
    }
    return obj->ft_resp;
}
em_bg96_error_handling get_status_sim(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char buffer_parse[10]={0};
    
    obj->ft_resp=obj->send_data_device(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==obj->ft_resp)
    {
        parse_string(buffer_receive,',','\r',buffer_parse);
        uint8_t un_data=(uint8_t)atoi(buffer_parse);
        if (un_data==0)
        {
            obj->sim_comfig->sim_state=US_STATE_INSERTED;
        }
        else if (un_data==1)
        {
            obj->sim_comfig->sim_state=US_STATE_REMOVED;
        }
        else if (un_data==2)
        {
            obj->sim_comfig->sim_state=US_STATE_UNKNOWNU;
        }    
    }
    return obj->ft_resp;
}

em_bg96_error_handling set_mode_echo(st_bg96_config *obj,em_status_echo mode)
{
    char buffer_receive[30]={0};
    obj->ft_resp=obj->send_data_device(mode==0?CMD_BG96_MODE_ECHO_OFF:CMD_BG96_MODE_ECHO_ON,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==obj->ft_resp)
    {
       obj->mode_echo=mode;
    }
    else obj->code_error=BG96_ERROR_SET_MODE_ECHO;   
    return obj->ft_resp;
}

em_bg96_error_handling set_format_response(st_bg96_config *obj,em_format_response mode)
{
    char buffer_receive[30]={0};
    obj->ft_resp=obj->send_data_device(mode==0? CMD_BG96_MODE_RESPONSE_SHORT:CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer_receive,300);
    if (FT_BG96_OK==obj->ft_resp)
    {
        obj->format_response=mode;
    }
    else obj->code_error=BG96_ERROR_SET_FORMAT_RESPONSE;
    return obj->ft_resp;
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
    char cmd[30]="AT+CMGS=\"";
    strcat(cmd,number);
    strcat(cmd,"\"\r");
    strcpy(buffer_message,message);
    strcat(buffer_message,"\r");
    
    ft_resp=obj->send_data_device(cmd,RS_BG96_SIGNAL,buffer_receive,12000);
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

em_bg96_error_handling set_parameter_context_tcp(st_bg96_config *obj)
{
    char buffer_receive[50]={0};
    char cmd[100];
    sprintf(cmd,"AT+QICSGP=%u,%u,\"%s\",\"%s\",\"%s\",%u\r",obj->obj_tcp->context_id,obj->obj_tcp->context_type,obj->obj_tcp->tcp_apn,obj->obj_tcp->tcp_username,obj->obj_tcp->tcp_password,obj->obj_tcp->method_authentication);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,300);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_SET_PARAMETER_CONTEXT_TCP;
    }
    return obj->ft_resp;
}

em_bg96_error_handling activate_context_pdp(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[15];
    sprintf(cmd,"AT+QIACT=%u\r",obj->obj_tcp->context_id);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,150000);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_ACTIVATE_CONTEXT_PDP;
    }
    return obj->ft_resp;
}
em_bg96_error_handling desactivate_context_pdp(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[15];
    sprintf(cmd,"AT+QIDEACT=%u\r",obj->obj_tcp->context_id);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_OK,buffer_receive,40000);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_DESACTIVATE_CONTEXT_PDP;
    }
    return obj->ft_resp;
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
em_bg96_error_handling open_client_mqtt(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[50];
    sprintf(cmd,"AT+QMTOPEN=%u,%s,%u\r",obj->obj_mqtt->identifier_socket_mqtt,obj->obj_mqtt->host_name,obj->obj_mqtt->port);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_CERO,buffer_receive,75000);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_OPEN_CLIENT_MQTT;
    }
    return obj->ft_resp;
}
em_bg96_error_handling close_client_mqtt(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[50];
    sprintf(cmd,"AT+QMTCLOSE=%u\r",obj->obj_mqtt->identifier_socket_mqtt);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_CERO,buffer_receive,300);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_CLOSE_CLIENT_MQTT;
    }
    return obj->ft_resp;
}
em_bg96_error_handling connect_server_mqtt(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[50]={0};
    sprintf(cmd,"AT+QMTCONN=%u,\"%s\",\"%s\",\"%s\"\r",obj->obj_mqtt->identifier_socket_mqtt,obj->obj_mqtt->mqtt_client_id,obj->obj_mqtt->mqtt_username,obj->obj_mqtt->mqtt_password);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_CERO,buffer_receive,5000);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_CONNECT_SERVER_MQTT;
    }
   return obj->ft_resp;
}

em_bg96_error_handling disconnect_server_mqtt(st_bg96_config *obj)
{
    char buffer_receive[30]={0};
    char cmd[50]={0};
    sprintf(cmd,"AT+QMTDISC=%u\r",obj->obj_mqtt->identifier_socket_mqtt);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_CERO,buffer_receive,300);
    if (obj->ft_resp!=FT_BG96_OK)
    {
        obj->code_error=BG96_ERROR_DISCONNECT_SERVER_MQTT;
    }
    return obj->ft_resp;
}
em_bg96_error_handling publish_message(st_bg96_config *obj,char *topic,char *data)
{
    char buffer_receive[30]={0};
    char cmd[50]={0};
    sprintf(cmd,"AT+QMTPUB=%u,0,0,0,\"%s\"\r",obj->obj_mqtt->identifier_socket_mqtt,topic);
    obj->ft_resp=obj->send_data_device(cmd,RS_BG96_SIGNAL,buffer_receive,300);
    if (FT_BG96_OK==obj->ft_resp)
    {
        obj->ft_resp=obj->send_data_device(data,NULL,buffer_receive,300);
        if (FT_BG96_OK==obj->ft_resp)
        {
            obj->ft_resp=obj->send_data_device("\x1A\r",RS_BG96_CERO,buffer_receive,15000);
        }
        else obj->code_error=BG96_ERROR_PUBLISH_MESSAGE;   
    }
    else obj->code_error=BG96_ERROR_PUBLISH_MESSAGE;
    return obj->ft_resp;
}


em_bg96_error_handling send_data_mqtt(st_bg96_config *obj,char *topic,char *data)
{
    em_states_send_data states_send_data_mqtt=INIT_SEND_DATA_MQTT;
    uint8_t flag_machine=1;
    while (flag_machine==1)
    {
        switch (states_send_data_mqtt)
        {
        case INIT_SEND_DATA_MQTT:
            if (get_status_modem(obj)==FT_BG96_OK)
            {
                states_send_data_mqtt=CONFIG_PDP_CONTEXT;
                /*if (get_status_sim(obj)==FT_BG96_OK)
                {
                    if (obj_sim_comfig->sim_state==US_STATE_INSERTED)
                    {
                        states_send_data_mqtt=CONFIG_PDP_CONTEXT;
                    }
                }
                else states_send_data_mqtt=RESET_MODULE;*/
            }
            else states_send_data_mqtt=RESET_MODULE;
            break;
        case CONFIG_PDP_CONTEXT:
            if (set_parameter_context_tcp(obj)==FT_BG96_OK)
                    states_send_data_mqtt=ACTIVATE_PDP_CONTEXT;
            else states_send_data_mqtt=RESET_MODULE;                       
            break;
        case ACTIVATE_PDP_CONTEXT:
            if (activate_context_pdp(obj)==FT_BG96_OK)
                states_send_data_mqtt=OPEN_CONNECTION_MQTT; 
            else states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;        
            break;
        case OPEN_CONNECTION_MQTT:
            if (open_client_mqtt(obj)==FT_BG96_OK)
                states_send_data_mqtt=CONNECT_BROKER_MQTT;
            else states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;
            break;
        case CONNECT_BROKER_MQTT:
            if (connect_server_mqtt(obj)==FT_BG96_OK)
                states_send_data_mqtt=PUB_MQTT;
            else states_send_data_mqtt=DISCONNECT_BROKER_MQTT;
            break;
        case PUB_MQTT:
            if (publish_message(obj,topic,data)==FT_BG96_OK)
                states_send_data_mqtt=DISCONNECT_BROKER_MQTT;             
            break;
        case DISCONNECT_BROKER_MQTT:
            if (disconnect_server_mqtt(obj)==FT_BG96_OK)
            {
                states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;
            }
            break;
        case DESACTIVATE_PDP_CONTEXT:
            if (desactivate_context_pdp(obj)==FT_BG96_OK)
            {   
                states_send_data_mqtt=RESET_MODULE;
                break;
            }
        case RESET_MODULE:
            flag_machine=0;
            break;
        default:
            flag_machine=0;
            break;
        }   
    }
    return obj->ft_resp;   
}
