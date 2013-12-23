#include "sensor.h"
#include "scene/object.h"
#include "math/vec4.h"
#include "mocap/frame.h"
#include "mocap/framequat.h"
#include "scene/grf.h"
#include <vector>
#define ERROR 0.1
static float tolerance = 50;
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
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int count = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 value = chara->getGRFSum(foots.at(i));
        if(value.module()>tolerance){
            count++;
        }
    }

    return count;

}

Vec4 Sensor::getSupportProjected(Character *chara,bool capture)
{
    std::vector<Object*> foots;
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
     Vec4 proj = Vec4();
    if (capture) {
        for(unsigned int i=0;i<foots.size();i++){
                proj += foots.at(i)->getPositionCurrent();
        }
        proj = proj/2.0;
        proj.x2 = 0;
        return proj;
    }
    int hierarquia = getHierarchy2Use(chara);

    if (hierarquia==0){
        for(unsigned int i=0;i<foots.size();i++){
                proj += foots.at(i)->getPositionCurrent();
        }
        proj = proj/2.0;
        proj.x2 = 0;
        return proj;
    }else{
        for(unsigned int i=0;i<foots.size();i++){
            if(chara->getIdObject(foots.at(i))==hierarquia-3){
                proj = foots.at(i)->getPositionCurrent();
            }
        }
        proj.x2 = 0;
        return proj;
    }

}

bool Sensor::isSwingFoot(Object *obj)
{
    if(obj==NULL) return false;
    if(!(obj->getFoot())) return false;
    if((obj->getCharacter()==NULL)) return false;
    if(obj->getCharacter()->getMoCap()->status && obj->getCharacter()->getMoCap()->sizeFrames()>0){
        int frame =  obj->getCharacter()->getMoCap()->frame_current;
        std::vector<Object*> foots;
        for(int i=0;i< obj->getCharacter()->getNumBodies();i++){
            if( obj->getCharacter()->getBody(i)->getFoot()) foots.push_back( obj->getCharacter()->getBody(i));
        }
        bool foot_l,foot_r;
        bool close_enought_l = false;
        bool close_enought_r = false;
        Vec4 prop = foots.at(0)->getProperties();
        float height = prop.y()/2.0;
        if (foots.at(0)->posEffectorBackward().y()<height+0.005 || foots.at(0)->posEffectorForward().y()<height+0.005){
            close_enought_l = true;
        }
        if (foots.at(1)->posEffectorBackward().y()<height+0.005 || foots.at(1)->posEffectorForward().y()<height+0.005){
            close_enought_r = true;
        }
        foot_l =  obj->getCharacter()->getMoCap()->getFrameMotion(frame)->getFootLeftGround();
        foot_r =  obj->getCharacter()->getMoCap()->getFrameMotion(frame)->getFootRightGround();
        if (foot_l && foot_r && close_enought_l && close_enought_r)
            return false;
        else if (foot_l && close_enought_l){
            if (obj == foots.at(0)) return false;
            else return true;

        }
        else if (foot_r && close_enought_r){
            if (obj == foots.at(1)) return false;
            else return true;

        }
        return false;
    }
    return false;
}

int Sensor::getSwingFoot(Character *chara)
{
    std::vector<Object*> foots;
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int body = -1;
    int count = 0;


    for(unsigned int i=0;i<foots.size();i++){
        Vec4 value = chara->getGRFSum(foots.at(i));
        if(value.module()<tolerance){
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
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int body = -1;
    int count = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 value = chara->getGRFSum(foots.at(i));
        if(value.module()>tolerance){
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
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int state = 0;
    for(unsigned int i=0;i<foots.size();i++){
        Vec4 value = chara->getGRFSum(foots.at(i));
        if(value.y()>tolerance){
            contact = foots.at(i);
            state++;
        }
    }
    if(state==0) return FOOTS_AIR+3;
    else if(state>1) return ALL_FOOTS_GROUND+3;
    else return chara->getPositionBody(contact)+3;

}

int Sensor::getHierarchy2UseMocap(Character *chara)
{
    int frame = chara->getMoCap()->frame_current;
    std::vector<Object*> foots;
    Object* contact=NULL;
    for(int i=0;i<chara->getNumBodies();i++){
        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
    }
    int state = 0;
    bool foot_l,foot_r;
    bool close_enought_l = false;
    bool close_enought_r = false;
    Vec4 prop = foots.at(0)->getProperties();
    float height = prop.y()/2.0;
    foot_l = chara->getMoCap()->getFrameMotion(frame)->getFootLeftGround();
    foot_r = chara->getMoCap()->getFrameMotion(frame)->getFootRightGround();
    if (!foot_l){
        if (foots.at(0)->posEffectorBackward().y()<height+0.0005 || foots.at(0)->posEffectorForward().y()<height+0.0005){
            close_enought_l = true;
        }
    }else{
        close_enought_l =true;
    }
    if (!foot_r){
        if (foots.at(1)->posEffectorBackward().y()<height+0.0005 || foots.at(1)->posEffectorForward().y()<height+0.0005){
            close_enought_r = true;
        }
    }else{
        close_enought_r = true;
    }
    if (foot_l && foot_r && close_enought_l && close_enought_r)
        state = 2;
    else if (foot_l && close_enought_l){
        state = 1;
        contact = foots.at(0);
    }
    else if (foot_r && close_enought_r){
        state = 1;
        contact = foots.at(1);
    }

//    for(unsigned int i=0;i<foots.size();i++){
//        Vec4 prop = foots.at(i)->getProperties();
//        float height = prop.y()/2.0;
//        Vec4 pos = chara->getMoCap()->getFrameMotion(frame)->getPosition(chara->getIdObject(foots.at(i)));
//        //Vec4 posf = chara->getMoCap()->getFrameMotion(frame-1)->getPosition(chara->getIdObject(foots.at(i)));
//        if((pos.y() <= height+tolerance)){
//            contact = foots.at(i);
//            state++;
//        }
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
//    }
    //printf("State %d\n",state);
    if(state==0) return FOOTS_AIR+3;
    else if(state>1) return ALL_FOOTS_GROUND+3;
    else return chara->getPositionBody(contact)+3;
}
