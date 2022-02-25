#ifdef LIDAR_ROBOT

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
  u_config.sample_count = 1;
  ultra.init(&u_config);
  // do systemu prida modul pro ultrazvuk
  core->addModule(&ultra);

  // defaultni konfigurace modulu
  ServoConfig s_config;
  s_config.pin = PIN_SERVO;
  s_config.default_position = 0;
  s_config.speed = 360;
  servo.init(&s_config);
  // do systemu prida modul pro ultrazvuk
  core->addModule(&servo);
}

static int sample_count_left = 0;
static int sample_count_right = 0;
static int sample_count_center = 0;
static float avg_left = 0.0;
static float avg_right = 0.0;
static float avg_center = 0.0;
static int minimum = 255;

#define SAFE_RANGE 50
#define CRITICAL_RANGE 30

#define MAX_SPEED 10
#define MIN_SPEED 5

//#define DEBUG

/**
 * @brief Zpracovani naskenovanych hodnot
 *
 */
static void processScan()
{
#ifdef DEBUG
  if (sample_count_left > 0)
  {
    Serial.print("Left: ");
    Serial.println(avg_left / sample_count_left);
  }
  if (sample_count_right > 0)
  {
    Serial.print("Right: ");
    Serial.println(avg_right / sample_count_right);
  }
  if (sample_count_center > 0)
  {
    Serial.print("Center: ");
    Serial.println(avg_center / sample_count_center);
  }
#endif

  if (minimum < SAFE_RANGE)
  {
    if (sample_count_left > 0 && sample_count_right > 0 && sample_count_center > 0)
    {
      // robot zacne zatacet na tu stranu na ktere je v prumeru vetsi namerena vzdalenost (mene prekazek)
      // prioritu ma center -> pokud je neco pred robotem tak se muze jen otacet
      avg_left /= (float)sample_count_left;
      avg_right /= (float)sample_count_right;
      avg_center /= (float)sample_count_center;
      // jede vpred + zataci
      if (avg_center < CRITICAL_RANGE)
      {
        if (avg_left > avg_right)
        {
          hbridge.setSpeed(-MIN_SPEED, MIN_SPEED);
        }
        else
        {
          hbridge.setSpeed(MIN_SPEED, -MIN_SPEED);
        }
      }
      else
      {
        if (avg_left > avg_right)
        {
          hbridge.setSpeed(MIN_SPEED, MAX_SPEED);
        }
        else
        {
          hbridge.setSpeed(MAX_SPEED, MIN_SPEED);
        }
      }
    }
    else
    {
      // reseni chybneho stavu
      hbridge.setSpeed(MIN_SPEED, MAX_SPEED);
    }
  }
  else
  {
    // jede vpred pokud se pred nim nic nenachazi
    hbridge.setSpeed(MAX_SPEED, MAX_SPEED);
  }

  // reset
  sample_count_left = sample_count_right = sample_count_center = 0;
  avg_left = avg_right = avg_center = 0.0;
  minimum = 255;
}

/**
 * @brief Update loop
 *
 */
static void update(RobotCore *core)
{
  // podobny princip skenovani jako u lidaru

  // naustaly pohyb serva z leva do prava
  if (servo.getPosition() < 1)
  {
    servo.setPosition(180);
    processScan();
  }
  else if (servo.getPosition() > 179)
  {
    servo.setPosition(0);
    processScan();
  }

  // skenovani prostoru
  int pos = servo.getPosition();
  if (pos < 75)
  {
    // prava strana robota
    ++sample_count_right;
    avg_right += ultra.getDist();
  }
  else if (pos < 115)
  {
    // prava strana robota
    ++sample_count_center;
    avg_center += ultra.getDist();
  }
  else
  {
    // leva strana robota
    ++sample_count_left;
    avg_left += ultra.getDist();
  }
  minimum = min(minimum, ultra.getDist());
}

#endif