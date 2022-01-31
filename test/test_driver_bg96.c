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

st_bg96_config config_module={.send_data_device=NULL,.mode_echo=STATE_ECHO_OFF,.format_response=SHORT_RESULT};
st_config_sim config_sim={.sim_state=US_STATE_UNKNOWNU};

void test_init_driver(void)
{   
   TEST_ASSERT_EQUAL(FT_BG96_OK,init_driver(&config_module,send_data));
   TEST_ASSERT_EQUAL(config_module.send_data_device,send_data);
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
   TEST_ASSERT_EQUAL(FT_BG96_OK,get_status_sim(&config_module,&config_sim));
   TEST_ASSERT_EQUAL(US_STATE_UNKNOWNU,config_sim.sim_state);
}

void test_set_mode_echo(void)
{  
   char buffer[30]={0};
   send_data_ExpectAndReturn(CMD_BG96_MODE_ECHO_ON,RS_BG96_OK,buffer,300,FT_BG96_OK);
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