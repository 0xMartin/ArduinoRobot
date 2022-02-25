/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-23 12:12:16
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 17:13:23
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
#include "ir_controler.h"

IRControler::IRControler(uint8_t _ID, void *_robot_core) : Module(_ID, H_BRIDGE, _robot_core)
{
    this->irrecv = NULL;
    this->last_value = 0;
    this->last_key = IR_KEY_NONE;
}

bool IRControler::init(void *_configuration)
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
    this->config = (IRControlerConfig *)_configuration;
    this->irrecv = new IRrecv(this->config->pin);
    this->irrecv->enableIRIn();

    // pokud je mapa znak prazdna pak inizializuje defaultni
    if (this->config->keyMap == NULL || this->config->mapSize == 0)
    {
        this->config->keyMap = (IRKey *)malloc(sizeof(IRKey) * 17);
        this->config->mapSize = 17;
        this->config->keyMap[0] = {0xFF6897, IR_KEY_1};
        this->config->keyMap[1] = {0xFF9867, IR_KEY_2};
        this->config->keyMap[2] = {0xFFB04F, IR_KEY_3};
        this->config->keyMap[3] = {0xFF30CF, IR_KEY_4};
        this->config->keyMap[4] = {0xFF18E7, IR_KEY_5};
        this->config->keyMap[5] = {0xFF7A85, IR_KEY_6};
        this->config->keyMap[6] = {0xFF10EF, IR_KEY_7};
        this->config->keyMap[7] = {0xFF38C7, IR_KEY_8};
        this->config->keyMap[8] = {0xFF5AA, IR_KEY_9};
        this->config->keyMap[9] = {0xFF4AB5, IR_KEY_0};
        this->config->keyMap[10] = {0xFF42BD, IR_KEY_MULTIPLY};
        this->config->keyMap[11] = {0xFF52AD, IR_KEY_HASH};
        this->config->keyMap[12] = {0xFF02FD, IR_KEY_OK};
        this->config->keyMap[13] = {0xFF22DD, IR_KEY_LEFT};
        this->config->keyMap[14] = {0xFFC23D, IR_KEY_RIGHT};
        this->config->keyMap[15] = {0xFF629D, IR_KEY_UP};
        this->config->keyMap[16] = {0xFFA857, IR_KEY_DOWN};
    }

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

void IRControler::__process()
{
    decode_results results;
    if (this->irrecv->decode(&results))
    {
        Serial.println(results.value);
        this->last_value = results.value;
        for (uint8_t i = 0; i < this->config->mapSize; ++i)
        {
            if (this->config->keyMap[i].value == this->last_value)
            {
                this->last_key = this->config->keyMap[i].key;
                break;
            }
        }
        this->time = millis();
        irrecv->resume();
    }
    else
    {
        if (!this->config->hold_key)
        {
            if (this->last_key != IR_KEY_NONE)
            {
                if (millis() - this->time > 100)
                {
                    this->last_key = IR_KEY_NONE;
                }
            }
        }
    }
}

void IRControler::printModuleInfo(char *_str_buffer, uint16_t _lenght) const
{
    if (_str_buffer != NULL)
    {
        snprintf(
            _str_buffer,
            _lenght,
            "[IR CONTROLER] - ID: %d - Value: %d - Key: %d",
            (int)this->ID,
            (int)this->last_value,
            (int)this->last_key);
    }
}

unsigned long IRControler::getLastValue()
{
    return this->last_value;
}

uint8_t IRControler::getLastKey()
{
    return this->last_key;
}
