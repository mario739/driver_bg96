/**
 * @file application_mqtt.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


typedef enum
{
    INIT_SEND_DATA_MQTT,
    RESET_MODULE,
    CONFIG_PDP_CONTEXT,
    ACTIVATE_PDP_CONTEXT,
    OPEN_CONNECTION_MQTT,
    CONNECT_BROKER_MQTT,
    PUB_MQTT,
    DEACTIVATE_PDP_CONTEXT,
    DISCONNECT_BROKER_MQTT,
}en_states_send_data;

en_states_send_data states_send_data_mqtt=INIT_SEND_DATA_MQTT;
void send_data_mqtt();