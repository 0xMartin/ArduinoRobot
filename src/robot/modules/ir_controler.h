/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-23 12:10:00
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 17:04:43
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

#ifndef IR_CONTROLER_H
#define IR_CONTROLER_H

#include "../robot_core.h"

#include <IRremote.h>

// zakladni klice
#define IR_KEY_NONE 0x0
#define IR_KEY_1 0x1
#define IR_KEY_2 0x2
#define IR_KEY_3 0x3
#define IR_KEY_4 0x4
#define IR_KEY_5 0x5
#define IR_KEY_6 0x6
#define IR_KEY_7 0x7
#define IR_KEY_8 0x8
#define IR_KEY_9 0x9
#define IR_KEY_0 0xA
#define IR_KEY_MULTIPLY 0xB
#define IR_KEY_HASH 0xC
#define IR_KEY_LEFT 0xD
#define IR_KEY_RIGHT 0xE
#define IR_KEY_UP 0xF
#define IR_KEY_DOWN 0x10
#define IR_KEY_OK 0x11

/**
 * @brief Struktura pomoci, ktere se de defunuje prirazeni "key" k urcite hodnote nasnimane ze senzoru
 *
 */
struct IRKey
{
    unsigned long value;
    uint8_t key;
};

/**
 * @brief Struktura pro konfiguraci utralzvokoveho modulu
 *
 */
struct IRControlerConfig
{
    uint8_t pin;
    bool hold_key;
    uint8_t mapSize;
    IRKey *keyMap;
};

/**
 * @brief H-mustek modul
 *
 */
class IRControler : public Module
{
protected:
    unsigned long last_value; /* Naposledy nactena hodnota ze senzoru */
    uint8_t last_key;         /* Posledni znak */
    unsigned long time;       /* Cas posledno nactene hodnoty */

    IRrecv *irrecv;
    IRControlerConfig *config; /* Configurace IR controleru */

    virtual void __process() override;

public:
    /**
     * @brief Vytvori instanci modulu IR Controler
     *
     * @param _ID ID modulu
     * @param _robot_core Ukazatel na jadro systemu
     */
    IRControler(uint8_t _ID, void *_robot_core = NULL);

    /**
     * @brief Inizializuje modul IR Controler
     *
     * @param _configuration Ukazatel na strukturu s configuraci (IRControlerConfig)
     * @return true Vse probehlo v poradku
     * @return false Neco se nepovedlo
     */
    virtual bool init(void *_configuration) override;

    virtual void printModuleInfo(char *_str_buffer, uint16_t _lenght) const override;

    /**
     * @brief Navrati naposledy nactenou hodnotu prijateho signalu
     *
     * @return unsigned long
     */
    unsigned long getLastValue();

    /**
     * @brief Navrati naposledy nacteny znak
     *
     * @return uint8_t
     */
    uint8_t getLastKey();
};

#endif