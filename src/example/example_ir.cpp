#ifdef IR_REMOTE_CONTROL

#include <Arduino.h>
#include "robot/robot_core.h"
#include "robot/modules/modules.h"
INIT_ROBO_SYSTEM

HBridge hbridge(1);
IRControler ir(2);

/**
 * @brief Sestaveni modelu robota
 *
 */
static void buildRobotModel(RobotCore *core)
{
  // defaultni konfigurace modulu
  HBridgeConfig h_config;
  h_config = DEFAULT_CONFIG_H_BRIDGE;
  hbridge.init(&h_config);
  // do systemu prida modul pro ovladani H mustku
  core->addModule(&hbridge);

  IRControlerConfig ir_config;
  // defaultni konfigurace modulu
  ir_config = DEFAULT_CONFIG_IR;
  ir.init(&ir_config);
  // do systemu prida modul pro IR komunikaci
  core->addModule(&ir);
}

/**
 * @brief Update loop
 *
 */
static void update(RobotCore *core)
{
  switch (ir.getLastKey())
  {
  case IR_KEY_DOWN:
    hbridge.setSpeed(-50, -50);
    break;
  case IR_KEY_UP:
    hbridge.setSpeed(70, 70);
    break;
  case IR_KEY_LEFT:
    hbridge.setSpeed(0, 50);
    break;
  case IR_KEY_RIGHT:
    hbridge.setSpeed(50, 0);
    break;
  default:
    hbridge.stopMotor();
    break;
  }
}

#endif