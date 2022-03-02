/**
 * @file application_mqtt.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "driver_bg96.h"
#include "application_mqtt.h"




void send_data_mqtt(st_bg96_config *obj,st_config_context_tcp *obj_tcp)
{
    while (1)
    {
        switch (states_send_data_mqtt)
        {
        case INIT_SEND_DATA_MQTT:
            if (get_status_modem(obj)==FT_BG96_OK)
            {
                if (get_status_sim(obj,))
                {
                    states_send_data_mqtt=CONFIG_PDP_CONTEXT;
                }
            }
            break;
        case CONFIG_PDP_CONTEXT:
            if (set_parameter_context_tcp(obj,obj_tcp)==FT_BG96_OK)
            {
               states_send_data_mqtt=ACTIVATE_PDP_CONTEXT;
            }
            break;
        case ACTIVATE_PDP_CONTEXT:
            if (activate_context_pdp(obj,obj_tcp)==FT_BG96_OK)
            {
               states_send_data_mqtt=OPEN_CONNECTION_MQTT; 
            }
            else
            { 
               states_send_data_mqtt=DEACTIVATE_PDP_CONTEXT;        
            }
            break;
        case OPEN_CONNECTION_MQTT:
            if (/* condition */)
            {
                /* code */
            }
            
           
        default:
            break;
        }   
    }   
}
