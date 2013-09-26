#include "sensor.h"
#include "scene/object.h"
#include "math/vec4.h"
#include "mocap/frame.h"
#include "mocap/framequat.h"
#include <vector>
Sensor::Sensor()
{
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
        if(foots.at(i)->getPositionCurrent().y() <= height) state++;
    }
    return state;

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
        if(pos.y() <= height+0.05){
            contact = foots.at(i);
            state++;
        }
    }
    //printf("State %d\n",state);
    if(state==0) return FOOTS_AIR+3;
    else if(state>1) return ALL_FOOTS_GROUND+3;
    else return chara->getPositionBody(contact)+3;
}
