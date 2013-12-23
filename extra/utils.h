#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include "QList"
class Vec4;
class Scene;
class MoCap;
class Character;

class Utils
{
public:
    Utils();
    static bool readModelRubens(Scene* scene,const std::string &fileName);
    static bool saveModelRubens(Character *chara,const std::string &fileName);
    static bool saveSimulationConfig(Scene* scene,const std::string &fileName);
    static bool loadSimulationConfig(Scene* scene,const std::string &fileName);
    static bool readFramesConfig(Character *chara,const std::string &fileName);
    static bool saveFramesConfig(Character *chara,const std::string &fileName);
    static bool loadMotionCapture(MoCap* moCap,Character* chara, const std::string &fileName);
    static bool saveListMotion(QList<Vec4> motion, QList<Vec4> mocap ,QList<double> y,const std::string &fileName);
    static bool saveListDistMotion(QList<double> sim, QList<double> mocap ,QList<double> tim,const std::string &fileName);
};

#endif // UTILS_H
