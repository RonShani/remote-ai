#include "remote_ai_client.hpp"

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
    String res = wait_for_response(10);
    if(port != m_port){
        strcpy(m_host, host);
        m_port = port;
    }
    if(m_topics.size() > 0){
        resubscribe_topics();
    }
}

void RemoteAIClient::add_topic(String const &a_topic)
{
    
    m_client.print("/sub/"+a_topic);
    m_topics.push_back(a_topic);
}

void RemoteAIClient::remove_topic(String const &a_topic)
{
    for(std::vector<String>::iterator i = m_topics.begin(); i != m_topics.end(); ++i){
        if(i->compareTo(a_topic) == 0){
            m_topics.erase(i);
            return;
        }
    }
}

String RemoteAIClient::wait_for_response(int max_attempts)
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
        return {};
      }
      for(auto &topic : m_topics){
        if(is_topic(topic, line)){
            return topic;
        }
      }
      return line;
      /*
      if(is_ok(line)){
        return "ok";
      } else {
        return {};
      }
      */
      
    }
    yield();
    return {};
}

void RemoteAIClient::ping()
{
    if(m_timer.is_passed_ms(150)){
        m_client.print("ping");
    }
}

bool RemoteAIClient::is_topic(String &a_topic, String &a_msg)
{
  return a_msg.startsWith(a_topic);
}

bool RemoteAIClient::is_ok(String &a_msg)
{
    for(size_t i = 0; i < a_msg.length()-2; ++i) {
        if(a_msg[i] == '2'){
            if(a_msg[i+1]=='0' && a_msg[i+2]=='0'){
                return true;
            } else {
                return false;   
            }
        }
    }
    return false;
}

void RemoteAIClient::resubscribe_topics()
{
    for(auto &topic : m_topics){
        m_client.print("/sub/"+topic);
        wait_for_response(10);
    }
}
