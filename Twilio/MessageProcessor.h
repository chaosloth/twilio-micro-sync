#pragma once
#include <cstdlib>
#include <string>
#include "Parser.h"
#include "Utility.h"
#include "ArduinoJson-v6.19.4.h"
#include "TwilsockMessage.h"
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
    typedef void (*FuncPtrString)(std::string);

    class MessageProcessor
    {
        std::string token;

    public:
        void setToken(std::string token);
        void processMessage(std::string &incoming);
        void registerOnMessage(FuncPtrString func);
        std::string genRandom(const int len);
        std::string makeInit();
        std::string makeAckReply(std::string ref);
        std::string makeActiveGrant();
        MessageProcessor(std::string token) : token(token) {}
        MessageProcessor() {}

    private:
        void prepareAndSend(std::string header);
        void prepareAndSend(std::string header, std::string payload);
    };
}