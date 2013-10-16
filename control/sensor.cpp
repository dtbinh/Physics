#include "sensor.h"
#include "scene/object.h"
#include "math/vec4.h"
#include "mocap/frame.h"
#include "mocap/framequat.h"
#include <vector>
#define ERROR 0.1
static float tolerance = 0.05;
Sensor::Sensor()
{
}

void Sensor::setTolerance(float val)
{
    tolerance = val;
}

int Sensor::getStateFoots(Character *chara)
{
    std::vector<Object*> foots;
    for(unsigned int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int state = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        if(foots.at(i)->getPositionCurrent().y() <= height+0.05) state++;
    }
    return state;

}

int Sensor::getSwingFoot(Character *chara)
{
    std::vector<Object*> foots;
    for(unsigned int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int body = -1;
    int count = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        if(foots.at(i)->getPositionCurrent().y() > height+0.05){
            body = chara->getIdObject(foots.at(i));
            count++;
        }
    }
    if (count>1) return -1;
    return body;
}

int Sensor::getStanceFoot(Character *chara)
{
    std::vector<Object*> foots;
    for(unsigned int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int body = -1;
    int count = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        if(foots.at(i)->getPositionCurrent().y() <= height+0.05){
            body = chara->getIdObject(foots.at(i));
            count++;
        }
    }
    if (count>1) return -1;
    return body;
}

int Sensor::getHierarchy2Use(Character *chara)
{
    std::vector<Object*> foots;
    Object* contact=NULL;
    for(unsigned int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int state = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        if(foots.at(i)->getPositionCurrent().y() <= height+0.05){
            contact = foots.at(i);
            state++;
        }
    }
    //printf("State %d\n",state);
    if(state==0) return FOOTS_AIR+3;
    else if(state>1) return ALL_FOOTS_GROUND+3;
    else return chara->getPositionBody(contact)+3;

}

int Sensor::getHierarchy2UseMocap(Character *chara)
{
    int frame = chara->getMoCap()->frame_current;
    std::vector<Object*> foots;
    Object* contact=NULL;
    for(unsigned int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int state = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        Vec4 pos = chara->getMoCap()->getFrameMotion(frame)->getPosition(chara->getIdObject(foots.at(i)));
        Vec4 posf = chara->getMoCap()->getFrameMotion(frame-1)->getPosition(chara->getIdObject(foots.at(i)));
        if((pos.y() <= height+tolerance)){
            contact = foots.at(i);
            state++;
        }
        /**************** Sensor de teste
        Vec4 prop = foots.at(i)->getProperties();
        float height = prop.y()/2.0;
        Quaternion rot = chara->getMoCap()->getFrameMotion(frame)->getOrientation(chara->getIdObject(foots.at(i)));
        Vec4 pos = chara->getMoCap()->getFrameMotion(frame)->getPosition(chara->getIdObject(foots.at(i)));
        pos = Object::posEffectorForward(pos,rot,foots.at(i));
        Quaternion rotf = chara->getMoCap()->getFrameMotion(frame-1)->getOrientation(chara->getIdObject(foots.at(i)));
        Vec4 posf = chara->getMoCap()->getFrameMotion(frame-1)->getPosition(chara->getIdObject(foots.at(i)));
        posf = Object::posEffectorForward(posf,rotf,foots.at(i));

        Quaternion rot2 = chara->getMoCap()->getFrameMotion(frame)->getOrientation(chara->getIdObject(foots.at(i)));
        Vec4 pos2 = chara->getMoCap()->getFrameMotion(frame)->getPosition(chara->getIdObject(foots.at(i)));
        pos2 = Object::posEffectorBackward(pos2,rot2,foots.at(i));
        Quaternion rotf2 = chara->getMoCap()->getFrameMotion(frame-1)->getOrientation(chara->getIdObject(foots.at(i)));
        Vec4 posf2 = chara->getMoCap()->getFrameMotion(frame-1)->getPosition(chara->getIdObject(foots.at(i)));
        posf2 = Object::posEffectorBackward(posf2,rotf2,foots.at(i));
        if((pos.y() <= height+tolerance)&&((pos-posf).module()<ERROR)){
            contact = foots.at(i);
            state++;
        }else if((pos2.y() <= height+tolerance)&&((pos2-posf2).module()<ERROR)){
            contact = foots.at(i);
            state++;
        }
        ********************/
    }
    //printf("State %d\n",state);
    if(state==0) return FOOTS_AIR+3;
    else if(state>1) return ALL_FOOTS_GROUND+3;
    else return chara->getPositionBody(contact)+3;
}
