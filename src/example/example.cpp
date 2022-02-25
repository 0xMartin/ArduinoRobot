#ifdef DEFAULT

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

#endif