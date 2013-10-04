#ifndef SENSOR_H
#define SENSOR_H

#include "scene/character.h"

//#define ALL_FOOT_AIR 0
//#define ONE_FOOT_GROUND 1
//#define TWO_FOOT_GROUND 2
#define ALL_FOOTS_GROUND  -3  //para ser usado na analise modal   //pos no vector inertiaFactorMaps: 0 (+3)
#define FOOTS_AIR_INV     -2                                      //pos no vector inertiaFactorMaps: 1 (+3)
#define FOOTS_AIR         -1                                      //pos no vector inertiaFactorMaps: 2 (+3)

/*
  Esta classe define todos os sensores usados no controle de balanço, ou seja,
  os estados em que se encontram os pé(s), em contato ou não com o solo.
*/
class Sensor
{
public:
    Sensor();
    static void setTolerance(float val);
    static int getStateFoots(Character *chara);
    static int getSwingFoot(Character *chara);
    static int getStanceFoot(Character *chara);
    static int getHierarchy2Use(Character *chara); //esta função determina qual matriz deverá ser usada no cálculo da
                                                   //jacobiana
    static int getHierarchy2UseMocap(Character *chara);

};

#endif // SENSOR_H
