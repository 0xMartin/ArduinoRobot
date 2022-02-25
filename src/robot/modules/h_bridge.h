/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 23:01:51
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-20 23:42:45
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

#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include "../robot_core.h"

/**
 * @brief Struktura pro konfiguraci utralzvokoveho modulu
 *
 */
struct HBridgeConfig
{
    uint8_t pin_in1;
    uint8_t pin_in2;
    uint8_t pin_ENA;
    uint8_t pin_in3;
    uint8_t pin_in4;
    uint8_t pin_ENB;
    bool invert_A;
    bool invert_B;
};

/**
 * @brief H-mustek modul
 *
 */
class HBridge : public Module
{
protected:
    bool time_on;          /* Cas aktivovan*/
    unsigned long time;    /* Cas po ktery budou motory spustene (ms) */
    int left_power;        /* Vykon leveho motoru */
    int right_power;       /* Vykon praveho motoru */
    HBridgeConfig *config; /* Configurace H-mustku */

    virtual void __process() override;

public:
    /**
     * @brief Vytvori instance modulu H-mustku
     *
     * @param _ID ID modulu
     * @param _robot_core Ukazatel na jadro systemu
     */
    HBridge(uint8_t _ID, void *_robot_core = NULL);

    /**
     * @brief Inizializuje modul H-mustku
     *
     * @param _configuration Ukazatel na strukturu s configuraci (HBridgeConfig)
     * @return true Vse probehlo v poradku
     * @return false Neco se nepovedlo
     */
    virtual bool init(void *_configuration) override;

    virtual void printModuleInfo(char *_str_buffer, uint16_t _lenght) const override;

    /**
     * @brief Nastaveni vykonu motoru -100 <-> 100
     *
     * @param _left_power Vykon leveho motoru
     * @param _right_power Vykon praveho motur
     */
    void setSpeed(int _left_power, int _right_power);

    /**
     * @brief Nastaveni vykonu motoru -100 <-> 100 na urcity cas
     *
     * @param _left_power Vykon leveho motoru
     * @param _right_power Vykon praveho
     * @param _time Cas po ktery budou motory spustene
     */
    void setSpeedTime(int _left_power, int _right_power, unsigned long _time);

    /**
     * @brief Nastaveni vykonu motoru -100 <-> 100 na urcity cas a pocka
     *
     * @param _left_power Vykon leveho motoru
     * @param _right_power Vykon praveho
     * @param _time Cas po ktery budou motory spustene
     */
    void setSpeedTimeAndWait(int _left_power, int _right_power, unsigned long _time);

    /**
     * @brief Zastaveni motoru
     *
     */
    void stopMotor();
};

#endif