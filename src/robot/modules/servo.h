/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 18:49:55
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:26:56
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

#ifndef SERVO_H
#define SERVO_H

#include "../robot_core.h"

#include <Servo.h>

/**
 * @brief Configurace servo modulu
 *
 */
struct ServoConfig
{
    uint8_t pin;
    int speed;
    int default_position;
};

/**
 * @brief ServoMotor modul
 *
 */
class ServoMotor : public Module
{
protected:
    bool stopped;          /* Pro zastaveni pohybu */
    int set_position;      /* Aktualne nastavena pozice (stupne) */
    float writed_position; /* Naposledy zapsana pozica na servo */
    ServoConfig *config;   /* Configurace serva */
    Servo handler;         /* Manipulace ze servem */
    unsigned long time;    /* Pro postupnout rychlost */

    virtual void __process() override;

public:
    /**
     * @brief Vytvori instance modulu serva
     *
     * @param _ID ID modulu
     * @param _robot_core Ukazatel na jadro systemu
     */
    ServoMotor(uint8_t _ID, void *_robot_core = NULL);

    /**
     * @brief Inizializuje modul ultrazvuku
     *
     * @param _configuration Ukazatel na strukturu s configuraci (UltrasonicConfig)
     * @return true Vse probehlo v poradku
     * @return false Neco se nepovedlo
     */
    virtual bool init(void *_configuration) override;

    virtual void printModuleInfo(char *_str_buffer, uint16_t _lenght) const override;

    /**
     * @brief Ziska aktualni pozici, na ktere se servo nachazi
     *
     * @return uint8_t
     */
    int getPosition();

    /**
     * @brief Nastavy novou pozici serva
     *
     * @param position Nova pozice 0 <-> 180 (90 - stred)
     */
    void setPosition(int _position);

    /**
     * @brief Nastavy novou pozici serva a ceka dokud se servo na ni nedostane
     *
     * @param position Nova pozice 0 <-> 180 (90 - stred)
     */
    void setPositionAndWait(int _position);

    /**
     * @brief Zahaji pohyb serva (nutne pouzit je kdyz bylo servo zastaveno funci "stop()")
     *
     */
    void begin();

    /**
     * @brief Okamzite zastavy pohyb serva
     *
     */
    void stop();
};

#endif