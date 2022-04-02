/**
 * @file driver_bg96.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DRIVER_BG96
#define DRIVER_BG96

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"



typedef enum
{
  FT_BG96_OK=0,
  FT_BG96_ERROR,
  FT_BG96_BUSY,
  FT_BG96_TIMEOUT,
} em_bg96_error_handling;

typedef em_bg96_error_handling(*pf_send_data)(char*,char*,char*,uint32_t);//comando,respuest,buffer,time
typedef void (*pf_turn_on_modem)(void);
typedef enum
{
    MODE_PDU=0,
    MODE_TEXT,
}em_sms_mode;

typedef struct 
{
    uint8_t sms_mode;
}st_bg96_data_sms;

typedef enum 
{
    US_STATE_INSERTED=0,
    US_STATE_REMOVED,
    US_STATE_UNKNOWNU,
}em_sim_states;

typedef enum
{
    STATE_ECHO_ON=0,
    STATE_ECHO_OFF,        
}em_status_echo;

typedef enum
{
    SHORT_RESULT=0,
    LONG_RESULT,
}em_format_response;

typedef enum
{
    ME_DISABLE=0,
    ME_ENABLE_NUMERIC,
    ME_ENABLE_TEXT,
}em_format_error;

typedef enum
{
    TURN_ON_MODEM,
    INIT_SEND_DATA_MQTT,
    RESET_MODULE,
    CONFIG_PDP_CONTEXT,
    ACTIVATE_PDP_CONTEXT,
    OPEN_CONNECTION_MQTT,
    CONNECT_BROKER_MQTT,
    PUB_MQTT,
    DESACTIVATE_PDP_CONTEXT,
    DISCONNECT_BROKER_MQTT,
}em_states_send_data;


typedef enum
{
    STATUS_MODEM,
    STATUS_SIM,
    SET_MODE_TEXT,
    SEND_DATA_SMS,
    ERROR_SEND_DATA,
}em_states_send_sms;

typedef struct 
{
    uint8_t context_id;            //ID del contexto rango 1-16
    uint8_t context_type;          //Tipo de protocolo 1=IPV4,2=IPV6
    char *tcp_apn;                 //Nombre del punto de acceso
    char *tcp_username;           //Nombre de usuario
    char *tcp_password;            //Password 
    uint8_t method_authentication; //Metodo de autentificacion 
}st_config_context_tcp;

typedef struct 
{
    uint8_t identifier_socket_mqtt; //Numero del socket configurado 1-5
    uint8_t quality_service;        //Calidad de servicio del socket MQTT 
    char *host_name;
    uint32_t port;
    char *mqtt_username;
    char *mqtt_password;
    char *mqtt_client_id;
}st_config_parameters_mqtt;


typedef struct 
{
    em_sim_states sim_state;

}st_config_sim;

typedef struct 
{
    pf_send_data       send_data_device;
    pf_turn_on_modem   turn_on_modem;
    em_status_echo     mode_echo;
    em_format_response format_response;
    em_format_error    format_error;
    em_sms_mode        mode_sms; 
    uint32_t           code_error;
    em_bg96_error_handling ft_resp;
    st_config_sim      *sim_comfig;
    st_config_context_tcp *obj_tcp;
    st_config_parameters_mqtt *obj_mqtt;
}st_bg96_config;

em_bg96_error_handling init_driver(st_bg96_config *obj,pf_send_data ft_send_data_device,pf_turn_on_modem ft_turn_on_modem);

em_bg96_error_handling get_status_modem(st_bg96_config *obj);
em_bg96_error_handling get_status_sim(st_bg96_config *obj);

em_bg96_error_handling set_mode_echo(st_bg96_config *obj,em_status_echo mode);
em_bg96_error_handling set_format_response(st_bg96_config *obj,em_format_response mode);
em_bg96_error_handling set_format_error(st_bg96_config *obj,em_format_error mode);
em_bg96_error_handling set_sms_format(st_bg96_config *obj,em_sms_mode mode);
em_bg96_error_handling set_parameter_context_tcp(st_bg96_config *obj);
em_bg96_error_handling send_sms_bg96(st_bg96_config *obj,char*number,char*message);


em_bg96_error_handling activate_context_pdp(st_bg96_config *obj);
em_bg96_error_handling desactivate_context_pdp(st_bg96_config *obj);

em_bg96_error_handling set_parameters_mqtt(st_bg96_config *obj,st_config_parameters_mqtt *obj_mqtt,st_config_context_tcp *obj_tcp);
em_bg96_error_handling open_client_mqtt(st_bg96_config *obj);
em_bg96_error_handling close_client_mqtt(st_bg96_config *obj);
em_bg96_error_handling connect_server_mqtt(st_bg96_config *obj);
em_bg96_error_handling disconnect_server_mqtt(st_bg96_config *obj);
em_bg96_error_handling publish_message(st_bg96_config *obj,char *topic,char *data);

em_bg96_error_handling send_data_mqtt(st_bg96_config *obj,char *topic,char *data);
#endif