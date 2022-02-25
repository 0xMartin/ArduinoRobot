// odkomentuj pro vyber prikladoveho programu
//#define DEFAULT
//#define IR_REMOTE_CONTROL
#define LIDAR_ROBOT
//#define MIZE_ROBOT

/**
 * @brief Zakladni struktura programu
 * 
 */
#ifdef DEFAULT
#include "example/example.cpp"
#endif

/**
 * @brief Robot na dalkove ovladani (ovladaneni => jen sipky na ovladaci pro pohyb)
 *
 */
#ifdef IR_REMOTE_CONTROL
#include "example/example_ir.cpp"
#endif

/**
 * @brief Robot vyhybajici se prekazkam, vyuziva ultrazvuk pro nepretrzite skenovani
 *  prostoru kolem sebe a podle toho se rozhoduje kam pojede (podopne jako lidar)
 *
 */
#ifdef LIDAR_ROBOT
#include "example/example_lidar.cpp"
#endif

/**
 * @brief Robot jede vpred dokud nenarazi na nejakou prekazku, pak se rozhledne
 *  kolem sebe a zjisti kam dal muze jet (pouzitelne napr. do bludiste)
 * 
 */
#ifdef MIZE_ROBOT
#include "example/example_mize.cpp"
#endif


