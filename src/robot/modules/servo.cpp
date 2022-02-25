/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 18:58:21
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:29:33
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

#include "servo.h"

ServoMotor::ServoMotor(uint8_t _ID, void *_robot_core) : Module(_ID, SERVO_MOTOR, _robot_core)
{
    this->set_position = 0;
    this->writed_position = 0;
    this->stopped = false;
}

bool ServoMotor::init(void *_configuration)
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
    this->config = (ServoConfig *)_configuration;
    this->handler.attach(this->config->pin);
    this->handler.write(this->config->default_position + 6);
    this->writed_position = max(min(this->config->default_position, 180), 0);
    this->set_position = this->writed_position;

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

void ServoMotor::__process()
{
    if (this->stopped)
    {
        return;
    }
    // pozice serva je nastavena jen pokud doslo ke zmene hodnoty
    if (abs(this->writed_position - this->set_position) > 0)
    {
        // postupna zmena pozice
        float step = this->config->speed * (millis() - this->time) / 1000.0;
        this->time = millis();
        if (this->writed_position < this->set_position)
        {
            this->writed_position += step;
            if (this->writed_position > 180)
            {
                this->writed_position = 180;
            }
        }
        else
        {
            this->writed_position -= step;
            if (this->writed_position < 0)
            {
                this->writed_position = 0;
            }
        }
        // zapise pozici na servo
        this->handler.write(this->writed_position + 6);
    }
}

void ServoMotor::printModuleInfo(char *_str_buffer, uint16_t _lenght) const
{
    if (_str_buffer != NULL)
    {
        snprintf(
            _str_buffer,
            _lenght,
            "[SERVO] - ID: %d - POSITION: %d",
            (int)this->ID,
            (int)this->writed_position);
    }
}

int ServoMotor::getPosition()
{
    return (int)this->writed_position;
}

void ServoMotor::setPosition(int _position)
{
    this->time = millis();
    this->set_position = max(min(_position, 180), 0);
}

void ServoMotor::setPositionAndWait(int _position)
{
    this->time = millis();
    this->set_position = max(min(_position, 180), 0);
    while (abs(this->getPosition() - this->set_position) > 1)
    {
        delay(10);
        __process();
    }
}

void ServoMotor::begin()
{
    this->stopped = false;
}

void ServoMotor::stop()
{
    this->stopped = true;
}