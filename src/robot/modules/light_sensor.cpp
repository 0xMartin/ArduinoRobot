/*************************************************************************** 
 * @Author: Martin Krcma  
 * @Date: 2022-02-23 16:02:16  
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 16:33:36
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
#include "light_sensor.h"

LightSensor::LightSensor(uint8_t _ID, void *_robot_core) : Module(_ID, H_BRIDGE, _robot_core)
{
    this->last_value = 0;
}

bool LightSensor::init(void *_configuration)
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
    this->config = (LightSensorConfig *)_configuration;
    pinMode(this->config->pin, INPUT);

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

void LightSensor::__process()
{
    this->last_value = analogRead(this->config->pin); 
}

void LightSensor::printModuleInfo(char *_str_buffer, uint16_t _lenght) const
{
    if (_str_buffer != NULL)
    {
        snprintf(
            _str_buffer,
            _lenght,
            "[LIGHT SENSOR] - ID: %d - Value: %d",
            (int)this->ID,
            0);
    }
}

int LightSensor::getValue() {
    return this->last_value;
}