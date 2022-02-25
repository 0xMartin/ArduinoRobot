/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 23:14:25
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:43:03
 *
 *  Copyright (C) 2022 Martin Krcma
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/
#include "h_bridge.h"

HBridge::HBridge(uint8_t _ID, void *_robot_core) : Module(_ID, H_BRIDGE, _robot_core)
{
    this->left_power = 0;
    this->right_power = 0;
    this->time = 0;
    this->time_on = false;
}

bool HBridge::init(void *_configuration)
{
    if (_configuration == NULL)
    {
// DEBUG INFO
#ifndef REMOVE_DEBUG
        if (((RobotCore *)this->robot_core)->getConfig()->debug_info)
        {
            DEBUG_PREFIX;
            DEBUG_INFO("Nepodarilo se inicializovat\n");
        }
#endif
        return false;
    }

    // inicializace
    this->config = (HBridgeConfig *)_configuration;
    pinMode(this->config->pin_in1, OUTPUT);
    pinMode(this->config->pin_in2, OUTPUT);
    pinMode(this->config->pin_in3, OUTPUT);
    pinMode(this->config->pin_in4, OUTPUT);
    pinMode(this->config->pin_ENA, OUTPUT);
    pinMode(this->config->pin_ENB, OUTPUT);

    this->stopMotor();

// DEBUG INFO
#ifndef REMOVE_DEBUG
    if (((RobotCore *)this->robot_core)->getConfig()->debug_info)
    {
        DEBUG_PREFIX;
        DEBUG_INFO("Inizializace byla uspesna\n");
    }
#endif
    return true;
}

void HBridge::__process()
{
    if (this->time_on)
    {
        if (this->time < millis())
        {
            this->stopMotor();
            this->time_on = false;
        }
    }
    analogWrite(this->config->pin_ENA, abs(this->left_power));
    analogWrite(this->config->pin_ENB, abs(this->right_power));
}

void HBridge::printModuleInfo(char *_str_buffer, uint16_t _lenght) const
{
    if (_str_buffer != NULL)
    {
        snprintf(
            _str_buffer,
            _lenght,
            "[H-BRIDGE] - ID: %d - Left power: %d - Right power: %d",
            (int)this->ID,
            (int)this->left_power,
            (int)this->right_power);
    }
}

void HBridge::setSpeed(int _left_power, int _right_power)
{
    if (this->config == NULL)
    {
        return;
    }

    this->left_power = max(-100, min(_left_power, 100));
    if (this->config->invert_A)
    {
        this->left_power = -this->left_power;
    }
    this->left_power = (int)(this->left_power / 100.0 * 1023.0);
    this->right_power = max(-100, min(_right_power, 100));
    if (this->config->invert_B)
    {
        this->right_power = -this->right_power;
    }
    this->right_power = (int)(this->right_power / 100.0 * 1023.0);

    if (this->left_power > 0)
    {
        digitalWrite(this->config->pin_in1, HIGH);
        digitalWrite(this->config->pin_in2, LOW);
    }
    else if (this->left_power < 0)
    {
        digitalWrite(this->config->pin_in1, LOW);
        digitalWrite(this->config->pin_in2, HIGH);
    }
    else
    {
        digitalWrite(this->config->pin_in1, LOW);
        digitalWrite(this->config->pin_in2, LOW);
    }

    if (this->right_power > 0)
    {
        digitalWrite(this->config->pin_in3, HIGH);
        digitalWrite(this->config->pin_in4, LOW);
    }
    else if (this->right_power < 0)
    {
        digitalWrite(this->config->pin_in3, LOW);
        digitalWrite(this->config->pin_in4, HIGH);
    }
    else
    {
        digitalWrite(this->config->pin_in3, LOW);
        digitalWrite(this->config->pin_in4, LOW);
    }
}

void HBridge::setSpeedTime(int _left_power, int _right_power, unsigned long _time)
{
    this->time = millis() + _time;
    this->time_on = true;
    this->setSpeed(_left_power, _right_power);
}

void HBridge::setSpeedTimeAndWait(int _left_power, int _right_power, unsigned long _time)
{
    this->setSpeedTime(_left_power, _right_power, _time);
    while (this->time_on)
    {
        delay(10);
        __process();
    }
}

void HBridge::stopMotor()
{
    this->left_power = 0;
    this->right_power = 0;
    digitalWrite(this->config->pin_in1, LOW);
    digitalWrite(this->config->pin_in2, LOW);
    digitalWrite(this->config->pin_in3, LOW);
    digitalWrite(this->config->pin_in4, LOW);
}