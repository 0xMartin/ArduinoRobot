#ifdef MIZE_ROBOT

#include <Arduino.h>
#include "robot/robot_core.h"
#include "robot/modules/modules.h"
INIT_ROBO_SYSTEM

HBridge hbridge(1);
Ultrasonic ultra(2);
ServoMotor servo(3);

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

  // defaultni konfigurace modulu
  UltrasonicConfig u_config;
  u_config.pin_echo = PIN_ULTRASONIC_ECHO;
  u_config.pin_trig = PIN_ULTRASONIC_TRIG;
  u_config.sample_count = 2;
  ultra.init(&u_config);
  // do systemu prida modul pro ultrazvuk
  core->addModule(&ultra);

  // defaultni konfigurace modulu
  ServoConfig s_config;
  s_config.pin = PIN_SERVO;
  s_config.default_position = 90;
  s_config.speed = 200;
  servo.init(&s_config);
  // do systemu prida modul pro ultrazvuk
  core->addModule(&servo);
}

/**
 * @brief Update loop
 *
 */
static void update(RobotCore *core)
{
  delay(2);
  if (ultra.getDist() > 20)
  {
    // jede vpred
    hbridge.setSpeed(50, 50);
  }
  else
  {
    // hledani cesty
    hbridge.stopMotor();

    // leva strana
    servo.setPositionAndWait(0);
    ultra.process();
    if (ultra.getDist() > 30)
    {
      Serial.println("left");
      servo.setPositionAndWait(90);
      hbridge.setSpeedTimeAndWait(15, -15, 320);
      return;
    }

    // prava
    servo.setPositionAndWait(180);
    ultra.process();
    if (ultra.getDist() > 30)
    {
      Serial.println("right");
      servo.setPositionAndWait(90);
      hbridge.setSpeedTimeAndWait(-15, 15, 320);
      return;
    }

    // otocit se zpet
    Serial.println("back");
    servo.setPositionAndWait(90);
    hbridge.setSpeedTimeAndWait(-15, 15, 600);
  }
}

#endif
