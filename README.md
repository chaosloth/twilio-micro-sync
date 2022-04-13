# Twilio Sync Client Libray for Microcontrollers

## Overview

Light weight library specifically for microcontrollers. This has been tested on ESP32 with the xtensa g++ complier only, you mileage may vary.

Check out twilio.com/sync

## Example usage

```c++
websockets::WebsocketsClient client;
twilio::MessageProcessor proc;

void sendWsMessage(std::string msg)
{
  Serial.printf(">>> Message sent as binary:\n%s\n\n", msg.c_str());
  client.sendBinary(msg.c_str());
}

void onWsMessageCallback(websockets::WebsocketsMessage message)
{
  Serial.printf("<<< Got Message:\n%s\n\n", message.data().c_str());
  std::string wsString(message.data().c_str());
  try
  {
    proc.processMessage(wsString);
  }
  catch (std::exception const &e)
  {
    Serial.printf("Message Processing Error: %s\n", e.what());
  }
}

void wireSocketProcessor()
{
  proc.registerOnMessage(sendWsMessage);
  client.setCACert(twil_ssl_ca_cert);
  client.onMessage(onWsMessageCallback);
  client.onEvent(onEventsCallback);
}
```
