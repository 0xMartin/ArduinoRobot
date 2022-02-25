/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-23 15:59:41
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 16:33:42
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

#ifndef LIGHT_H
#define LIGHT_H

#include "../robot_core.h"

/**
 * @brief Struktura pro konfiguraci utralzvokoveho modulu
 *
 */
struct LightSensorConfig
{
    uint8_t pin;
};

/**
 * @brief H-mustek modul
 *
 */
class LightSensor : public Module
{
protected:
    int last_value;
    LightSensorConfig *config; /* Configurace light sensoru */

    virtual void __process() override;

public:
    /**
     * @brief Vytvori instanci modulu light sensoru
     *
     * @param _ID ID modulu
     * @param _robot_core Ukazatel na jadro systemu
     */
    LightSensor(uint8_t _ID, void *_robot_core = NULL);

    /**
     * @brief Inizializuje modul light sensor
     *
     * @param _configuration Ukazatel na strukturu s configuraci (LightSensorConfig)
     * @return true Vse probehlo v poradku
     * @return false Neco se nepovedlo
     */
    virtual bool init(void *_configuration) override;

    virtual void printModuleInfo(char *_str_buffer, uint16_t _lenght) const override;

    /**
     * @brief Navrati hodnotu ze senzoru
     * 
     * @return int 
     */
    int getValue();
};

#endif