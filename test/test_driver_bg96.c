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
#include "mock_send_commands.h"

void test_set_sms_format(void)
{
   st_bg96_data_sms data;
   st_bg96_data_sms data2={.sms_mode=3};
   char buffer[30];
   
   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGF=0\r","OK","ERROR",buffer,300,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_RESPONDE_SMS,17);

   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGF?","OK","ERROR",buffer,300,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_RESPONDE_SMS,17);

   TEST_ASSERT_EQUAL(FT_BG96_OK,set_sms_format(&data,MODE_PDU));
   TEST_ASSERT_EQUAL_UINT8(0,data.sms_mode);
   
   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGF=1\r","OK","ERROR",buffer,300,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_RESPONDE_SMS2,17);

   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGF?","OK","ERROR",buffer,300,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_RESPONDE_SMS2,17);

   TEST_ASSERT_EQUAL(FT_BG96_OK,set_sms_format(&data,MODE_TEXT));
   TEST_ASSERT_EQUAL_UINT8(1,data.sms_mode);

   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGF=0\r","OK","ERROR",buffer,300,FT_BG96_ERROR);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_RESPONDE_SMS,17);

   TEST_ASSERT_EQUAL(FT_BG96_ERROR,set_sms_format(&data2,MODE_PDU));
   TEST_ASSERT_EQUAL_UINT8(3,data2.sms_mode);

}

void test_send_sms(void)
{
   char buffer[100];
   bg96_SendCmdSingle_ExpectAndReturn("AT+CMGS=\"59172950576\"\r",">","ERROR",buffer,120,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(">",2);

   bg96_SendCmdSingle_ExpectAndReturn("hola que haces\r","OK","ERROR",buffer,120,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde("OK",3);

   bg96_SendCmdSingle_ExpectAndReturn("\x1A\r","OK","ERROR",buffer,120,FT_BG96_OK);
   bg96_SendCmdSingle_IgnoreArg_md_Responde();
   bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde("OK",3);

   TEST_ASSERT_EQUAL(FT_BG96_OK,send_sms("59172950576","hola que haces"));
}