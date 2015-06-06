#include "sensor.h"
#include "scene/object.h"
#include "math/vec4.h"
#include "mocap/frame.h"
#include "mocap/framequat.h"
#include "scene/grf.h"
#include <vector>
#include "graphics/draw.h"
#include "scene/scene.h"
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

//    std::vector<Object*> foots;
//    for(int i=0;i<chara->getNumBodies();i++){
//        if(chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));
//    }
//    int count = 0;
//    for(unsigned int i=0;i<foots.size();i++){
//        Vec4 value = chara->getGRFSum(foots.at(i));
//        if(value.module()>tolerance){
//            count++;
//        }
//    }
//    foots.clear();
    if(chara->getMoCap()!=NULL && chara->getMoCap()->status){
        if (getHierarchy2UseMocap(chara)==0) return 2;
        else if(getHierarchy2UseMocap(chara)<0) return 0;
        else return 1;
    }
    else{
        if (getHierarchy2UseMocap(chara)==0) return 2;
        else if (getHierarchy2UseMocap(chara)<0) return 0;
        else return 1;
    }

//    return count;

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
        foots.clear();
        return proj;
    }
    int hierarquia = getHierarchy2Use(chara);

    if (hierarquia==0){
        for(unsigned int i=0;i<foots.size();i++){
                proj += foots.at(i)->getPositionCurrent();
        }
        proj = proj/2.0;
        proj.x2 = 0;
        foots.clear();
        return proj;
    }else{
        for(unsigned int i=0;i<foots.size();i++){
            if(chara->getIdObject(foots.at(i))==hierarquia-3){
                proj = foots.at(i)->getPositionCurrent();
            }
        }
        proj.x2 = 0;
        foots.clear();
        return proj;
    }

}

bool Sensor::isSwingFoot(Object *obj,Character *chara)
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
        bool close_enought_l = true;
        bool close_enought_r = true;
        Vec4 prop = foots.at(0)->getProperties();
        float height = prop.y()/2.0;
        if( (foots.at(0)->posEffectorBackward().y()<height+0.05 || foots.at(0)->posEffectorForward().y()<height+0.05)||((chara->getGRFSum(foots.at(0)).y()>0))){
        //if (chara->getGRFSum(foots.at(0)).module()>0){
            close_enought_l = true;
        }
        if ((foots.at(1)->posEffectorBackward().y()<height+0.05 || foots.at(1)->posEffectorForward().y()<height+0.05)||(chara->getGRFSum(foots.at(1)).y()>0)){
        //if (chara->getGRFSum(foots.at(1)).module()>0){
            close_enought_r = true;
        }
        foot_l =  obj->getCharacter()->getMoCap()->getFrameMotion(frame)->getFootLeftGround();
        foot_r =  obj->getCharacter()->getMoCap()->getFrameMotion(frame)->getFootRightGround();
//        if (!foot_l){
//            //if (foots.at(0)->posEffectorBackward().y()<height+0.0005 || foots.at(0)->posEffectorForward().y()<height+0.0005){
//            if (chara->getGRFSum(foots.at(0)).y()>0){
//                close_enought_l = true;
//            }else
//                close_enought_l = false;
//        }else if (chara->getGRFSum(foots.at(0)).y()>0){

//            close_enought_l =true;
//        }
//        if (!foot_r){
//    //        if (foots.at(1)->posEffectorBackward().y()<height+0.0005 || foots.at(1)->posEffectorForward().y()<height+0.0005){
//            if (chara->getGRFSum(foots.at(1)).y()>0){
//                close_enought_r = true;
//            }else
//                close_enought_r = false;
//        }else if (chara->getGRFSum(foots.at(1)).y()>0){
//            close_enought_r =true;
//        }
        if (foot_l && foot_r && close_enought_l && close_enought_r){
            foots.clear();
            return false;
        }
        else if (foot_l && close_enought_l){
            if (obj == foots.at(0)) return false;
            else return true;

        }
        else if (foot_r && close_enought_r){
            if (obj == foots.at(1)) return false;
            else return true;

        }
        foots.clear();
        return false;
    }else{
        //if (chara->getGRFSum(obj).module()>0) return false;
        return false;
    }

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
    foots.clear();
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
        if(value.module()>0){
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
    Vec4 pd(0,0,0); //ponto desejado
    Vec4 com = chara->getPosCOM();

    Vec4 ori;
    dReal anglex;
    if (foots.at(0)->isCollideWithGround()){
        foots.at(0)->getRotationCurrent().toAxisAngle(&ori,&anglex);
        if(fabs(ori.z()*anglex)<45){
            pd += foots.at(0)->getPositionCurrent();
            contact = foots.at(0);
            state++;
        }
    }
    if (foots.at(1)->isCollideWithGround()){
        foots.at(1)->getRotationCurrent().toAxisAngle(&ori,&anglex);
        if(fabs(ori.z()*anglex)<45){
            pd += foots.at(1)->getPositionCurrent();
            state++;
            contact = foots.at(1);
        }
    }
//    for(unsigned int i=0;i<foots.size();i++){
//        Vec4 value = chara->getGRFSum(foots.at(i));
//        if(value.y()>5.0){
//            contact = foots.at(i);
//            //pd += foots.at(i)->getPositionCurrent();
//            state++;
//        }
//    }
    //caso 3
    if(state>1){
        pd /= 2.0;
        pd.x2 = 0.0;
        com.x2 = 0.0;
        //testando o primeiro pé
        //float desired1 = (pd - (foots.at(1)->getPositionCurrent().projXZ())).module();
        float dcom1 = (com - (foots.at(1)->getPositionCurrent().projXZ())).module();
        bool right_foot = true;
        bool left_foot = true;
        if (dcom1 > tolerance){ // pé esquerdo não esta em contato
            right_foot = false;
            //return chara->getPositionBody(foots.at(0))+3;
        }
        //float desired2 = (pd - (foots.at(0)->getPositionCurrent().projXZ())).module();
        float dcom2 = (com - (foots.at(0)->getPositionCurrent().projXZ())).module();
        if (dcom2> tolerance){ // pé esquerdo não esta em contato
            left_foot = false;
        }
        if (left_foot && right_foot){
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(1)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(0)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1+tolerance,Vec4(1,0,0),2.0);
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2+tolerance,Vec4(1,0,0),2.0);
            return ALL_FOOTS_GROUND+3;
        }
        else if(!left_foot && !right_foot){
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(1)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(0)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1+tolerance,Vec4(1,0,0),2.0);
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2+tolerance,Vec4(1,0,0),2.0);
            return ALL_FOOTS_GROUND+3;
        }
        else if(left_foot && !right_foot){
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(0)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2+tolerance,Vec4(1,0,0),2.0);
            return chara->getPositionBody(foots.at(0))+3;
        }
        else if(!left_foot && right_foot){
            Draw::drawCircle2D(Vec4::projectionPlane(foots.at(1)->getPositionCurrent(),chara->getScene()->getRotationPlaneVector(),Vec4()),tolerance,Vec4(0,1,0),2.0,chara->getScene()->getRotationPlane());
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1+tolerance,Vec4(1,0,0),2.0);
            return chara->getPositionBody(foots.at(1))+3;
        }else{
            return FOOTS_AIR+3;

        }


    }
//    if(state>1){
//        pd /= 2.0;
//        pd.x2 = 0.0;
//        com.x2 = 0.0;
//        //testando o primeiro pé
//        float desired1 = (pd - (foots.at(1)->getPositionCurrent().projXZ())).module();
//        float dcom1 = (com - (foots.at(1)->getPositionCurrent().projXZ())).module();
//        bool right_foot = true;
//        bool left_foot = true;
//        if (dcom1 - desired1 > tolerance){ // pé esquerdo não esta em contato
//            right_foot = false;
//            //return chara->getPositionBody(foots.at(0))+3;
//        }
//        float desired2 = (pd - (foots.at(0)->getPositionCurrent().projXZ())).module();
//        float dcom2 = (com - (foots.at(0)->getPositionCurrent().projXZ())).module();
//        if (dcom2 - desired2 > tolerance){ // pé esquerdo não esta em contato
//            left_foot = false;
//            //return chara->getPositionBody(foots.at(1))+3;
//        }
//        if (left_foot && right_foot){
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1,Vec4(0,1,0),2.0);
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2,Vec4(0,1,0),2.0);
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1+tolerance,Vec4(1,0,0),2.0);
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2+tolerance,Vec4(1,0,0),2.0);
//            return ALL_FOOTS_GROUND+3;
//        }
//        else if(left_foot && !right_foot){
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2,Vec4(0,1,0),2.0);
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent().projXZ(),desired2+tolerance,Vec4(1,0,0),2.0);
//            return chara->getPositionBody(foots.at(0))+3;
//        }
//        else if(!left_foot && right_foot){
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1,Vec4(0,1,0),2.0);
//            Draw::drawCircle2D(foots.at(1)->getPositionCurrent().projXZ(),desired1+tolerance,Vec4(1,0,0),2.0);
//            return chara->getPositionBody(foots.at(1))+3;
//        }else{
//            return FOOTS_AIR+3;

//        }


//    }
    //caso 2
//    if(state>0){
//        pd /= 2.0;
//        pd.x2 = 0.0;
//        com.x2 = 0.0;
//        //testando o primeiro pé
//        //Draw::drawLine(Vec4(0,0,0),Vec4(0,1,0),Vec4(0,1,0),1.0);
//        float d0 = (com - (foots.at(0)->getPositionCurrent().projXZ())).module();
//        float d1 = (com - (foots.at(1)->getPositionCurrent().projXZ())).module();
//        if (d0-tolerance>d1){
//            Draw::drawCircle2D(foots.at(0)->getPositionCurrent(),)
//            return chara->getPositionBody(foots.at(1))+3;
//        }
//        if (d1-tolerance>d0) return chara->getPositionBody(foots.at(0))+3;

//        float dcom = (com - (foots.at(1)->getPositionCurrent().projXZ())).module();
//        if (dcom - desired > tolerance){
//            return chara->getPositionBody(foots.at(0))+3;
//        }
//        desired = (pd - (foots.at(0)->getPositionCurrent().projXZ())).module();
//        dcom = (com - (foots.at(0)->getPositionCurrent().projXZ())).module();
//        if (dcom - desired > tolerance){
//            return chara->getPositionBody(foots.at(1))+3;
//        }

    //}

    //foots.clear();
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
        //if (foots.at(0)->posEffectorBackward().y()<height+0.05 || foots.at(0)->posEffectorForward().y()<height+0.05){
        if (foots.at(0)->isCollideWithGround()/*getHierarchy2Use(chara)-3==ALL_FOOTS_GROUND || getHierarchy2Use(chara)-3==chara->getPositionBody(foots.at(0))*//*chara->getGRFSum(foots.at(0)).module()>0*/){
            close_enought_l = true;
        }/*else
            close_enought_l = false;*/
    }else if (foots.at(0)->isCollideWithGround()/*getHierarchy2Use(chara)-3==ALL_FOOTS_GROUND || getHierarchy2Use(chara)-3==chara->getPositionBody(foots.at(0))*//*chara->getGRFSum(foots.at(0)).module()>0*/){

        close_enought_l =true;
    }
    if (!foot_r){
        //if (foots.at(1)->posEffectorBackward().y()<height+0.05 || foots.at(1)->posEffectorForward().y()<height+0.05){
        if (foots.at(1)->isCollideWithGround()/*getHierarchy2Use(chara)-3==ALL_FOOTS_GROUND || getHierarchy2Use(chara)-3==chara->getPositionBody(foots.at(1))*//*chara->getGRFSum(foots.at(1)).module()>0*/){
            close_enought_r = true;
        }/*else
            close_enought_r = false;*/
    }else if (foots.at(1)->isCollideWithGround()/*getHierarchy2Use(chara)-3==ALL_FOOTS_GROUND || getHierarchy2Use(chara)-3==chara->getPositionBody(foots.at(1))*//*chara->getGRFSum(foots.at(1)).module()>0*/){
        close_enought_r =true;
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
    switch (state){
        case 0:{
            //printf("\nUso da Hierarquia via MotionCap = Nenhum(%d)",FOOTS_AIR+3);
            return FOOTS_AIR+3;
            break;
    }
    case 1:{
        //printf("\nUso da Hierarquia via MotionCap = Um (%d)",chara->getPositionBody(contact)+3);
        return chara->getPositionBody(contact)+3;
        break;
    }
    case 2:{
        //printf("\nUso da Hierarquia via MotionCap = Dois (%d)",ALL_FOOTS_GROUND+3);
        return ALL_FOOTS_GROUND+3;
        break;
    }
    }
}
