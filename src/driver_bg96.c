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

em_bg96_error_handling init_driver(st_bg96_config *self,pf_send_data ft_send_data_device,pf_turn_on_modem ft_turn_on_modem)
{
    self->send_data_device=ft_send_data_device;
    self->turn_on_modem=ft_turn_on_modem;
    if (self->send_data_device!=NULL)
    {
        self->ft_resp=FT_BG96_OK;
    }            
    else self->last_error=BG96_ERROR_INIT;
    
    return self->ft_resp;
}
em_bg96_error_handling get_status_modem(st_bg96_config* self)
{
    char buffer_resp[20]={0};
    self->ft_resp=self->send_data_device(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer_resp,1000);
    if (self->ft_resp!=FT_BG96_OK)
    {
        self->last_error=BG96_ERROR_STATUS_MODEM;
    }
    return self->ft_resp;
}
em_bg96_error_handling get_status_sim(st_bg96_config *self)
{   
    char buffer_resp[50]={0};  
    self->ft_resp=self->send_data_device(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer_resp,5000);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_STATUS_SIM;
    }
    return self->ft_resp;
}
em_bg96_error_handling get_information_product(st_bg96_config *self)
{
    char buffer_resp[100]={0};  
    self->ft_resp=self->send_data_device(CMD_BG96_GET_INFO_PRODUCT,RS_BG96_OK,buffer_resp,300);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_INFO_PRODUCTO;
    }
    else
    {   
        // self->info_product.mark=(uint8_t)"data";
        // self->info_product.model="";
        // self->info_product.revision="";
        //procesar caracteres 
    }
    return self->ft_resp;
    
}
em_bg96_error_handling get_status_pdp_context(st_bg96_config *self)
{
    char buffer_resp[100]={0}; 
    self->ft_resp=self->send_data_device(CMD_BG96_STATUS_PDP_CONTEXT,RS_BG96_OK,buffer_resp,1000);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_PDP_CONTEXT;
    }
    return self->ft_resp;
}


/**
 * @brief Set the mode echo object
 * 
 * @param self puntero de la structura del objeto del modulo 
 * @param mode 1:enable 0:unenable 
 * @return em_bg96_error_handling 
 */

em_bg96_error_handling set_mode_echo(st_bg96_config *self,uint8_t mode)
{
    char buffer_resp[30]={0};
    self->ft_resp=self->send_data_device( mode==0? CMD_BG96_MODE_ECHO_OFF:CMD_BG96_MODE_ECHO_ON,RS_BG96_OK,buffer_resp,300);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_SET_MODE_ECHO;   
    }
    return self->ft_resp;
}


/**
 * @brief Set the format response object
 * 
 * @param self 
 * @param mode 0:Short 1:Long
 * @return em_bg96_error_handling 
 */

em_bg96_error_handling set_format_response(st_bg96_config *self,uint8_t mode)
{
    char buffer_resp[30]={0};
    self->ft_resp=self->send_data_device(mode==0? CMD_BG96_MODE_RESPONSE_SHORT:CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer_resp,300);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_SET_FORMAT_RESPONSE;
    }
    return self->ft_resp;
}


/**
 * @brief Set the format error object
 * 
 * @param self 
 * @param mode 0:Disable 1:Numeric 2:Text
 * @return em_bg96_error_handling 
 */
em_bg96_error_handling set_format_error(st_bg96_config *self,uint8_t mode)
{
    char buffer_resp[30]={0};
    char cmd[20];
    sprintf(cmd,"AT+CMEE=%u\r",mode);
    self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,buffer_resp,300);
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_SET_MODE_ERROR;
    }
    return self->ft_resp;
}


/**
 * @brief Set the sms format object
 * 
 * @param self 
 * @param mode 0:PDU 1:TEXT
 * @return em_bg96_error_handling 
 */
em_bg96_error_handling set_sms_format(st_bg96_config *self,uint8_t mode)
{
    char buffer_resp[30]={0};    
    char cmd[12]="AT+CMGF=";
    strcat(cmd,(mode>0)?"1":"0");
    strcat(cmd,"\r");
    self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,buffer_resp,300);   
    if (FT_BG96_OK!=self->ft_resp)
    {
        self->last_error=BG96_ERROR_SET_SMS_FORMAT;
    }
    return self->ft_resp;
}

// em_bg96_error_handling send_sms_bg96(st_bg96_config *self,char*number,char*message)
// {
//     char buffer_message[256];
//     char cmd[30]="AT+CMGS=\"";
//     strcat(cmd,number);
//     strcat(cmd,"\"\r");
//     strcpy(buffer_message,message);
//     strcat(buffer_message,"\x1a\r");  
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_SIGNAL,12000);
//     if (FT_BG96_OK==self->ft_resp)
//     {
//         self->ft_resp=self->send_data_device(buffer_message,RS_BG96_OK,12000);
//         if (FT_BG96_OK!=self->ft_resp)
//         {
//             self->last_error=BG96_ERROR_SEND_SMS;
//         }
//     }
//     return self->ft_resp;
// }

// em_bg96_error_handling set_parameter_context_tcp(st_bg96_config *self)
// {
//     char cmd[100];
//     sprintf(cmd,"AT+QICSGP=%u,%u,\"%s\",\"%s\",\"%s\",%u\r",self->self_tcp->context_id,self->self_tcp->context_type,self->self_tcp->tcp_apn,self->self_tcp->tcp_username,self->self_tcp->tcp_password,self->self_tcp->method_authentication);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,300);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_SET_PARAMETER_CONTEXT_TCP;
//     }
//     return self->ft_resp;
// }

// em_bg96_error_handling activate_context_pdp(st_bg96_config *self)
// {
//     char cmd[15];
//     sprintf(cmd,"AT+QIACT=%u\r",self->self_tcp->context_id);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,15000);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_ACTIVATE_CONTEXT_PDP;
//     }
//     return self->ft_resp;
// }
// em_bg96_error_handling desactivate_context_pdp(st_bg96_config *self)
// {
//     char cmd[15];
//     sprintf(cmd,"AT+QIDEACT=%u\r",self->self_tcp->context_id);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,40000);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_DESACTIVATE_CONTEXT_PDP;
//     }
//     return self->ft_resp;
// }
// em_bg96_error_handling set_parameters_mqtt(st_bg96_config *self,st_config_parameters_mqtt *self_mqtt,st_config_context_tcp *self_tcp)
// {
//     em_bg96_error_handling ft_resp=FT_BG96_ERROR;
//     char cmd_pdpid[30];
//     sprintf(cmd_pdpid,"AT+QMTCFG=\"pdpid\",%u,%u\r",self_mqtt->identifier_socket_mqtt,self_tcp->context_id);
//     ft_resp=self->send_data_device(cmd_pdpid,RS_BG96_OK,300);
//     return ft_resp;
// }
// em_bg96_error_handling open_client_mqtt(st_bg96_config *self)
// {
//     char cmd[50];
//     sprintf(cmd,"AT+QMTOPEN=%u,%s,%u\r",self->self_mqtt->identifier_socket_mqtt,self->self_mqtt->host_name,self->self_mqtt->port);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_CERO,75000);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_OPEN_CLIENT_MQTT;
//     }
//     return self->ft_resp;
// }
// em_bg96_error_handling close_client_mqtt(st_bg96_config *self)
// {
//     char cmd[50];
//     sprintf(cmd,"AT+QMTCLOSE=%u\r",self->self_mqtt->identifier_socket_mqtt);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_CERO,300);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_CLOSE_CLIENT_MQTT;
//     }
//     return self->ft_resp;
// }
// em_bg96_error_handling connect_server_mqtt(st_bg96_config *self)
// {
//     char cmd[50]={0};
//     sprintf(cmd,"AT+QMTCONN=%u,\"%s\",\"%s\",\"%s\"\r",self->self_mqtt->identifier_socket_mqtt,self->self_mqtt->mqtt_client_id,self->self_mqtt->mqtt_username,self->self_mqtt->mqtt_password);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_CERO,5000);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_CONNECT_SERVER_MQTT;
//     }
//    return self->ft_resp;
// }

// em_bg96_error_handling disconnect_server_mqtt(st_bg96_config *self)
// {
//     char cmd[50]={0};
//     sprintf(cmd,"AT+QMTDISC=%u\r",self->self_mqtt->identifier_socket_mqtt);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_CERO,5000);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_DISCONNECT_SERVER_MQTT;
//     }
//     return self->ft_resp;
// }
// em_bg96_error_handling publish_message(st_bg96_config *self,char *topic,char *data)
// {
//     char cmd[50]={0};
//     char buffer_data[120]={0};
//     sprintf(buffer_data,"%s\x1a\r",data);
//     sprintf(cmd,"AT+QMTPUB=%u,0,0,0,\"%s\"\r",self->self_mqtt->identifier_socket_mqtt,topic);
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_SIGNAL,300);
//     if (FT_BG96_OK==self->ft_resp)
//     {
//         self->ft_resp=self->send_data_device(buffer_data,RS_BG96_CERO,15000);
//         if (self->ft_resp!=FT_BG96_OK)
//         {
//             self->last_error=BG96_ERROR_PUBLISH_MESSAGE;
//         }   
//     }
//     else self->last_error=BG96_ERROR_PUBLISH_MESSAGE;
//     return self->ft_resp;
// }

// em_bg96_error_handling turn_off_bg96(st_bg96_config *self)
// {
//     char cmd[15]="AT+QPOWD\r";
//     self->ft_resp=self->send_data_device(cmd,RS_BG96_OK,100);
//     if (self->ft_resp!=FT_BG96_OK)
//     {
//         self->last_error=BG96_ERROR_TURN_OFF;
//     }
//     return self->ft_resp;

// }

// em_bg96_error_handling send_data_mqtt(st_bg96_config *self,char *topic,char *data)
// {
//     em_states_send_data states_send_data_mqtt=TURN_ON_MODEM;
//     uint count=0;
//     uint8_t flag_machine=1;
//     while (flag_machine==1)
//     {
//         switch (states_send_data_mqtt)
//         {
//         case TURN_ON_MODEM:
//             self->turn_on_modem();
//             states_send_data_mqtt=INIT_SEND_DATA_MQTT;
//             break;
//         case INIT_SEND_DATA_MQTT:
//             if (get_status_modem(self)==FT_BG96_OK)
//                 states_send_data_mqtt=CONFIG_PDP_CONTEXT;
//             else
//             {
//                 if (count==3)
//                 {
//                     states_send_data_mqtt=RESET_MODULE;
//                 }
//                 else count++;
//             }   
//             break;
//         case CONFIG_PDP_CONTEXT:
//             if (set_parameter_context_tcp(self)==FT_BG96_OK)
//                     states_send_data_mqtt=ACTIVATE_PDP_CONTEXT;
//             else
//             {
//                 if (count==3)
//                 {
//                     states_send_data_mqtt=RESET_MODULE;
//                 }
//                 else count++;
//             }                        
//             break;
//         case ACTIVATE_PDP_CONTEXT:
//             if (activate_context_pdp(self)==FT_BG96_OK)
//                 states_send_data_mqtt=OPEN_CONNECTION_MQTT; 
//             else states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;        
//             break;
//         case OPEN_CONNECTION_MQTT:
//             if (open_client_mqtt(self)==FT_BG96_OK)
//                 states_send_data_mqtt=CONNECT_BROKER_MQTT;
//             else states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;
//             break;
//         case CONNECT_BROKER_MQTT:
//             if (connect_server_mqtt(self)==FT_BG96_OK)
//                 states_send_data_mqtt=PUB_MQTT;
//             else states_send_data_mqtt=DISCONNECT_BROKER_MQTT;
//             break;
//         case PUB_MQTT:
//             if (publish_message(self,topic,data)==FT_BG96_OK)
//                 states_send_data_mqtt=DISCONNECT_BROKER_MQTT;             
//             break;
//         case DISCONNECT_BROKER_MQTT:
//             if (disconnect_server_mqtt(self)==FT_BG96_OK)
//             {
//                 states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;
//             }
//             else
//             {
//                 states_send_data_mqtt=DESACTIVATE_PDP_CONTEXT;
//             }
//             break;
//         case DESACTIVATE_PDP_CONTEXT:
//             if (desactivate_context_pdp(self)==FT_BG96_OK)
//             {   
//                 states_send_data_mqtt=RESET_MODULE;
//                 break;
//             }
//             else
//             {
//                 states_send_data_mqtt=RESET_MODULE;
//             }
//         case RESET_MODULE:
//             if (turn_off_bg96(self)==FT_BG96_OK)
//             {
//                 flag_machine=0;
//             }
//             flag_machine=0; 
//             break;
//         default:
//             flag_machine=0;
//             break;
//         }   
//     }
//     return self->ft_resp;   
// }
// em_bg96_error_handling send_data_sms(st_bg96_config *self,em_sms_mode mode,char *number,char *message)
// {
//     uint8_t flag_send_sms=1;
//     em_states_send_sms states_send_sms=STATUS_MODEM;
//     while (flag_send_sms==1)
//     {
//         switch (states_send_sms)
//         {
//         case STATUS_MODEM:
//             if (get_status_modem(self)==FT_BG96_OK)
//                 states_send_sms=STATUS_SIM;
//             else states_send_sms=ERROR_SEND_DATA;
//             break;
//         case STATUS_SIM:
//             if (get_status_sim(self)==FT_BG96_OK)
//                 states_send_sms=SET_MODE_TEXT;
//             else states_send_sms=ERROR_SEND_DATA;
//             break;
//         case SET_MODE_TEXT:
//             if (set_sms_format(self,mode)==FT_BG96_OK)
//                 states_send_sms=SEND_DATA_SMS;
//             else states_send_sms=ERROR_SEND_DATA;
//             break;
//         case SEND_DATA_SMS:
//             if (send_sms_bg96(self,number,message)==FT_BG96_OK)
//                 flag_send_sms=0;
//             break;
//         case ERROR_SEND_DATA:
//             flag_send_sms=0;
//             break;
//         default:
//             flag_send_sms=0;
//             break;
//         }
//     }
    
// }