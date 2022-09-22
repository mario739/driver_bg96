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
#include "mock_functions.h"
#include "commands.h"


st_bg96_config config_module={.send_data_device=NULL,
                              .last_error=BG96_NO_ERROR,
                              .self_tcp={ .context_id=1,
                                          .context_type=1,
                                          .method_authentication=1,
                                          .tcp_apn="4g.entel",
                                          .tcp_password="",
                                          .tcp_username=""},
                              .self_mqtt={.identifier_socket_mqtt=0,
                                          .quality_service=0,
                                          .host_name="\"industrial.api.ubidots.com\"",
                                          .port=1883,
                                          .mqtt_client_id="123456789",
                                          .mqtt_username="",
                                          .mqtt_password=""}};

void test_init_driver(void)
{     
   TEST_ASSERT_EQUAL(FT_BG96_OK,init_driver(&config_module,send_data));
   TEST_ASSERT_EQUAL(config_module.send_data_device,send_data);
   TEST_ASSERT_EQUAL_UINT(config_module.last_error,BG96_NO_ERROR);
}

void test_get_status_modem(void)
{
   char buffer_resp[20]={0};
   send_data_ExpectAndReturn(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer_resp,1000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_modem(&config_module));

   send_data_ExpectAndReturn(CMD_BG96_STATUS_MODEM,RS_BG96_OK,buffer_resp,1000,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,get_status_modem(&config_module));
}

void test_get_status_sim(void)
{
   char buffer_resp[50]={0};
   send_data_ExpectAndReturn(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer_resp,5000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_sim(&config_module));

   send_data_ExpectAndReturn(CMD_BG96_STATUS_SIM,RS_BG96_OK,buffer_resp,5000,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,get_status_sim(&config_module));

   
}

void test_get_info_product(void)
{
   char buffer_resp[100]={0};
   send_data_ExpectAndReturn(CMD_BG96_GET_INFO_PRODUCT,RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_information_product(&config_module));

   send_data_ExpectAndReturn(CMD_BG96_GET_INFO_PRODUCT,RS_BG96_OK,buffer_resp,300,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,get_information_product(&config_module));

}

void test_get_status_pdp_context(void)
{
   char buffer_resp[100]={0};
   send_data_ExpectAndReturn(CMD_BG96_STATUS_PDP_CONTEXT,RS_BG96_OK,buffer_resp,1000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_pdp_context(&config_module));

   send_data_ExpectAndReturn(CMD_BG96_STATUS_PDP_CONTEXT,RS_BG96_OK,buffer_resp,1000,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,get_status_pdp_context(&config_module));

}

void test_set_format_response(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn(CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_format_response(&config_module,1));

   send_data_ExpectAndReturn(CMD_BG96_MODE_RESPONSE_LONG,RS_BG96_OK,buffer_resp,300,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,set_format_response(&config_module,1));
   TEST_ASSERT_EQUAL(BG96_ERROR_SET_FORMAT_RESPONSE,config_module.last_error);
}

void test_set_format_error()
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn(CMD_TEST_BG96_FORMAT_ERROR,RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_format_error(&config_module,2));

   send_data_ExpectAndReturn(CMD_TEST_BG96_FORMAT_ERROR,RS_BG96_OK,buffer_resp,300,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,set_format_error(&config_module,2));
   
   TEST_ASSERT_EQUAL(BG96_ERROR_SET_MODE_ERROR ,config_module.last_error);
}

void test_set_sms_format(void)
{
  char buffer_resp[30]={0};
  send_data_ExpectAndReturn(CMD_TEST_BG96_MODE_SMS,RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
  TEST_ASSERT_EQUAL(FT_BG96_OK,set_sms_format(&config_module,1));

  send_data_ExpectAndReturn(CMD_TEST_BG96_MODE_SMS,RS_BG96_OK,buffer_resp,300,FT_BG96_ERROR);
  TEST_ASSERT_EQUAL(FT_BG96_ERROR,set_sms_format(&config_module,1));
  
}

void test_send_sms_bg96(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+CMGS=\"72950576\"\r",RS_BG96_SIGNAL,buffer_resp,12000,FT_BG96_OK);
   send_data_ExpectAndReturn("HOLA\x1a\r",RS_BG96_OK,buffer_resp,12000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,send_sms_bg96(&config_module,"72950576","HOLA"));

   send_data_ExpectAndReturn("AT+CMGS=\"72950576\"\r",RS_BG96_SIGNAL,buffer_resp,12000,FT_BG96_ERROR);
   TEST_ASSERT_EQUAL(FT_BG96_ERROR,send_sms_bg96(&config_module,"72950576","HOLA"));

}

void test_set_parameters_context_tcp(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QICSGP=1,1,\"4g.entel\",\"\",\"\",1\r",RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_parameter_context_tcp(&config_module));
}

void test_activate_context_pdp(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QIACT=1\r",RS_BG96_OK,buffer_resp,150000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,activate_context_pdp(&config_module));
}

void test_desactivate_context_pdp(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QIDEACT=1\r",RS_BG96_OK,buffer_resp,40000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,desactivate_context_pdp(&config_module));
}

void test_set_parameters_MQTT(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QMTCFG=\"pdpid\",0,1\r",RS_BG96_OK,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,set_parameters_mqtt(&config_module));
}

void test_open_client_mqtt(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QMTOPEN=0,\"industrial.api.ubidots.com\",1883\r",RS_BG96_CERO,buffer_resp,75000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,open_client_mqtt(&config_module));
}

void test_close_client_mqtt(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QMTCLOSE=0\r",RS_BG96_CERO,buffer_resp,300,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,close_client_mqtt(&config_module));
}

void test_connect_server_mqtt(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QMTCONN=0,\"123456789\",\"\",\"\"\r",RS_BG96_CERO,buffer_resp,5000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,connect_server_mqtt(&config_module));
}

void test_disconnect_server_mqtt(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QMTDISC=0\r",RS_BG96_CERO,buffer_resp,5000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,disconnect_server_mqtt(&config_module));
}

void test_publish_message(void)
{
   char buffer_resp[30]={0};
   char topic[19]="/v1.6/devices/demo";
   char data[25]="{\"demo\":10,\"humedad\":60}";
   send_data_ExpectAndReturn("AT+QMTPUB=0,0,0,0,\"/v1.6/devices/demo\"\r",RS_BG96_SIGNAL,buffer_resp,300,FT_BG96_OK);
   send_data_ExpectAndReturn("{\"demo\":10,\"humedad\":60}\x1a\r",RS_BG96_CERO,buffer_resp,15000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,publish_message(&config_module,topic,data));
}

void test_turn_off_bg96(void)
{
   char buffer_resp[30]={0};
   send_data_ExpectAndReturn("AT+QPOWD\r",RS_BG96_OK,buffer_resp,100,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,turn_off_bg96(&config_module));    
}

void test_state_machine_send_data(void)
{
   char buffer_resp[30]={0};
   char topic[19]="/v1.6/devices/demo";
   char data[25]="{\"demo\":10,\"humedad\":60}";
   send_data_ExpectAndReturn("AT+QMTOPEN=0,\"industrial.api.ubidots.com\",1883\r",RS_BG96_CERO,buffer_resp,75000,FT_BG96_OK);
   send_data_ExpectAndReturn("AT+QMTCONN=0,\"123456789\",\"\",\"\"\r",RS_BG96_CERO,buffer_resp,5000,FT_BG96_OK);
   send_data_ExpectAndReturn("AT+QMTPUB=0,0,0,0,\"/v1.6/devices/demo\"\r",RS_BG96_SIGNAL,buffer_resp,300,FT_BG96_OK);
   send_data_ExpectAndReturn("{\"demo\":10,\"humedad\":60}\x1a\r",RS_BG96_CERO,buffer_resp,15000,FT_BG96_OK);
   send_data_ExpectAndReturn("AT+QMTDISC=0\r",RS_BG96_CERO,buffer_resp,5000,FT_BG96_OK);
   TEST_ASSERT_EQUAL(FT_BG96_OK,send_data_mqtt(&config_module,topic,data));
}

