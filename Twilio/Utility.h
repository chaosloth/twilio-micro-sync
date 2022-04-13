#pragma once
#include <string>
#include <strings.h>

namespace twilio
{
    class Utility
    {
    public:
        static const bool icomp(std::string &s1, const std::string &s2);
    };

}