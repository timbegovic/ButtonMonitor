#include <QDebug>
#include <lgpio.h>

#include "gpio.h"
#include "config.h"

gpio::gpio(QObject *parent) : QObject(parent)
{
    m_handle = lgGpiochipOpen(CHIP); // get a handle to the GPIO
    int init_level = 0;
    for (auto pin : LEDS) // Outputs
        lgGpioClaimOutput(m_handle, LFLAGS, pin, init_level);
    for (auto pin : BUTTONS) // Inputs
    {
        lgGpioClaimInput(m_handle, LFLAGS, pin);
        m_oldStates.push_back((bool)lgGpioRead(m_handle, pin)); // ensures proper edge-detection from point of initialization on
    }
}

gpio::~gpio()
{
    // Cleanup GPIO
    int init_level = 0;
    for (auto pin : LEDS)
        lgGpioWrite(m_handle, pin, init_level);
    lgGpiochipClose(m_handle);
    qDebug() << "bye";
}

// Write to pins
void gpio::set(int pin, bool value)
{
    lgGpioWrite(m_handle, pin, value);
}

void gpio::set(unsigned int pattern)
{
    int n = 0;
    int value = 0;
    for (auto pin: LEDS) {

       lgGpioWrite(m_handle, pin, value);

    }
}

bool gpio::get(int pin, getMode mode)
{
    unsigned int idx = BUTTONS.indexOf(pin);

    bool pinState = lgGpioRead(m_handle, pin);
    bool out = pinState;

    switch(mode)
    {

    case getMode::falling:
    {
        out = (m_oldStates[idx] && !pinState);  // ⁻⁻\__
        m_oldStates[idx] = pinState;
        break;
    }

    case getMode::rising:
    {
        out = (!m_oldStates[idx] && pinState);  // __/⁻⁻
        m_oldStates[idx] = pinState;
        break;
    }

    default: break;

    }

    return out;
}
