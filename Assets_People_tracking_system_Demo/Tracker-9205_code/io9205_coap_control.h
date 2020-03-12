#ifndef __IO_9205_COAP_H__
#define __IO_9205_COAP_H__
#include <string.h>
#include "qapi_cli.h"
#include "qapi_coap.h"
#include "qapi_status.h"
#include "qapi_dss.h"
#include "qapi_types.h"
#include "qapi_timer.h"
#include "io9205_timers.h"

#include "tx_api.h"
#include "stringl.h"

#define COAP_HEART_TIME 60 // 60
#define COAP_CLOSE_FLAG 1

typedef void *qapi_Net_coap_Hndl_t;
extern qapi_DSS_Hndl_t dss_hndl;
extern qapi_Net_coap_Hndl_t app_coap_hndle;
extern qapi_Coap_Packet_t *app_coap_message;
extern char *coap_server_ip;

qapi_Status_t app_coap_create_session(void);
qapi_Status_t app_coap_delete_session(void);
qapi_Status_t app_coap_create_connection(void);
qapi_Status_t app_coap_close_connection(void);
qapi_Status_t app_coap_init_message(void);
qapi_Status_t app_coap_set_header(void);

// Clinet upinfo test msg to Server
qapi_Status_t app_coap_send_test_message(void);

// create coap session and connection
qapi_Status_t app_coap_create_message_func(void);

// create coap session and connection
qapi_Status_t app_coap_close_message_func(void);
/************************* coap interface *************************/
qapi_Status_t app_coap_send_tracker_loc_message_func(void);
qapi_Status_t app_coap_send_tracker_alarm_message_func(void);
qapi_Status_t app_coap_send_test_message_func(void);

int t95_coap_init(void);

#endif
