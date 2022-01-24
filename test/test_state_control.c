

#include "unity.h"
#include "state_control.h"
#include "mock_send_commands.h"


void test_probar_funcion_estado_del_modem(void)
{ 
    char buffer[2];
    char buffer2[3]="OK";
    bg96_SendCmdSingle_ExpectAndReturn(CMD_STATUS,"OK","ERROR",buffer,100,FT_BG96_OK);
    bg96_SendCmdSingle_IgnoreArg_md_Responde();
    bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CM_CMD_MOCK_GENERAL_RESPONSE,7);
    TEST_ASSERT_EQUAL(0,get_state_modem());
}   

void test_get_status_usim(void)
{
    char buffer[2];
    bg96_SendCmdSingle_ExpectAndReturn(CMD_STATUS_USIM ,"OK","ERROR",buffer,100,FT_BG96_OK);
    bg96_SendCmdSingle_IgnoreArg_md_Responde();
    bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_STATUS_USIM_RESPONSE_1,24);
    TEST_ASSERT_EQUAL(0,get_status_usim());

    bg96_SendCmdSingle_ExpectAndReturn(CMD_STATUS_USIM ,"OK","ERROR",buffer,100,FT_BG96_OK);
    bg96_SendCmdSingle_IgnoreArg_md_Responde();
    bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_STATUS_USIM_RESPONSE_2,24);
    TEST_ASSERT_EQUAL(1,get_status_usim());

    bg96_SendCmdSingle_ExpectAndReturn(CMD_STATUS_USIM ,"OK","ERROR",buffer,100,FT_BG96_OK);
    bg96_SendCmdSingle_IgnoreArg_md_Responde();
    bg96_SendCmdSingle_ReturnArrayThruPtr_md_Responde(CMD_STATUS_USIM_RESPONSE_3,24);
    TEST_ASSERT_EQUAL(2,get_status_usim());
   
}

