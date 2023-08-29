#ifndef REMOTE_AI_CLIENT
#define REMOTE_AI_CLIENT

#include <WiFi.h>
#include <WiFiMulti.h>
#include <vector>
#include "timer.hpp"

class RemoteAIClient {
public:
    RemoteAIClient() = default;
    RemoteAIClient(const char *a_ssid, const char *a_pass);
    void connect_host(const char *host, const uint16_t port);
    void add_topic(String const &a_topic);
    void remove_topic(String const &a_topic);
    String wait_for_response(int max_attempts);
    void ping();

private:  
    bool is_topic(String &a_topic, String &a_msg);

private:
    WiFiMulti m_wifimulti;
    WiFiClient m_client;
    std::vector<String> m_topics;
    Timer m_timer;
};

#endif // REMOTE_AI_CLIENT