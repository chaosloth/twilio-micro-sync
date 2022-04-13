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

#include "Utility.h"

namespace twilio
{
    const bool Utility::icomp(std::string &s1, const std::string &s2)
    {
        return (strcasecmp(s1.c_str(), s2.c_str()) == 0);
    }
}