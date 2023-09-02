#ifndef REMOTE_AI_CLIENT
#define REMOTE_AI_CLIENT

#include <WiFi.h>
#include <WiFiMulti.h>
#include <map>
#include "timer.hpp"
#include "functionality.hpp"

class RemoteAIClient{

public:

    RemoteAIClient() = default;
    ~RemoteAIClient() = default;
    RemoteAIClient(const char *a_ssid, const char *a_pass);
    void connect_host(const char *host, const uint16_t port);
    void add_topic(String const &a_topic, Functionality a_func);
    void remove_topic(String const &a_topic);
    void wait_for_response(int max_attempts);
    void ping();
    void topic_loader(std::vector<String> &a_topics, std::vector<Functionality> const &a_func_vector);
    
private:  
    bool is_topic(String const&a_topic, String &a_msg);
    void resubscribe_topics();

private:
    WiFiMulti m_wifimulti;
    WiFiClient m_client;
    std::map<String, Functionality> m_topics;
    Timer m_timer;
    char m_host[20];
    uint16_t m_port;
};

#endif // REMOTE_AI_CLIENT

