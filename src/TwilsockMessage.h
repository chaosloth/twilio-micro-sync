#pragma once
#include <string>

namespace twilio
{
    namespace MethodKey
    {
        static const std::string INIT = "init";
        static const std::string UPDATE = "update";
        static const std::string PING = "ping";
        static const std::string CLOSE = "close";
        static const std::string REPLY = "reply";
        static const std::string NOTIFICATION = "notification";
        static const std::string MESSAGE = "message";
        static const std::string CLIENT_UPDATE = "client_update";
        static const std::string TELEMETRY_V1 = "telemetry.v1";
    }

    // Reusable JSON payload key names
    namespace Key
    {
        static const std::string HTTP_STATUS = "http_status";
        static const std::string STATUS = "status";
        static const std::string STATUS_CODE = "code";
        static const std::string STATUS_STATUS = "status";
        static const std::string STATUS_ERROR_CODE = "errorCode";
        static const std::string METHOD = "method";
        static const std::string REQUEST_ID = "id";
        static const std::string PAYLOAD_SIZE = "payload_size";
        static const std::string PAYLOAD_TYPE = "payload_type";
        static const std::string TOKEN = "token";
        static const std::string CONTINUATION_TOKEN = "continuation_token";
        static const std::string CONTINUATION_TOKEN_STATUS = "continuation_token_status";
        static const std::string REISSUED = "reissued";
        static const std::string REISSUE_REASON = "reissue_reason";
        static const std::string REISSUE_MESSAGE = "reissue_message";
        static const std::string ACTIVE_GRANT = "active_grant";
        static const std::string HTTP_HEADERS = "http_headers";
        static const std::string MESSAGE_TYPE = "message_type";
        static const std::string NOTIFICATION_CTX_ID = "notification_ctx_id";
        static const std::string CLIENT_UPDATE_TYPE = "client_update_type";
        // HTTP upstream request with additional params
        static const std::string HTTP_REQUEST = "http_request";
        static const std::string HTTP_REQUEST_HOST = "host";
        static const std::string HTTP_REQUEST_PATH = "path";
        static const std::string HTTP_REQUEST_PARAMS = "params";
        static const std::string HTTP_REQUEST_HEADERS = "headers";
        // Client metadata v3
        // See https://docs.google.com/document/d/14kGVPnteNuVYHRICOZ0UpO9S0Pn7YvNGPmSbWW7hvOo/edit#
        static const std::string CLIENT_METADATA = "metadata";
        // Runtime information
        static const std::string ENVIRONMENT_NAME = "env";
        static const std::string ENVIRONMENT_VERSION = "envv";
        static const std::string OS_NAME = "os";
        static const std::string OS_VERSION = "osv";
        static const std::string OS_ARCHITECTURE = "osa";
        static const std::string DEVICE_MODEL = "dev";
        static const std::string DEVICE_VENDOR = "devv";
        static const std::string DEVICE_TYPE = "devt";
        // Build-time information
        static const std::string SDK_NAME = "sdk";
        static const std::string SDK_VERSION = "sdkv";
        static const std::string SDK_TYPE = "type";
        static const std::string APPLICATION_NAME = "app";
        static const std::string APPLICATION_VERSION = "appv";
        static const std::string CAPABILITIES = "capabilities";
        static const std::string TWEAKS = "tweaks";
        static const std::string TWEAK_KEY = "tweak_key";
        static const std::string REGISTRATIONS = "registrations";           // Init message
        static const std::string INIT_REGISTRATIONS = "init_registrations"; // init Reply message
        static const std::string INITREG_PRODUCT = "product";
        static const std::string INITREG_TYPE = "type";
        static const std::string INITREG_PROTOCOL_VERSION = "notification_protocol_version";
        static const std::string INITREG_MESSAGE_TYPES = "message_types";
        static const std::string TELEMETRY_EVENTS = "events";
        static const std::string TELEMETRY_START = "start";
        static const std::string TELEMETRY_END = "end";
        static const std::string TELEMETRY_TITLE = "title";
        static const std::string TELEMETRY_DETAILS = "details";
        static const std::string TELEMETRY_ID = "id";
        static const std::string TELEMETRY_TYPE = "type";
    }

    static const std::string PROTOCOL_NAME = "TWILSOCK";
    static const std::string PROTOCOL_VERSION = "V3.0";
    static const std::string DEFAULT_PAYLOAD_TYPE = "application/json";

    namespace Capabilities
    {
        static const std::string CLIENT_UPDATE = MethodKey::CLIENT_UPDATE;
        static const std::string TELEMETRY_V1 = MethodKey::TELEMETRY_V1;
    }

    static const std::string tokenReissueReasonStrings[] =
        {
            "MISSING",
            "EXPIRED",
            "MALFORMED",
            "NOT_SUPPORTED_FORMAT",
            "DIFFERENT_IDENTITY",
            "DIFFERENT_GRANTS",
            "WRONG_SIGNATURE",
            "UNKNOWN"};
}
