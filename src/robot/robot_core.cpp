/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 14:13:18
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 14:02:42
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

#include "robot_core.h"

RobotCore::RobotCore()
{
    this->module_count = 0;
    this->module_list = NULL;
    this->config = NULL;
}

bool RobotCore::init(CoreConfig *_config)
{
    if (this->config == NULL)
    {
        this->config = _config;
        Serial.begin(this->config->serial_baud_rate);
        this->module_list = (Module **)malloc(sizeof(Module *) * this->config->module_max_count);
        if (this->module_list == NULL)
        {
// DEBUG INFO
#ifndef REMOVE_DEBUG
            if (this->config->debug_info)
            {
                DEBUG_PREFIX;
                DEBUG_INFO("Nepodarilo se inicializovat jadro v dusledku nedostatecne velke pameti\n");
            }
#endif
            return false;
        }
// DEBUG INFO
#ifndef REMOVE_DEBUG
        if (this->config->debug_info)
        {
            DEBUG_PREFIX;
            DEBUG_INFO("Jadro uspesne inicializovano\n");
        }
#endif
        return true;
    }
    return false;
}

bool RobotCore::addModule(Module *_module)
{
    if (_module == NULL)
    {
// DEBUG INFO
#ifndef REMOVE_DEBUG
        if (this->config->debug_info)
        {
            DEBUG_PREFIX;
            DEBUG_INFO("Neni mozne pridat modul typu NULL\n");
        }
#endif
        return false;
    }

    // prida modul do listu
    if (this->module_count + 1 < this->config->module_max_count)
    {
        _module->setRobotCore(this);
        this->module_list[this->module_count] = _module;
        this->module_count++;
// DEBUG INFO
#ifndef REMOVE_DEBUG
        if (this->config->debug_info)
        {
            DEBUG_PREFIX;
            DEBUG_INFO("Modul ");
            char buffer[32];
            getTypeName(buffer, 32, _module->getType());
            DEBUG_INFO(buffer);
            DEBUG_INFO(" uspesne pridan\n");
        }
#endif
        return true;
    }
    else
    {
// DEBUG INFO
#ifndef REMOVE_DEBUG
        if (this->config->debug_info)
        {
            DEBUG_PREFIX;
            DEBUG_INFO("Nelze pridat jiz vice modulu\n");
        }
#endif
        return false;
    }
}

bool RobotCore::removeModule(Module *_module)
{
    for (uint8_t i = 0; i < this->module_count; ++i)
    {
        if (this->module_list[i] == _module)
        {
            for (uint8_t j = i; j + 1 < this->module_count; ++j)
            {
                this->module_list[j] = this->module_list[j + 1];
            }
            this->module_count--;
// DEBUG INFO
#ifndef REMOVE_DEBUG
            if (this->config->debug_info)
            {
                DEBUG_PREFIX;
                DEBUG_INFO("Modul uspesne odebran\n");
            }
#endif
            return true;
        }
    }
// DEBUG INFO
#ifndef REMOVE_DEBUG
    if (this->config->debug_info)
    {
        DEBUG_PREFIX;
        DEBUG_INFO("Nepodarilo se odebrat modul\n");
    }
#endif
    return false;
}

void RobotCore::showAllModules()
{
    static char buffer[256];
    Module *module;
    for (uint8_t i = 0; i < this->module_count; ++i)
    {
        module = this->module_list[i];
        if (module != NULL)
        {
            module->printModuleInfo(buffer, 256);
            Serial.println(buffer);
        }
    }
}

Module *RobotCore::getModuleWithID(uint8_t _ID)
{
    Module *module;
    for (uint8_t i = 0; i < this->module_count; ++i)
    {
        module = this->module_list[i];
        if (module != NULL)
        {
            if (module->getID() == _ID)
            {
// DEBUG INFO
#ifndef REMOVE_DEBUG
                if (this->config->debug_info)
                {
                    DEBUG_PREFIX;
                    DEBUG_INFO("Modul nalezen\n");
                }
#endif
                return module;
            }
        }
    }
// DEBUG INFO
#ifndef REMOVE_DEBUG
    if (this->config->debug_info)
    {
        DEBUG_PREFIX;
        DEBUG_INFO("Modul nenalezen\n");
    }
#endif
    return NULL;
}

CoreConfig *RobotCore::getConfig() const
{
    return this->config;
}

void RobotCore::processEvents()
{
    Module *module;
    for (uint8_t i = 0; i < this->module_count; ++i)
    {
        module = this->module_list[i];
        if (module != NULL)
        {
            module->process();
        }
    }
}