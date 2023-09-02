#include "remote_ai_client.hpp"
#include "functionality.hpp"

RemoteAIClient::RemoteAIClient(const char *a_ssid, const char *a_pass)
: m_wifimulti{}, m_client{}, m_topics{}, m_timer{}, m_host{0}, m_port{0}
{
    m_wifimulti.addAP(a_ssid, a_pass);
    while(m_wifimulti.run() != WL_CONNECTED) {
        delay(500);
    }
}

void RemoteAIClient::connect_host(const char *host, const uint16_t port)
{
    if (!m_client.connect(host, port)) {
        delay(10);
        yield();
        return;
    }
    m_client.setNoDelay(true);
    wait_for_response(1000);
    if(port != m_port){
        strcpy(m_host, host);
        m_port = port;
    }
    if(m_topics.size() > 0){
        resubscribe_topics();
    }
}

void RemoteAIClient::add_topic(String const &a_topic, Functionality a_func)
{
    
    m_client.print("/sub/"+a_topic);
    m_topics[a_topic] = a_func;
}

void RemoteAIClient::remove_topic(String const &a_topic)
{
    for(std::map<String, Functionality>::iterator i = m_topics.begin(); i != m_topics.end(); ++i){
        if(i->first.compareTo(a_topic) == 0){
            m_topics.erase(i);
            return;
        }
    }
}

void RemoteAIClient::wait_for_response(int max_attempts)
{
    if(m_client.connected() == 0){
        connect_host(m_host, m_port);
    }
    int maxloops=0;
    while (m_client.available() == 0 && maxloops < max_attempts)
    {
      maxloops++;
      delay(1);
      yield();
    }

    if (m_client.available() > 0)
    {
      String line = m_client.readStringUntil('\r');
      if(line.length() == 0){
        return;
      }
      for(auto &topic : m_topics){
        if(is_topic(topic.first, line)){
            topic.second.run();
        }
      }
    }
    yield();
}

void RemoteAIClient::ping()
{
    if(m_timer.is_passed_ms(150)){
        m_client.print("ping");
    }
}

bool RemoteAIClient::is_topic(String const &a_topic, String &a_msg)
{
  return a_msg.startsWith(a_topic);
}

void RemoteAIClient::resubscribe_topics()
{
    for(auto &topic : m_topics){
        m_client.print("/sub/"+topic.first);
        wait_for_response(10);
    }
}

void RemoteAIClient::topic_loader(std::vector<String> &a_topics, std::vector<Functionality> const &a_func_vector)
{
    for(size_t i = 0; i < a_topics.size(); ++i){
        add_topic(a_topics[i], a_func_vector[i]);
        wait_for_response(500);
    }
}