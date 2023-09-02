#ifndef REMOTE_AI_CLIENT
#define REMOTE_AI_CLIENT

#include <WiFi.h>
#include <WiFiMulti.h>
#include <vector>
#include "timer.hpp"

class RemoteAIClient {
public:
    RemoteAIClient() = default;
    ~RemoteAIClient() = default;
    RemoteAIClient(const char *a_ssid, const char *a_pass);
    void connect_host(const char *host, const uint16_t port);
    void add_topic(String const &a_topic);
    void remove_topic(String const &a_topic);
    String wait_for_response(int max_attempts);
    void ping();

    template <typename Act, typename Context>
    void topic_loader(std::vector<String> &a_topics, Act a_action, Context &a_context);
    
private:  
    bool is_topic(String &a_topic, String &a_msg);
    bool is_ok(String &a_msg);
    void resubscribe_topics();

private:
    WiFiMulti m_wifimulti;
    WiFiClient m_client;
    std::vector<String> m_topics;
    Timer m_timer;
    char m_host[20];
    uint16_t m_port;
};


template <typename Act, typename Context>
void RemoteAIClient::topic_loader(std::vector<String> &a_topics, Act a_action, Context &a_context)
{
    for(auto &topic : a_topics){
        add_topic(topic);
        String response = wait_for_response(10000);
        if(is_ok(response)){
            a_action(a_context);
        }
    }
}

#endif // REMOTE_AI_CLIENT