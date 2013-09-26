#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>

class Scene;
class MoCap;
class Character;

class Utils
{
public:
    Utils();
    static bool readModelRubens(Scene* scene,const std::string &fileName);
    static bool saveModelRubens(Character *chara,const std::string &fileName);
    static bool loadMotionCapture(MoCap* moCap,Character* chara, const std::string &fileName);
};

#endif // UTILS_H
