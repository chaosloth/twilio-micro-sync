/*============================================================
  Copyright (c) 2022 Christopher Connolly
  Project:     MicroSync
  Author:      Christopher Connolly
  Last update: 13.04.2022
  Description: Light weight Twilsock message parser Sync Client
  License:     MIT
  Dependencies & Kudos:
    - ArduionoJson v6.19.4 (included in library folder)
  ============================================================
  */
#include <cstdio>
#include "Twilio/Parser.h"
#include "Twilio/MessageProcessor.h"

using namespace std;

twilio::MessageProcessor proc;

void test1()
{
    printf("\n\n**TEST1: Incoming Message \n");
    string header = R"RAW(TWILSOCK V3.0 528)RAW";
    string packet_end = "\r\n";
    string payload = R"RAW({"method":"reply","id":"TM13e75e18-3f4a-4673-8ce7-f4f1799969fc","payload_size":499,"payload_type":"application/json","status":{"code":200,"status":"OK"},"http_headers":{"server":"envoy","date":"Mon, 11 Apr 2022 06:39:53 GMT","content-type":"application/json","content-length":"499","etag":"\"2\"","twilio-request-id":"RQbacf77edf98a4c268cfaaa6cd4885a37","vary":"Accept-Encoding","x-shenanigans":"none","strict-transport-security":"max-age=31536000","x-envoy-upstream-service-time":"13"},"http_status":{"code":200,"status":"OK"}})RAW";
    payload = payload + packet_end + R"RAW({"data":{"firstName":"C"},"revision":"2","last_event_id":2,"sid":"ET901860bcd48b4642975dd552e4ae3d22","url":"https://cds.us1.twilio.com/v3/Services/IS3a8795e21ff952dfaa4163afd086321c/Documents/ET901860bcd48b4642975dd552e4ae3d22","unique_name":"session123","account_sid":"AC98fabdfe1966d015735a5a5e9e524a19","service_sid":"IS3a8795e21ff952dfaa4163afd086321c","date_expires":null,"date_created":"2022-04-08T06:46:39.662Z","date_updated":"2022-04-08T06:55:43.285Z","created_by":"asd","type":"document"})RAW";

    std::string raw = header + packet_end + payload + packet_end;
    twilio::ParseContext ctx(raw);

    try
    {
        twilio::MessagePtr msg = twilio::Parser::parse(ctx);

        printf("Frame Length: %lu\n", msg->frame.length());
        printf("Protocol: %s\n", msg->protocol.c_str());
        printf("Version: %s\n", msg->version.c_str());
        printf("ID: %s\n", msg->id.c_str());
        printf("Method: %s\n", msg->method.c_str());
        printf("Header Length: %lu\n", msg->header.length());
        printf("Payload Length: %lu\n", msg->msgPayload.length());

        printf("JSON Keys for header\n");
        DynamicJsonDocument headerJsonDoc(2048);
        deserializeJson(headerJsonDoc, msg->header);
        twilio::Parser::printJsonObject(headerJsonDoc.as<JsonObject>(), "");
        printf("\n");

        printf("JSON Keys for payload\n");
        DynamicJsonDocument payloadJsonDoc(2048);
        deserializeJson(payloadJsonDoc, msg->msgPayload);
        twilio::Parser::printJsonObject(payloadJsonDoc.as<JsonObject>(), "");
        printf("\n\n");
    }

    catch (std::exception const &e)
    {
        printf("Main Error: %s\n", e.what());
    }
}

void test2()
{
    try
    {
        printf("\n\n**TEST2: Outgoing Message Serialization\n");
        std::string token = "DEAD-BEEF-DEAD-BEEF";
        twilio::MessageProcessor proc(token);

        // std::string json = proc.makeInit();
        std::string json = proc.makeAckReply("a-made-up-id-7");

        twilio::ParseContext outCtx;
        outCtx.header = json;

        std::string outMsg = twilio::Parser::createMessage(outCtx);
        printf("%s\n", outMsg.c_str());
    }

    catch (std::exception const &e)
    {
        printf("Main Error: %s\n", e.what());
    }
}

void test3()
{
    printf("\n\n**TEST3: Incoming message processing\n");
    string header = R"RAW(TWILSOCK V3.0 180)RAW";
    string packet_end = "\r\n";
    string payload = R"RAW({"method":"notification","id":"TMed706b21f5a344aeb2de3f518b9a5328","payload_size":230,"payload_type":"application/json","message_type":"twilio.sync.event","notification_ctx_id":""})RAW";
    payload = payload + packet_end + R"RAW({"event_type":"subscription_established","correlation_id":1649773425339,"event_protocol_version":3,"event":{"object_sid":"ET901860bcd48b4642975dd552e4ae3d22","object_type":"document","replay_status":"completed","last_event_id":5}})RAW";
    std::string raw = header + packet_end + payload + packet_end;

    proc.processMessage(raw);
}

void test4()
{
    printf("\n\n**TEST: Generate init message\n");
    twilio::MessageProcessor proc;
    proc.setToken("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCIsImN0eSI6InR3aWxpby1mcGE7dj0xIn0.eyJqdGkiOiJTS2M2MzZhNGU2YWFjMjNkNjA3N2E5NzBhMDM0ZDY4MmVhLTE2NDk4MjgwOTUiLCJncmFudHMiOnsiaWRlbnRpdHkiOiJlc3AzMiIsImRhdGFfc3luYyI6eyJzZXJ2aWNlX3NpZCI6IklTM2E4Nzk1ZTIxZmY5NTJkZmFhNDE2M2FmZDA4NjMyMWMifX0sImlhdCI6MTY0OTgyODA5NSwiZXhwIjoxNjQ5ODMxNjk1LCJpc3MiOiJTS2M2MzZhNGU2YWFjMjNkNjA3N2E5NzBhMDM0ZDY4MmVhIiwic3ViIjoiQUM5OGZhYmRmZTE5NjZkMDE1NzM1YTVhNWU5ZTUyNGExOSJ9.vgKmmcRZ0ueJfkeePACYiUOa7Ov52K-3umeKSX6HA1w");

    twilio::ParseContext outCtx;
    outCtx.header = proc.makeInit();

    std::string msg = twilio::Parser::createMessage(outCtx);
    printf("Procol message: %s", msg.c_str());
}

void test5()
{

    printf("\n\n**TEST5: Process reponse\n");
    string header = R"RAW(TWILSOCK V3.0 920)RAW";
    string packet_end = "\r\n";
    string payload = R"RAW({"method":"reply","id":"TMXd4PoKWwUlPd9ZwfQgpeN3WiStdYMx5Ru0n8","payload_size":0,"status":{"code":200,"status":"OK"},"capabilities":["client_update","offline_storage"],"continuation_token":"eyJmb3JtYXQiOiJydGQtY3QtMSIsImVuZHBvaW50SWQiOiJ0d2kxLWNiNzQ5ZDk1MjQwNTQyMjNhZDczZDZjYTEyNTIxM2Y0IiwiZ3JhbnRzIjp7ImlkZW50aXR5IjoiZXNwMzIiLCJkYXRhX3N5bmMiOnsic2VydmljZV9zaWQiOiJJUzNhODc5NWUyMWZmOTUyZGZhYTQxNjNhZmQwODYzMjFjIn19fQ.tETD+ERq1kDEb/PXQkqCojGxwX14el723LQBPjCLa98","continuation_token_status":{"reissued":true,"reissue_reason":"MISSING","reissue_message":"Continuation token is not provided"},"offline_storage":{"data_sync":{"storage_id":"d0b81c8fba188dcd33427723f535fb9f6c8554fd"}},"init_registrations":[{"notification_ctx_id":"0b662ab0-4ab7-48a7-a2f1-9e19325f667b","type":"ers","product":"data_sync","message_types":["twilio.sync.event","com.twilio.rtd.cds.document","com.twilio.rtd.cds.list","com.twilio.rtd.cds.map"]}]})RAW";

    std::string raw = header + packet_end + payload + packet_end;

    try
    {
        twilio::MessageProcessor proc;
        proc.processMessage(raw);
    }
    catch (std::exception const &e)
    {
        printf("Message Processing Error: %s\n", e.what());
    }
}

void printMsg(std::string msg)
{
    printf("Procol message: %s", msg.c_str());
}

void setup()
{
    printf("Setup\n");
    std::string token = "DEAD-BEEF-DEAD-BEEF";
    proc.setToken(token);
    proc.registerOnMessage(printMsg);
}

void loop()
{
}

int main()
{
    printf("Parser Test\n");

    setup();
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}