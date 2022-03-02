/**
 * @file test_driver_bg96.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "unity.h"
#include "driver_bg96.h"
#include "mock_string_management.h"
#include "commands.h"

#define TS_RS_STATUS_SIM  "\r\n+QSIMSTAT: 0,1\r\n\r\nOK\r\n"

st_config_sim  config_sim={.sim_state=US_STATE_UNKNOWNU};
st_bg96_config config_module={.send_data_device=NULL,.mode_echo=STATE_ECHO_OFF,.format_response=SHORT_RESULT
                              ,.ft_resp=FT_BG96_ERROR,.code_error=BG96_NO_ERROR,.sim_comfig=&config_sim};
st_config_context_tcp config_context_tcp={.context_id=1,.context_type=1,.tcp_apn="4g.entel",.tcp_username="",.tcp_password="",.method_authentication=1};
st_config_parameters_mqtt config_parameters_mqtt={.identifier_socket_mqtt=0,.quality_service=0,.host_name="\"industrial.api.ubidots.com\"",.port=1883,.mqtt_client_id="123456789",.mqtt_username="",.mqtt_password=""};

void test_init_driver(void)
{   
   TEST_ASSERT_EQUAL(FT_BG96_OK,init_driver(&config_module,send_data));
   TEST_ASSERT_EQUAL(config_module.send_data_device,send_data);
   TEST_ASSERT_EQUAL_UINT(config_module.code_error,BG96_NO_ERROR);
}

void test_get_status_modem(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer,100,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ReturnArrayThruPtr_buffer(RS_BG96_OK ,7);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_modem(&config_module));
}

void test_get_status_sim(void)
{
   char buffer[10];
   char *p;
   send_data_ExpectAndReturn(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ReturnArrayThruPtr_buffer(RS_BG96_STATUS_SIM,25);
   parse_string_Expect(RS_BG96_STATUS_SIM,',','\r',p);
   parse_string_IgnoreArg_buffer();
   parse_string_ReturnArrayThruPtr_buffer("2",2);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_sim(&config_module));
   TEST_ASSERT_EQUAL(US_STATE_UNKNOWNU,config_sim.sim_state);
}

void test_set_mode_echo(void)
{  
   char buffer[30]={0};
   send_data_ExpectAndReturn(CMD_BG96_MODE_ECHO_OFF,RS_BG96_OK,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_mode_echo(&config_module,STATE_ECHO_ON));
   TEST_ASSERT_EQUAL(STATE_ECHO_ON,config_module.mode_echo);
}

void test_set_format_response(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn(CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_format_response(&config_module,LONG_RESULT));
   TEST_ASSERT_EQUAL(LONG_RESULT,config_module.format_response);
}

void test_set_format_error()
{
  char buffer[30]={0};
  send_data_ExpectAndReturn(CMD_TEST_BG96_FORMAT_ERROR,RS_BG96_OK,buffer,300,FT_BG96_OK);
  send_data_IgnoreArg_buffer();
  TEST_ASSERT_EQUAL(FT_BG96_OK,set_format_error(&config_module,ME_ENABLE_TEXT));
  TEST_ASSERT_EQUAL(ME_ENABLE_TEXT,config_module.format_error);
}

void test_set_sms_format(void)
{
  char buffer[30]={0};
  send_data_ExpectAndReturn(CMD_TEST_BG96_MODE_SMS,RS_BG96_OK,buffer,300,FT_BG96_OK);
  send_data_IgnoreArg_buffer();
  TEST_ASSERT_EQUAL(FT_BG96_OK,set_sms_format(&config_module,MODE_TEXT));
  TEST_ASSERT_EQUAL(MODE_TEXT,config_module.mode_sms);
}

void test_send_sms_bg96(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+CMGS=\"72950576\"\r",">",buffer,12000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ExpectAndReturn("HOLA\r",NULL,buffer,12000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ExpectAndReturn("\x1A\r",RS_BG96_OK,buffer,12000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,send_sms_bg96(&config_module,"72950576","HOLA"));
}

void test_set_parameters_context_tcp(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QICSGP=1,1,\"4g.entel\",\"\",\"\",1\r",RS_BG96_OK,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_parameter_context_tcp(&config_module,&config_context_tcp));
}

void test_activate_context_pdp(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QIACT=1\r",RS_BG96_OK,buffer,150000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,activate_context_pdp(&config_module,&config_context_tcp));
}

void test_desactivate_context_pdp(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QIDEACT=1\r",RS_BG96_OK,buffer,40000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,desactivate_context_pdp(&config_module,&config_context_tcp));
}

void test_set_parameters_MQTT(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QMTCFG=\"pdpid\",0,1\r",RS_BG96_OK,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_parameters_mqtt(&config_module,&config_parameters_mqtt,&config_context_tcp));
}

void test_open_client_mqtt(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QMTOPEN=0,\"industrial.api.ubidots.com\",1883\r",RS_BG96_CERO,buffer,75000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,open_client_mqtt(&config_module,&config_parameters_mqtt));
}

void test_close_client_mqtt(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QMTCLOSE=0\r",RS_BG96_CERO,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,close_client_mqtt(&config_module,&config_parameters_mqtt));
}

void test_connect_server_mqtt(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QMTCONN=0,\"123456789\",\"\",\"\"\r",RS_BG96_CERO,buffer,5000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,connect_server_mqtt(&config_module,&config_parameters_mqtt));
}
void test_disconnect_server_mqtt(void)
{
   char buffer[30]={0};
   send_data_ExpectAndReturn("AT+QMTDISC=0\r",RS_BG96_CERO,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,disconnect_server_mqtt(&config_module,&config_parameters_mqtt));
}
void test_publish_message(void)
{
   char buffer[30]={0};
   char topic[]="/v1.6/devices/demo";
   char data[]="{\"demo\":10,\"humedad\":60}";
   send_data_ExpectAndReturn("AT+QMTPUB=0,0,0,0,\"/v1.6/devices/demo\"\r",RS_BG96_SIGNAL,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ExpectAndReturn(data,NULL,buffer,300,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   send_data_ExpectAndReturn("\x1A\r",RS_BG96_CERO,buffer,15000,FT_BG96_OK);
   send_data_IgnoreArg_buffer();
   TEST_ASSERT_EQUAL(FT_BG96_OK,publish_message(&config_module,&config_parameters_mqtt,topic,data));
}