#include "communication_manager.hpp"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>

#define SCL_PIN 21
#define SDA_PIN 22

CommunicationManager::CommunicationManager()
: m_pwm(Adafruit_PWMServoDriver(0x47, Wire))
{}

void CommunicationManager::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN, 400000);
    m_pwm.begin();
}

Adafruit_PWMServoDriver &CommunicationManager::pwm()
{
    return m_pwm;
}
