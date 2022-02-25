# Arduino Robot Library

Knihona postavená na arduino frameworku ulehčuje prací s moduly a čidly. Aktualně je implementováno několik zakladních modulů (Modul pro ovládání H-můstku L298N, modul pro řízení servo motoru, modul pro IR přijímač, modul pro light sensor, modul pro ultrazvukový sensor). System využívá jedno vlakno a všechny moduly jsou navrženy tak, že nijak výrazně nebrzdí chod programu (multitasking). 

## Základní struktura:
```c++
#include <Arduino.h>
#include "robot/robot_core.h"
#include "robot/modules/modules.h"
INIT_ROBO_SYSTEM


/**
 * @brief Sestaveni modelu robota
 *
 */
static void buildRobotModel(RobotCore *core)
{
    // Zavola se jen jednou pri spousteni programu (zde vytvaret a vkladat moduly do systemu)
}

/**
 * @brief Update loop
 *
 */
static void update(RobotCore *core)
{
    // Vola se neustalo (vzdy po tom co se zpracuji vsechny moduly systemu)
}
```
