
#include "io9205_coap_control.h"
#include "track_demo_module.h"

#define MEMSET(d, c, l) memset((void *)(d), (int32_t)(c), (int32_t)(l))

#define MAX_RESOURCE_NAME_LEN 63
#define COAP_MAX_BUFFER_LENGTH 2048
#define PRV_ADDRSTRLEN 48
#define AF_INET_V4 2 /* < Address family is IPv4. > */
#define TEST_TIME_ZONE 8
#define APP_TIME 3600 * 8

#define NULL_SERVER_IP "0.0.0.0" // 54.222.234.168
#define COAP_SERVER_IP "54.223.123.203"
#define LOCAL_TIME 1583892610
#define MARIN_TIME_TS 315964800
typedef struct lcd_time_s
{
    uint8_t hour_h;
    uint8_t hour_l;
    uint8_t min_h;
    uint8_t min_l;
    uint8_t sec_h;
    uint8_t sec_l;
    uint8_t month_h;
    uint8_t month_l;
    uint8_t day_h;
    uint8_t day_l;

    uint32_t year;
} lcd_time_t;

char *p_ip_info = NULL;
char *coap_server_ip = NULL;
qapi_Coap_Packet_t *app_coap_message = NULL;
qapi_Net_coap_Hndl_t app_coap_hndle;
qapi_Coap_Message_Params_t coap_message_param;
qapi_Coap_Session_Info_t session_param;
qapi_Coap_Connection_Cfg_t connection_params;
TX_BYTE_POOL *byte_pool_coap;
boolean coap_connection_status = FALSE;
char coap_uri_path_block[MAX_RESOURCE_NAME_LEN] = {0};
char *payload = NULL;
uint16_t coap_uri_path_block_len = 0;
uint16_t coap_block_size = 0;

int marin_msg = 0;
lcd_time_t now_time;

#define MAX_MSG_LEN 256
#define ALARM_MSG_LEN 92
#define GLANCE_MSG_LEN 96
#define LOC_MSG_LEN 164 // 190
#define TIME_MSG_LEN 24 + 1
char *time_payload = "323030333036313633333238";

// 服务器下发消息的回调
static int32_t coap_handle_session_cb(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Packet_t *messageP, void *clientData);

// 处理回复消息的函数
static qapi_Coap_Status_t observe_notify_handler_t(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Packet_t *message);

// 发送订阅和发送消息的回调
static void coap_handle_transaction_cbt(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Transaction_t *transaction, qapi_Coap_Packet_t *message);

void display_time_get(lcd_time_t *time, char time_zone);

static void process_options(qapi_Coap_Packet_t *messageP);

static void process_options(qapi_Coap_Packet_t *messageP)
{
    if (messageP == NULL)
    {
        LOG_INFO("Got Null coap packet in process_options\n");
        return;
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_IF_MATCH))
    {
        LOG_INFO("QAPI_COAP_OPTION_IF_MATCH is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_URI_HOST))
    {
        LOG_INFO("QAPI_COAP_OPTION_URI_HOST is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_ETAG))
    {
        LOG_INFO("QAPI_COAP_OPTION_ETAG is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_IF_NONE_MATCH))
    {
        LOG_INFO("QAPI_COAP_OPTION_IF_NONE_MATCH is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_OBSERVE))
    {
        LOG_INFO("QAPI_COAP_OPTION_OBSERVE is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_URI_PORT))
    {
        LOG_INFO("QAPI_COAP_OPTION_URI_PORT is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_LOCATION_PATH))
    {
        LOG_INFO("QAPI_COAP_OPTION_LOCATION_PATH is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_URI_PATH))
    {
        LOG_INFO("QAPI_COAP_OPTION_URI_PATH is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_MAX_AGE))
    {
        LOG_INFO("QAPI_COAP_OPTION_MAX_AGE is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_URI_QUERY))
    {
        LOG_INFO("QAPI_COAP_OPTION_URI_QUERY is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_ACCEPT))
    {
        LOG_INFO("QAPI_COAP_OPTION_ACCEPT is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_TOKEN))
    {
        LOG_INFO("QAPI_COAP_OPTION_TOKEN is Set\n");
    }

    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_LOCATION_QUERY))
    {
        LOG_INFO("QAPI_COAP_OPTION_LOCATION_QUERY is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_BLOCK2))
    {
        LOG_INFO("QAPI_COAP_OPTION_BLOCK2 is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_BLOCK1))
    {
        LOG_INFO("QAPI_COAP_OPTION_BLOCK1 is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_SIZE))
    {
        LOG_INFO("QAPI_COAP_OPTION_SIZE is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_PROXY_URI))
    {
        LOG_INFO("QAPI_COAP_OPTION_PROXY_URI is Set\n");
    }
    if (QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_SIZE1))
    {
        LOG_INFO("QAPI_COAP_OPTION_SIZE1 is Set\n");
    }
}

void app_coap_free(void *p)
{
    if (p)
    {
        tx_byte_release(p);
        p = NULL;
    }
}

qapi_Status_t app_coap_create_session()
{
    qapi_Status_t result = QAPI_OK;
    qapi_Coap_Connection_Cfg_t connection_params;
    LOG_INFO("app_coap_create_session call!\n");

    memset(&session_param, 0x00, sizeof(qapi_Coap_Session_Info_t));
    memset(&connection_params, 0x00, sizeof(qapi_Coap_Connection_Cfg_t));

    session_param.coap_ack_random_factor = /*1;*/ QAPI_COAP_ACK_RANDOM_FACTOR;
    session_param.coap_default_maxage = QAPI_COAP_DEFAULT_MAX_AGE;
    session_param.coap_max_latency = QAPI_COAP_MAX_LATENCY;
    session_param.coap_max_retransmits = /*1;*/ QAPI_COAP_MAX_RETRANSMIT;
    session_param.coap_transaction_timeout = QAPI_COAP_RESPONSE_TIMEOUT; // QAPI_COAP_RESPONSE_TIMEOUT
    session_param.clientData = NULL;
    session_param.cb = coap_handle_session_cb;

    if (app_coap_hndle != NULL)
    {
        LOG_INFO("create_session : Session on already exists \
                    with session_handle \n");
        return result;
    }

    if (qapi_Coap_Create_Session(&app_coap_hndle, &session_param) != QAPI_OK)
    {
        result = QAPI_ERROR;
    }

    if (app_coap_hndle == NULL)
    {
        result = QAPI_ERROR;
        LOG_INFO("create_session : Session on not created \n");
    }
    else
    {
        result = QAPI_OK;
    }

    return result;
}

qapi_Status_t app_coap_delete_session()
{
    qapi_Status_t result = QAPI_OK;

    if (coap_server_ip)
    {
        free(coap_server_ip);
        coap_server_ip = NULL;
    }

    if (app_coap_hndle != NULL)
    {
        if (coap_connection_status == TRUE)
        {
            LOG_INFO("closing connection before deleting session .....\n");
            if (qapi_Coap_Close_Connection(app_coap_hndle) != QAPI_OK)
            {
                result = QAPI_ERROR;
            }
            else
            {
                LOG_INFO("qapi_Coap_Close_Connection returned success\n");
                coap_connection_status = FALSE;
                result = QAPI_OK;
            }
        }

        if (qapi_Coap_Destroy_Session(app_coap_hndle) != QAPI_OK)
        {
            LOG_INFO("qapi_Coap_Destroy_Session ! = QAPI_OK\n");
            result = QAPI_ERROR;
        }
        else
        {
            app_coap_hndle = NULL;
            //result = QAPI_OK;
        }
    }
    else
        LOG_INFO("session handle  doesnot exists\n");

    return result;
}

qapi_Status_t app_coap_create_connection()
{
    qapi_Status_t result = QAPI_OK;

    LOG_INFO("app_coap_create_connection in\n");

    connection_params.sec_Mode = QAPI_COAP_MODE_NONE;
    connection_params.proto = QAPI_COAP_PROTOCOL_UDP;
    connection_params.callHndl = (uint32_t)g_t95_dss_hndl;
    connection_params.resumption_enabled = FALSE;
    connection_params.session_resumption_timeout = 0;
    connection_params.src_host = p_ip_info;
    connection_params.src_port = 5685;
    connection_params.dst_host = COAP_SERVER_IP;
    connection_params.dst_port = 5683;
    connection_params.family_type = AF_INET_V4;

    LOG_INFO("qapi_Coap_Create_Connection dst_host = %s \n",
             connection_params.dst_host);
    if (qapi_Coap_Create_Connection(app_coap_hndle,
                                    &connection_params) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Create_Connection ! = QAPI_OK\n");
        result = QAPI_ERROR;
    }
    else
    {
        result = QAPI_OK;
    }

    return result;
}

qapi_Status_t app_coap_close_connection()
{
    qapi_Status_t result = QAPI_OK;
    if (app_coap_hndle == NULL)
    {
        LOG_INFO("Coap_Moudle: qapi_Coap_Close_Connection app_coap_hndle is NULL\n");
        return result;
    }

    if (qapi_Coap_Close_Connection(app_coap_hndle) != QAPI_OK)
    {
        LOG_INFO("Coap_Moudle: qapi_Coap_Close_Connection ! = QAPI_OK\n");
        result = QAPI_ERROR;
    }
    else
    {
        result = QAPI_OK;
    }
    return result;
}

qapi_Status_t app_coap_init_message()
{
    qapi_Coap_Message_Type_t msg_type = QAPI_COAP_TYPE_CON;
    qapi_Coap_Method_t method_type = QAPI_COAP_POST;
    coap_message_param.msg_cb = coap_handle_transaction_cbt;

    LOG_INFO("app_coap_init_message call.\n");
    if (QAPI_ERROR == qapi_Coap_Init_Message(app_coap_hndle,
                                             &app_coap_message,
                                             msg_type,
                                             method_type))
    {
        LOG_INFO("qapi_Coap_Init_Message app_coap_message error\n");
        return QAPI_ERROR;
    }
    else
    {
        LOG_INFO("qapi_Coap_Init_Message app_coap_message ok \n");
    }
    return QAPI_OK;
}

qapi_Status_t app_coap_set_header()
{
    qapi_Coap_Header_type header_type = QAPI_COAP_URI_PATH;
    size_t val_len = 0;
    const void *header_val = NULL;

    //LOG_INFO("app_coap_set_header call.\n");
    header_val = "postData";
    val_len = 8;

    if (header_type == QAPI_COAP_URI_PATH)
        coap_uri_path_block_len = val_len;

    if (qapi_Coap_Set_Header(app_coap_hndle, app_coap_message,
                             header_type, header_val, val_len) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Set_Header set header failed.\n");
        return QAPI_ERROR;
    }

    strlcpy(coap_uri_path_block, header_val, coap_uri_path_block_len + 1);
    coap_uri_path_block[coap_uri_path_block_len + 1] = '\0';
    //LOG_INFO("qapi_Coap_Set_Header coap_uri_path_block end.\n");
    return QAPI_OK;
}

qapi_Status_t app_coap_send_test_message()
{
    char *payload = "test";
    size_t payload_len = 4;

    coap_message_param.lastmid = (uint16_t *)0;
    coap_message_param.token = (uint8_t *)"96dce18c"; // 96dce18c
    coap_message_param.token_len = 0;

    if (qapi_Coap_Set_Payload(app_coap_hndle, app_coap_message,
                              payload, payload_len) != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message error\n");
        return QAPI_ERROR;
    }

    if (qapi_Coap_Send_Message(app_coap_hndle, app_coap_message,
                               &coap_message_param) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Send_Message returned error\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

/********************************** func callback **********************************/

static qapi_Coap_Status_t observe_notify_handler_t(qapi_Coap_Session_Hdl_t handle, qapi_Coap_Packet_t *message)
{
    qapi_Coap_Packet_t *response = NULL;
    uint16_t send_mid = 0;
    qapi_Coap_Status_t ret = QAPI_NO_ERROR;
    qapi_Coap_Message_Type_t message_response_type = QAPI_COAP_TYPE_NON;

    if (handle == NULL || message == NULL)
    {
        LOG_INFO("observe_notify_handler_t passed parameter is NULL\n");
        ret = QAPI_COAP_IGNORE;
        return ret;
    }

    if (message->type == QAPI_COAP_TYPE_ACK)
    {
        if ((char *)message->payload != NULL && message->payload_len > 0)
        {
            LOG_INFO("observe_notify_handler called : Recieved Ack for Upinfo \n");
            LOG_INFO("observe_notify_handler payload %s len = %d\n ", (char *)message->payload, message->payload_len);
        }
        else
        {
            LOG_INFO("observe_notify_handler called : Recieved Ack for observe \n");
        }
    }
    else if (message->type == QAPI_COAP_TYPE_CON && message->code == QAPI_CONTENT_2_05)
    {

        LOG_INFO("observe_notify_handler : NON_CON : Notify type = %lu [0 CON 1 NON]\n", message->type);
        LOG_INFO("observe_notify_handler called : Recieved NON_CON with code = %d\n", message->code);
        LOG_INFO("observe_notify_handler : NON_CON : Notify mid = %lu\n", message->mid);
        LOG_INFO("observe_notify_handler : NON_CON : Notify observe_value = %lu\n", message->observe);
        LOG_INFO("observe_notify_handler : NON_CON : Notify max_age = %lu\n", message->max_age);

        message_response_type = QAPI_COAP_TYPE_ACK;
        LOG_INFO("observe_notify_handler : NON_CON : Notify message_response_type = %lu\n", message_response_type);

        if (qapi_Coap_Init_Message(handle, &response, message_response_type, 0) != QAPI_OK)
        {
            LOG_INFO("observe_notify_handler : NON_CON : qapi_Coap_Init_Message returned error");
            ret = QAPI_COAP_IGNORE;
            return ret;
        }

        send_mid = message->mid;
        coap_message_param.lastmid = &(send_mid);
        response->mid = send_mid;

        coap_message_param.token = message->token;         // 96dce18c
        coap_message_param.token_len = message->token_len; // 8

        if (qapi_Coap_Send_Message(handle, response, &coap_message_param) != QAPI_OK)
        {
            LOG_INFO("observe_notify_handler : NON_CON : qapi_Coap_Send_Message returned error");
            ret = QAPI_COAP_IGNORE;
        }
    }

    return ret;
}

int32_t coap_handle_session_cb(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Packet_t *messageP, void *clientData)
{
    qapi_Coap_Message_Type_t message_type = QAPI_COAP_TYPE_CON;
    qapi_Coap_Status_t coap_response_code = QAPI_NO_ERROR;

    if (hdl == NULL || messageP == NULL)
    {
        LOG_INFO("CPAP_MODULE: Passed Argument is NULL\n");
        coap_response_code = QAPI_COAP_IGNORE;
        return (int32_t)coap_response_code;
    }

    LOG_INFO("coap_handle_session_cb: Received coap packet from server : Processing Packet.....\n");

    message_type = messageP->type;
    LOG_INFO("coap_handle_session_cb: messageP->code = %d\n", messageP->code);
    LOG_INFO("coap_handle_session_cb: messageP->payload_len = %d\n", messageP->payload_len);
    LOG_INFO("coap_handle_session_cb: messageP->buffer_len = %d\n", messageP->buffer_len);
    LOG_INFO("coap_handle_session_cb: messageP->type = %d\n", messageP->type);
    LOG_INFO("coap_handle_session_cb: messageP->mid = %d\n", messageP->mid);

    if (0 == messageP->payload_len)
    {
        LOG_INFO("=====coap_handle_session_cb: receive the ping pkg!======\n");
        coap_response_code = QAPI_COAP_IGNORE;
        return (int32_t)coap_response_code;
    }

    if (messageP->payload != NULL)
    {
        LOG_INFO("coap_handle_session_cb:Payload in recieved response %s len = %d\n", (char *)messageP->payload, messageP->payload_len);
    }
    if (message_type == QAPI_COAP_TYPE_CON && QAPI_IS_OPTION(messageP, QAPI_COAP_OPTION_OBSERVE))
    {
        LOG_INFO("coap_handle_session_cb Got CON Notify , calling observe-notify handler\n");
        process_options(messageP);

        coap_response_code = observe_notify_handler_t(hdl, messageP);
        if (coap_response_code != QAPI_NO_ERROR)
        {
            LOG_INFO("coap_handle_session_cb : observe_notify_handler returned error with result %d\n", coap_response_code);
        }
    }

    return (int32_t)coap_response_code;
}

void coap_handle_transaction_cbt(qapi_Coap_Session_Hdl_t hdl, qapi_Coap_Transaction_t *transaction, qapi_Coap_Packet_t *message)
{
    qapi_Coap_Transaction_t *transact;
    qapi_Coap_Status_t result;
    qapi_Coap_Packet_t *coapMessage = NULL;
    char *payload = NULL;

    if ((hdl == NULL) || (transaction == NULL))
    {
        LOG_INFO("hdl == NULL) || (transaction == NULL)\n");
        return;
    }

    transact = (qapi_Coap_Transaction_t *)transaction;
    coapMessage = (qapi_Coap_Packet_t *)message;

    LOG_INFO("coap_handle_transaction_cbt Reply recevied from Server.\n");
    if (NULL != coapMessage)
    {
        LOG_INFO("coap_handle_transaction_cbt coapMessage not NULL.\n");
    }
    else
    {
        LOG_INFO("coap_handle_transaction_cbt coapMessage NULL.\n");
    }

    LOG_INFO("coap_handle_transaction_cbt end\n");
    return;

end:
    if (NULL != transaction->message)
    {
        qapi_Coap_Free_Message(hdl, transaction->message);
        transaction->message = NULL;
    }

    if (NULL != coapMessage)
        qapi_Coap_Free_Message(hdl, coapMessage);

    if (payload != NULL)
        free(payload);
}

/********************************** coap interface **********************************/

qapi_Status_t app_coap_create_message_func(void)
{
    LOG_INFO("app_coap_create_message_func call! ");
    if (app_coap_create_session() != QAPI_OK)
    {
        LOG_INFO("app_coap_create_message_func : Create Session failed!\n");
        return QAPI_ERROR;
    }

    // qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
    if (app_coap_create_connection() != QAPI_OK)
    {
        LOG_INFO("app_coap_create_message_func : Create Connection failed!\n");
        return QAPI_ERROR;
    }

    LOG_INFO("app_coap_create_message_func ok\n");

    return QAPI_OK;
}

qapi_Status_t app_coap_close_message_func(void)
{
    if (app_coap_close_connection() != QAPI_OK)
    {
        LOG_INFO("app_coap_close_message_func : Close Connection failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_delete_session() != QAPI_OK)
    {
        LOG_INFO("app_coap_close_message_func : Delte Session failed!\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

qapi_Status_t app_coap_send_test_message_func(void)
{
    if (app_coap_init_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message_func : set init failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_set_header() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message_func : set header failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_send_test_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message_func : send message failed!\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_alarm_message()
{
    char payload[MAX_MSG_LEN] = {0};
    unsigned char *payload1 = "AA72043335323839373130313239323433360231303025";
    unsigned char payload2[TIME_MSG_LEN] = {0};
    unsigned char *payload3 = "03423253503156322E3130";
    size_t payload_len = ALARM_MSG_LEN;
    // 20 03 06 163328 -> 323030333036313633333238
    marin_msg = 0;
    display_time_get(&now_time, TEST_TIME_ZONE);
    memset(payload2, 0, TIME_MSG_LEN);
    sprintf((char *)payload2, "3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d",
            now_time.year / 10 % 10, now_time.year % 10,
            now_time.month_h, now_time.month_l,
            now_time.day_h, now_time.day_l,
            now_time.hour_h, now_time.hour_l,
            now_time.min_h, now_time.min_l,
            now_time.sec_h, now_time.sec_l);
    LOG_INFO("app_coap_send_tracker_alarm_message : %s", payload2);

    memset(payload, 0, MAX_MSG_LEN);
    memcpy(payload + marin_msg, payload1, strlen(payload1));
    marin_msg += strlen(payload1);
    memcpy(payload + marin_msg, payload2, strlen(payload2));
    marin_msg += strlen(payload2);
    memcpy(payload + marin_msg, payload3, strlen(payload3));
    marin_msg += strlen(payload3);

    payload_len = strlen(payload);
    LOG_INFO("app_coap_send_tracker_alarm_message : %s\n", payload);
    LOG_INFO("app_coap_send_tracker_alarm_message payload_len : %d\n", payload_len);
    LOG_INFO("app_coap_send_tracker_alarm_message marin_msg : %d\n", marin_msg);

    coap_message_param.lastmid = (uint16_t *)0;
    coap_message_param.token = (uint8_t *)"96dce18c"; // 96dce18c
    coap_message_param.token_len = 0;

    if (qapi_Coap_Set_Payload(app_coap_hndle, app_coap_message,
                              payload, payload_len) != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message error\n");
        return QAPI_ERROR;
    }

    if (qapi_Coap_Send_Message(app_coap_hndle, app_coap_message,
                               &coap_message_param) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Send_Message returned error\n");
        return QAPI_ERROR;
    }

    LOG_INFO("app_coap_send_tracker_alarm_message end!\n");
    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_loc_message()
{
    char payload[MAX_MSG_LEN] = {0};
    char *payload1 = "AA72023335323839373130313239323433360235302500";
    char payload2[TIME_MSG_LEN] = {0};
    char *payload3 = "00423253503156322E31303131362E3337333034350033392E39383134373400333532383937313031323932343336";
    size_t payload_len = LOC_MSG_LEN;

    marin_msg = 0;
    display_time_get(&now_time, TEST_TIME_ZONE);
    memset(payload2, 0, TIME_MSG_LEN);
    sprintf((char *)payload2, "3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d",
            now_time.year / 10 % 10, now_time.year % 10,
            now_time.month_h, now_time.month_l,
            now_time.day_h, now_time.day_l,
            now_time.hour_h, now_time.hour_l,
            now_time.min_h, now_time.min_l,
            now_time.sec_h, now_time.sec_l);

    LOG_INFO("app_coap_send_tracker_loc_message : %s\n", payload2);

    memset(payload, 0, MAX_MSG_LEN);
    memcpy(payload + marin_msg, payload1, strlen(payload1));
    marin_msg += strlen(payload1);
    memcpy(payload + marin_msg, payload2, strlen(payload2));
    marin_msg += strlen(payload2);
    memcpy(payload + marin_msg, payload3, strlen(payload3));
    marin_msg += strlen(payload3);
    payload_len = strlen(payload);

    LOG_INFO("app_coap_send_tracker_loc_message : %s\n", payload);
    LOG_INFO("app_coap_send_tracker_loc_message payload_len : %d\n", payload_len);
    LOG_INFO("app_coap_send_tracker_loc_message marin_msg : %d\n", marin_msg);

    coap_message_param.lastmid = (uint16_t *)0;
    coap_message_param.token = (uint8_t *)"96dce18c"; // 96dce18c
    coap_message_param.token_len = 0;
    if (qapi_Coap_Set_Payload(app_coap_hndle, app_coap_message,
                              payload, payload_len) != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message error\n");
        return QAPI_ERROR;
    }

    if (qapi_Coap_Send_Message(app_coap_hndle, app_coap_message,
                               &coap_message_param) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Send_Message returned error\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_glance_message()
{
    char payload[MAX_MSG_LEN] = {0};
    unsigned char *payload1 = "AA72053836333730333033303332383932310135302500";
    unsigned char payload2[TIME_MSG_LEN] = {0};
    unsigned char *payload3 = "323439475053547261636B6572";
    size_t payload_len = GLANCE_MSG_LEN;
    // 20 03 06 163328 -> 323030333036313633333238
    marin_msg = 0;
    display_time_get(&now_time, TEST_TIME_ZONE);
    memset(payload2, 0, TIME_MSG_LEN);
    sprintf((char *)payload2, "3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d3%d",
            now_time.year / 10 % 10, now_time.year % 10,
            now_time.month_h, now_time.month_l,
            now_time.day_h, now_time.day_l,
            now_time.hour_h, now_time.hour_l,
            now_time.min_h, now_time.min_l,
            now_time.sec_h, now_time.sec_l);
    LOG_INFO("app_coap_send_tracker_glance_message : %s", payload2);

    memset(payload, 0, MAX_MSG_LEN);
    memcpy(payload + marin_msg, payload1, strlen(payload1));
    marin_msg += strlen(payload1);
    memcpy(payload + marin_msg, payload2, strlen(payload2));
    marin_msg += strlen(payload2);
    memcpy(payload + marin_msg, payload3, strlen(payload3));
    marin_msg += strlen(payload3);

    payload_len = strlen(payload);
    LOG_INFO("app_coap_send_tracker_glance_message : %s\n", payload);
    LOG_INFO("app_coap_send_tracker_glance_message payload_len : %d\n", payload_len);
    LOG_INFO("app_coap_send_tracker_glance_message marin_msg : %d\n", marin_msg);

    coap_message_param.lastmid = (uint16_t *)0;
    coap_message_param.token = (uint8_t *)"96dce18c"; // 96dce18c
    coap_message_param.token_len = 0;

    if (qapi_Coap_Set_Payload(app_coap_hndle, app_coap_message,
                              payload, payload_len) != QAPI_OK)
    {
        LOG_INFO("app_coap_send_test_message error\n");
        return QAPI_ERROR;
    }

    if (qapi_Coap_Send_Message(app_coap_hndle, app_coap_message,
                               &coap_message_param) != QAPI_OK)
    {
        LOG_INFO("qapi_Coap_Send_Message returned error\n");
        return QAPI_ERROR;
    }

    LOG_INFO("app_coap_send_tracker_glance_message end!\n");
    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_alarm_message_func(void)
{
    LOG_INFO("app_coap_send_tracker_alarm_message_func call!\n");
    if (app_coap_init_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_alarm_message_func : set init failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_set_header() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_alarm_message_func : set header failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_send_tracker_alarm_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_alarm_message_func : send message failed!\n");
        return QAPI_ERROR;
    }

    LOG_INFO("app_coap_send_tracker_alarm_message_func : send message over!\n");

    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_loc_message_func(void)
{
    if (app_coap_init_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_loc_message_func : set init failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_set_header() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_loc_message_func : set header failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_send_tracker_loc_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_loc_message_func : send message failed!\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

qapi_Status_t app_coap_send_tracker_glance_message_func(void)
{
    if (app_coap_init_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_glance_message_func : set init failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_set_header() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_glance_message_func : set header failed!\n");
        return QAPI_ERROR;
    }

    if (app_coap_send_tracker_glance_message() != QAPI_OK)
    {
        LOG_INFO("app_coap_send_tracker_glance_message_func : send message failed!\n");
        return QAPI_ERROR;
    }

    return QAPI_OK;
}

void display_time_get(lcd_time_t *time, char time_zone)
{
    const char norMoth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char leapMoth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const short day_per_year[] = {366, 365, 365, 365};
    const char *pMoth = norMoth;
    uint8_t new_hour = 0, new_min = 0, new_mouth = 0, temp_year = 0;
    uint32_t new_year, time_sec = 0, new_day = 0;
    uint64_t u64_timestamp = 0;
    // 1980-1-6 0:0:0
    qapi_time_get_t current_time;
    qapi_time_get(QAPI_TIME_SECS, &current_time);
    u64_timestamp = (uint64_t)current_time.time_secs;
    LOG_INFO("u64_timestamp : %llu!\n", u64_timestamp);

    time_sec = u64_timestamp + time_zone * 3600;
    new_day = time_sec / (24 * 3600) + 5;
    new_year = 1980 + new_day / (365 * 3 + 366) * 4;
    new_day = new_day % (365 * 3 + 366) + 1;

    while (new_day > day_per_year[temp_year])
    {
        new_day = new_day - day_per_year[temp_year];
        new_year++;
        temp_year++;
    }

    if (new_year % 4 == 0)
    {
        if ((new_year % 100 == 0) && (new_year % 400 != 0))
        {
            pMoth = norMoth;
        }
        else
        {
            pMoth = leapMoth;
        }
    }
    while (new_day > *(pMoth + new_mouth))
    {
        new_day = new_day - *(pMoth + new_mouth);
        new_mouth++;
    }
    new_mouth++;

    new_hour = (time_sec / 3600) % 24;
    new_min = (time_sec / 60) % 60;
    // QCLI_Printf(qcli_misc_cli_app_handle, "%d/%02d/%02d\n",new_year,new_mouth,new_day);
    // QCLI_Printf(qcli_misc_cli_app_handle, "%02d:%02d:%02d\n",new_hour,new_min,time_sec%60);

    time->hour_h = new_hour / 10;
    time->hour_l = new_hour % 10;
    time->min_h = new_min / 10;
    time->min_l = new_min % 10;
    time->sec_h = time_sec % 60 / 10;
    time->sec_l = time_sec % 60 % 10;
    time->month_h = new_mouth / 10;
    time->month_l = new_mouth % 10;
    time->day_h = new_day / 10;
    time->day_l = new_day % 10;

    time->year = new_year;
}

int t95_coap_init(void)
{
    int ret = 0;
    LOG_INFO("t95_coap_init ok! ");
    ret = app_coap_create_message_func();

    return ret;
}
