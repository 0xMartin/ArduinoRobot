/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 13:53:49
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 17:13:26
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

#ifndef ROBOT_CORE_H
#define ROBOT_CORE_H

// pokud je nedostatek miste je mozne zkompilovat bez debug informaci pokud
// nejsou potreba (staci odkomentovat nasledujici radek)
#define REMOVE_DEBUG

#include <Arduino.h>
#include "module.h"

// piny
#define PIN_LIGHT_SENSOR_LEFT A1
#define PIN_LIGHT_SENSOR_CENTER A2
#define PIN_LIGHT_SENSOR_RIGHT A3
#define PIN_IR 12
#define PIN_H_BRIDGE_ENA 6
#define PIN_H_BRIDGE_ENB 5
#define PIN_H_BRIDGE_IN1 9
#define PIN_H_BRIDGE_IN2 8
#define PIN_H_BRIDGE_IN3 7
#define PIN_H_BRIDGE_IN4 4
#define PIN_SERVO 11
#define PIN_ULTRASONIC_ECHO 2
#define PIN_ULTRASONIC_TRIG 3

// konfigurace H BRIDGE
#define DEFAULT_CONFIG_H_BRIDGE                                   \
    {                                                             \
        PIN_H_BRIDGE_IN1, PIN_H_BRIDGE_IN2,                       \
            PIN_H_BRIDGE_ENA, PIN_H_BRIDGE_IN3, PIN_H_BRIDGE_IN4, \
            PIN_H_BRIDGE_ENB, true, false                         \
    }

// konfigurace IR prijimac
#define DEFAULT_CONFIG_IR      \
    {                          \
        PIN_IR, false, 0, NULL \
    }

// konfigurace ultrazvuk
#define DEFAULT_CONFIG_ULTRASONIC                   \
    {                                               \
        PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO, 1 \
    }

// konfigurace ultrazvuk
#define DEFAULT_CONFIG_SERVO \
    {                        \
        PIN_SERVO, 180, 90   \
    }

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DEBUG_PREFIX            \
    Serial.print("[");          \
    Serial.print(__FILENAME__); \
    Serial.print(":");          \
    Serial.print(__FUNCTION__); \
    Serial.print("] - ");

#define DEBUG_INFO(msg) Serial.print(msg);

/**
 * @brief Struktura pro konfiguraci jadra
 *
 */
struct CoreConfig
{
    uint8_t module_max_count;
    bool debug_info;
    unsigned long serial_baud_rate;
};

/**
 * @brief Jadro systemu robota
 *
 */
class RobotCore
{
protected:
    CoreConfig *config;   /* Configurace systemu */
    uint8_t module_count; /* Aktualni pocet modulu */
    Module **module_list; /* List s ukazately na moduly systemu */

public:
    /**
     * @brief Vytvori instanci jadra systemu robota
     *
     */
    RobotCore();

    /**
     * @brief Inicializuje jadro robo systemu
     *
     * @param _config Configurace jadra
     * @return true Vse probehlo v poradku
     * @return false Neco se nepodarilo
     */
    bool init(CoreConfig *_config);

    /**
     * @brief Prida do systemu novy modul (ultrazvuk, h mustek, ....)
     *
     * @param _module Ukazatel na novy modul
     * @return true Vse probehlo v poradku
     * @return false Jiz neni mozne pridat vice modulu
     */
    bool addModule(Module *_module);

    /**
     * @brief Odebere modul ze systemu
     *
     * @param _module Ukazatel na modul
     * @return true Odebrani modulu bylo uspesne
     * @return false Nebylo mozne odebrat modul
     */
    bool removeModule(Module *_module);

    /**
     * @brief Vypise info o vsech modulech systemu
     *
     */
    void showAllModules();

    /**
     * @brief V systemu najde modul, ktery ma pozadovane ID
     *
     * @param _ID ID hlenaho modulu modulu
     * @return Module*
     */
    Module *getModuleWithID(uint8_t _ID);

    /**
     * @brief Navrati ukazatel na strukturu s konfiguraci jadra systemu
     *
     * @return CoreConfig*
     */
    CoreConfig *getConfig() const;

    /**
     * @brief Zpracuje vsechny udalost (zpracuje kazdy modul), volat pri kazde iteraci nekonecne smycky programu
     *
     */
    void processEvents();
};

/**
 * @brief Inicializator systemu
 *
 */
#define INIT_ROBO_SYSTEM                          \
    RobotCore *core = new RobotCore();            \
    static void buildRobotModel(RobotCore *core); \
    static void update(RobotCore *core);          \
    void setup()                                  \
    {                                             \
        CoreConfig cnfg;                          \
        cnfg.debug_info = true;                   \
        cnfg.module_max_count = 10;               \
        cnfg.serial_baud_rate = 9600;             \
        core->init(&cnfg);                        \
        buildRobotModel(core);                    \
    }                                             \
    void loop()                                   \
    {                                             \
        core->processEvents();                    \
        update(core);                             \
    }

#endif