#ifndef MOCAP_H
#define MOCAP_H
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "math/vec4.h"
class Character;
class Frame;
class FrameQuat;

class MoCap
{
private:
    Character *chara;
    //guarda a configuração dos quaternios e posição a partir do arquivo de texto
    std::vector<Frame*> capMot;
    //guarda a configuração dos quaternios que serão usados na simulação
    std::vector<FrameQuat*> capMotFrame;


public:
    int frame_current;
    bool status;
    MoCap();
    MoCap(Character *chara);
    void appendFrame(Frame* frame);
    Frame* getFrameMotion(int i);
    std::vector<Frame*> getCapMotion();
    FrameQuat* getFrameSimulation(int i);
    void initializePosesModel(int frame=1); //inicializa o modelo com a pose passada pela captura de movimento
    void loadFrameSimulation();
    void stepFrame(int value);
    Vec4 velocityAngularBody(int frame, int body);
    Vec4 velocityLinearBody(int frame,int body);
    Vec4 getMomentumAngular(int frame);
    Vec4 getMomentumLinear(int frame);
    Vec4 getPosCOM(int frame);
    Vec4 getVelCOM(int frame);
    int sizeFrames();
    void setStatusMotion(bool b);
    void restart();
    int currentFrame();
    void clear();



    //draw
    void drawShadow(Vec4 offset,int frame);
};

#endif // MOCAP_H
