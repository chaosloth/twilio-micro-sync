/*============================================================
  Copyright (c) 2022 Christopher Connolly
  Project:     MicroSync
  Author:      Christopher Connolly
  Last update: 13.04.2022
  Description: Light weight Twilsock message parser Sync Client
  License:     MIT
  Dependencies & Kudos:
    - ArduionoJson
  ============================================================
  */

#include "MessageProcessor.h"
#include "TwilsockMessage.h"
#include <cstdlib>
#include <string>
#include "Parser.h"
#include "Utility.h"
#include "ArduinoJson-v6.19.4.h"

namespace twilio
{
    std::string token;
    FuncPtrString emitCallback;

    void MessageProcessor::registerOnMessage(FuncPtrString func)
    {
        emitCallback = func;
    }

    void MessageProcessor::prepareAndSend(std::string header)
    {
        twilio::ParseContext outCtx;
        outCtx.header = header;

        std::string protMsg = twilio::Parser::createMessage(outCtx);
        emitCallback(protMsg);
    }

    void MessageProcessor::prepareAndSend(std::string header, std::string payload)
    {
        twilio::ParseContext outCtx;
        outCtx.header = header;
        outCtx.msgPayload = payload;

        std::string protMsg = twilio::Parser::createMessage(outCtx);
        emitCallback(protMsg);
    }

    std::string MessageProcessor::genRandom(const int len)
    {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i)
        {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    }

    void MessageProcessor::setToken(std::string t)
    {
        token = t;
    }

    void MessageProcessor::processMessage(std::string &incoming)
    {
        // Create the context from incoming string
        twilio::ParseContext ctx(incoming);

        // Do the actual parsing
        twilio::MessagePtr msg = twilio::Parser::parse(ctx);

        printf("processMessage method: %s\n", msg->method.c_str());

        if (Utility::icomp(msg->method, MethodKey::PING))
        {
            std::string s = makeAckReply(msg->id);
            prepareAndSend(s);
        }
        else if (Utility::icomp(msg->method, MethodKey::REPLY))
        {
        }
        else if (Utility::icomp(msg->method, MethodKey::NOTIFICATION))
        {
            std::string s = makeAckReply(msg->id);
            prepareAndSend(s);
        }
        else if (Utility::icomp(msg->method, MethodKey::CLOSE))
        {
            std::string s = makeAckReply(msg->id);
            prepareAndSend(s);
        }
    }

    std::string MessageProcessor::makeAckReply(std::string ref)
    {
        DynamicJsonDocument doc(380);
        doc[Key::REQUEST_ID] = ref;
        doc[Key::METHOD] = MethodKey::REPLY;
        doc[Key::PAYLOAD_TYPE] = DEFAULT_PAYLOAD_TYPE;
        doc[Key::PAYLOAD_SIZE] = 0;

        JsonObject status = doc.createNestedObject(Key::STATUS);
        status[Key::STATUS_STATUS] = "OK";
        status[Key::STATUS_CODE] = 200;

        char buffer[192];
        serializeJson(doc, buffer);
        return buffer;
    }

    std::string MessageProcessor::makeInit()
    {
        DynamicJsonDocument headerDoc(1800);
        headerDoc[Key::REQUEST_ID] = "TM" + genRandom(36);
        headerDoc[Key::METHOD] = MethodKey::INIT;
        headerDoc[Key::TOKEN] = token;
        headerDoc[Key::CONTINUATION_TOKEN] = nullptr;
        headerDoc[Key::PAYLOAD_SIZE] = 0;
        headerDoc[Key::TWEAKS] = nullptr;

        JsonObject meta = headerDoc.createNestedObject(Key::CLIENT_METADATA);
        meta[Key::ENVIRONMENT_NAME] = "ESP32";
        meta[Key::ENVIRONMENT_VERSION] = "1.0.0.";
        meta[Key::OS_NAME] = "rtos";
        meta[Key::OS_VERSION] = "1.0.0";
        meta[Key::OS_ARCHITECTURE] = "32";
        meta[Key::ACTIVE_GRANT] = "sync";
        meta[Key::SDK_NAME] = "Cpp";
        meta[Key::SDK_VERSION] = "1.0.0";

        JsonArray registrations = headerDoc.createNestedArray(Key::REGISTRATIONS);
        JsonObject rego = registrations.createNestedObject();
        rego[Key::INITREG_PRODUCT] = "data_sync";
        rego[Key::INITREG_TYPE] = "ers";
        rego[Key::INITREG_PROTOCOL_VERSION] = 0;
        JsonArray message_types = rego.createNestedArray(Key::INITREG_MESSAGE_TYPES);
        message_types.add("twilio.sync.event");
        message_types.add("com.twilio.rtd.cds.document");
        message_types.add("com.twilio.rtd.cds.list");
        message_types.add("com.twilio.rtd.cds.map");

        JsonArray capabilities = headerDoc.createNestedArray(Key::CAPABILITIES);
        capabilities.add(Capabilities::CLIENT_UPDATE);
        capabilities.add(Capabilities::TELEMETRY_V1);
        capabilities.add("offline_storage");

        char buffer[1700];
        serializeJson(headerDoc, buffer);

        return buffer;
    }
}