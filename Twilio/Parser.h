#pragma once
#include "TwilsockMessage.h"
#include "ArduinoJson-v6.19.4.h"
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

namespace twilio
{
    struct ParseContext
    {
        std::string frame;
        std::string protocol;
        std::string version;
        std::string header;
        std::string msgPayload;
        std::string method;
        std::string id;
        explicit ParseContext(std::string const &frame) : frame(frame) {}
        explicit ParseContext() {}
    };

    using MessagePtr = std::shared_ptr<ParseContext>;

    class Parser
    {
    public:
        // [[nodiscard]] static MessagePtr parse(ParseContext &ctx);
        static MessagePtr parse(ParseContext &ctx);
        static std::string createMessage(ParseContext &ctx);
        static void printJsonObject(JsonObject obj, std::string indent);

    private:
        static void processFrame(MessagePtr &ctx);
        static const bool icomp(std::string &s1, const std::string &s2);
    };
}