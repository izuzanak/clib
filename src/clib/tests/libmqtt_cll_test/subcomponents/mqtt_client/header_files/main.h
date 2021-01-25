
#ifndef __MAIN_H
#define __MAIN_H

@begin
include "cl_test.h"
include "cl_mqtt.h"
@end

// - error codes -
#define ERROR_MQTT_CLIENT_CREATE_ERROR 1
#define ERROR_MQTT_CLIENT_EPOLL_ERROR 2
#define ERROR_MQTT_CLIENT_PUBLISH_ERROR 3
#define ERROR_MQTT_CLIENT_SUBSCRIBE_ERROR 4
#define ERROR_MQTT_CLIENT_UNSUBSCRIBE_ERROR 5

// === definition of generated structures ======================================

// -- mqtt_client_s --
@begin
struct
<
mqtt_conn_s:connection
bc_array_s:buffer
>
mqtt_client_s;
@end

WUR int mqtt_client_s_create(mqtt_client_s *this,
    const char *a_client_id,const char *a_server_ip,usi a_server_port);
WUR int mqtt_client_s_run(mqtt_client_s *this);
WUR int mqtt_client_s_mqtt_callback(void *a_mqtt_client,unsigned a_index,unsigned a_type,va_list a_ap);

// === inline methods of generated structures ==================================

// -- mqtt_client_s --
@begin
inlines mqtt_client_s
@end

// === definition of global functions ==========================================

void signal_handler(int a_signum);

// === program entry function ==================================================

int main(int argc,char **argv);

#endif

