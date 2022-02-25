/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 11:38:51
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:27:03
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

#include "ultrasonic.h"

Ultrasonic::Ultrasonic(uint8_t _ID, void *_robot_core) : Module(_ID, ULTRASONIC, _robot_core)
{
    this->current_distance = 0;
}

bool Ultrasonic::init(void *_configuration)
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
    this->config = (UltrasonicConfig *)_configuration;
    pinMode(this->config->pin_echo, INPUT);
    pinMode(this->config->pin_trig, OUTPUT);

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

void Ultrasonic::__process()
{
    long sum_dist = 0;
    for (uint8_t i = 0; i < this->config->sample_count; ++i)
    {
        digitalWrite(this->config->pin_trig, LOW);
        delayMicroseconds(2);
        digitalWrite(this->config->pin_trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->config->pin_trig, LOW);

        unsigned long duration;
        duration = pulseIn(this->config->pin_echo, HIGH);
        sum_dist += duration * 0.034 / 2;
    }
    this->current_distance = sum_dist / this->config->sample_count;
}

void Ultrasonic::printModuleInfo(char *_str_buffer, uint16_t _lenght) const
{
    if (_str_buffer != NULL)
    {
        snprintf(
            _str_buffer,
            _lenght,
            "[ULTRASONIC] - ID: %d - DIST: %d",
            (int)this->ID,
            (int)this->current_distance);
    }
}

uint8_t Ultrasonic::getDist()
{
    return this->current_distance;
}