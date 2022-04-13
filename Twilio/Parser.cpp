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

#include "Parser.h"

namespace twilio
{

    [[noreturn]] inline void reportParseError(MessagePtr &ctx, std::string const &message)
    {
        throw std::runtime_error("Parser Error: " + message);
    }

    const bool Parser::icomp(std::string &s1, const std::string &s2)
    {
        return (strcasecmp(s1.c_str(), s2.c_str()) == 0);
    }

    void Parser::printJsonObject(JsonObject obj, std::string indent)
    {
        for (JsonPair p : obj)
        {
            if (p.value().is<const char *>())
            {
                // Yes!
                // We can get the value via implicit cast:
                const char *s = p.value();
                // Or, via explicit method call:
                // auto v = p.value().as<const char *>();
                printf("%s%s: %s\n", indent.c_str(), p.key().c_str(), s);
            }
            else if (p.value().is<long>())
            {
                const long l = p.value();
                printf("%s%s: %lu\n", indent.c_str(), p.key().c_str(), l);
            }
            else
            {
                printf("%s%s:\n", indent.c_str(), p.key().c_str());
                printJsonObject(p.value(), indent + " ");
            }
        }
    }

    std::string Parser::createMessage(ParseContext &ctx)
    {
        MessagePtr msg = std::make_shared<ParseContext>(ctx);
        msg->protocol = PROTOCOL_NAME;
        msg->version = PROTOCOL_VERSION;

        std::string protoMsg;

        protoMsg = msg->protocol + " " + msg->version;
        protoMsg = protoMsg + " " + std::to_string(ctx.header.length());
        protoMsg = protoMsg + "\r\n";
        protoMsg = protoMsg + ctx.header;
        protoMsg = protoMsg + "\r\n";
        protoMsg = protoMsg + ctx.msgPayload;

        return protoMsg;
    }

    MessagePtr Parser::parse(ParseContext &ctx)
    {
        MessagePtr msg = std::make_shared<ParseContext>(ctx);
        processFrame(msg);
        return msg;
    }

    void Parser::processFrame(MessagePtr &ctx)
    {
        auto frame = ctx->frame;
        std::size_t end_of_protocol = frame.find("\r");

        if (end_of_protocol == std::string::npos)
        {
            return reportParseError(ctx, "Can not parse incoming message. New line symbol is not found.");
        }

        auto protocolLine = frame.substr(0, end_of_protocol);
        auto wsPos = std::string::npos;
        auto protocol = protocolLine.substr(0, (wsPos = protocolLine.find(' ')));
        protocolLine = protocolLine.substr(wsPos + 1);
        auto version = protocolLine.substr(0, (wsPos = protocolLine.find(' ')));
        // auto headerSize = std::stoi(protocolLine.substr(wsPos + 1));
        auto headerSize = std::atoi(protocolLine.substr(wsPos + 1).c_str());

        ctx->protocol = protocol;
        ctx->version = version;

        if (!icomp(protocol, PROTOCOL_NAME))
        {
            return reportParseError(ctx, "Can not parse incoming message. Protocol name is incorrect.");
        }

        if (!icomp(version, PROTOCOL_VERSION))
        {
            return reportParseError(ctx, "Can not parse incoming message. Version is incorrect.");
        }

        if (headerSize == 0)
        {
            return reportParseError(ctx, "Can not parse incoming message. Header size is incorrect.");
        }

        long remains = frame.length() - end_of_protocol;

        if (remains < headerSize)
        {
            return reportParseError(ctx, "Can not parse incoming message. Header size is larger than data.");
        }

        auto header = frame.substr(end_of_protocol + 2, headerSize);
        ctx->header = header;

        DynamicJsonDocument headerJsonDoc(2048);

        if (deserializeJson(headerJsonDoc, header.c_str()) != DeserializationError::Ok)
        {
            return reportParseError(ctx, "Cannot parse header into json doc");
        };

        if (!headerJsonDoc.containsKey("method"))
        {
            return reportParseError(ctx, "Header does not contain a 'method' attribute");
        }
        const char *s = headerJsonDoc["method"].as<const char *>();
        std::string method(s);
        ctx->method = method;

        if (!headerJsonDoc.containsKey("id"))
        {
            return reportParseError(ctx, "Header does not contain a 'method' attribute");
        }
        const char *idChar = headerJsonDoc["id"].as<const char *>();
        std::string id(idChar);
        ctx->id = id;

        if (frame.length() > end_of_protocol + 4 + headerSize)
        {
            ctx->msgPayload = frame.substr(end_of_protocol + 2 + headerSize, frame.length());

            DynamicJsonDocument payloadJsonDoc(2048);

            if (deserializeJson(payloadJsonDoc, ctx->msgPayload) != DeserializationError::Ok)
            {
                return reportParseError(ctx, "Cannot parse payload into json doc");
            };
        }
    }
}