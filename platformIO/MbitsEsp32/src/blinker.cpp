#include "blinker.hpp"

Blinker::Blinker(uint8_t a_led_channel, Adafruit_PWMServoDriver &a_pwm)
: m_led_channel(a_led_channel)
, m_pwm(a_pwm)
{
}

void Blinker::on(uint16_t a_speed)
{
    m_pwm.setPWM(m_led_channel, 0, a_speed);
}

void Blinker::off()
{
    m_pwm.setPWM(m_led_channel, 0, 0);
}
