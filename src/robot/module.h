/***************************************************************************
 * @Author: Martin Krcma
 * @Date: 2022-02-20 12:39:05
 * @Last Modified by: Martin Krcma
 * @Last Modified time: 2022-02-23 16:48:35
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

#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>

/**
 * @brief Typ modulu
 *
 */
enum ModuleType
{
    UNDEFINED = 0,
    ULTRASONIC = 1,
    SERVO_MOTOR = 2,
    IR_RECEIVER = 3,
    H_BRIDGE = 4,
    LIGHT_SENSOR = 5
};

/**
 * @brief Typ modulu (enum) prevede na string podoby
 *
 * @param _str_buffer Ukazatel na char buffer
 * @param _buffer_size Velikost char bufferu
 * @param _type Typ modulu
 */
static inline void getTypeName(char *_str_buffer, size_t _buffer_size, ModuleType _type)
{
    switch (_type)
    {
    case ULTRASONIC:
        snprintf(_str_buffer, _buffer_size, "ULTRASONIC");
        break;
    case SERVO_MOTOR:
        snprintf(_str_buffer, _buffer_size, "SERVO MOTOR");
        break;
    case IR_RECEIVER:
        snprintf(_str_buffer, _buffer_size, "IR_RECEIVER");
        break;
    case H_BRIDGE:
        snprintf(_str_buffer, _buffer_size, "H-BRIDGE");
        break;
    case LIGHT_SENSOR:
        snprintf(_str_buffer, _buffer_size, "LIGHT SENSOR");
    default:
        snprintf(_str_buffer, _buffer_size, "UNDEFINED");
    }
}

/**
 * @brief Privek linked listu akci
 *
 */
struct ActionNode
{
    ActionNode *next;
    void *action;
};

class Module
{
protected:
    uint8_t ID;               /* ID modulu */
    ModuleType type;          /* Typ modulu */
    void *robot_core;         /* Ukazatel na jadro robo systemu */
    ActionNode *actions_list; /* Linked list s akcemi pro tento modul */

    /**
     * @brief Zpracovani modul
     *
     */
    virtual void __process() = 0;

public:
    /**
     * @brief Konstruktor abstraktni tridy Modulu
     *
     * @param _ID ID modulu
     * @param _type Typ modulu
     * @param _robot_core Ukazatel na jadro robo systemu
     */
    Module(uint8_t _ID, ModuleType _type, void *_robot_core = NULL);

    /**
     * @brief Inicializuje modul
     *
     * @param _configuration Ukazatel na strukturu s konfiguraci
     * @return true Vse probehlo v poradku
     * @return false Neco se nepovedlo
     */
    virtual bool init(void *_configuration) = 0;

    /**
     * @brief Zpracuje modul + zpracuje akce
     *
     */
    void process();

    /**
     * @brief Ziska ID modulu
     *
     * @return uint8_t
     */
    uint8_t getID() const;

    /**
     * @brief Nastavy jadro robo systemu
     *
     * @param _robot_core  Pointer na jadro robo systemu
     */
    void setRobotCore(void *_robot_core);

    /**
     * @brief Ziska typ modulu
     *
     * @return ModuleType
     */
    ModuleType getType() const;

    /**
     * @brief Vypise informace do char bufferu
     *
     * @param _str_buffer Char buffer
     * @param _lenght Velikost char bufferu
     */
    virtual void printModuleInfo(char *_str_buffer, uint16_t _lenght) const = 0;

    /**
     * @brief Prida novou akci
     *
     * @param action Nova akce
     * @return true Akce uspesne pridana
     * @return false Akci se nepodarilo pridat
     */
    bool addAction(void *action);

    /**
     * @brief Z modulu odebere akci
     *
     * @param action Akce, kterou chceme odebrat
     * @return true Akci se podarilo odebrat
     * @return false Akci se nepodarilo odebrat
     */
    bool removeAction(void *action);

    /**
     * @brief Ziska prvni prvek linked listu z akcemi
     *
     * @return ActionNode*
     */
    ActionNode *getActionList() const;
};

/**
 * @brief Abstraktni trida akce modulu
 *
 */
class ModuleAction
{
public:
    /**
     * @brief Predicate funkce akce, pokud tato funkce navrati true akce
     * modulu zavola callback proceduru a vykona se pozadovany kod
     *
     */
    virtual bool predicate(Module *module) const = 0;

    /**
     * @brief Callback akce modulu
     *
     */
    virtual void callback(Module *module) = 0;
};

#endif