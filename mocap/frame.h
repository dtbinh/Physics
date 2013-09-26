#ifndef FRAME_H
#define FRAME_H
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "math/vec4.h"
#include "math/quaternion.h"

/*cada frame relaciona posições e orientações*/
class Frame
{
private:
    std::vector<Vec4> positions;
    std::vector<Quaternion> orientations;

public:
    Frame();
    Frame( std::vector<Vec4> pos, std::vector<Quaternion> quat);
    void appendPosition(Vec4 pos);
    void appendOrientation(Quaternion quat);
    void setPosition(int i,Vec4 pos);
    void setOrientation(int i, Quaternion quat);
    Vec4 getPosition(int i);
    Quaternion getOrientation(int i);
    ~Frame();

    //retorna o tamanho dos vetores pos e quat, que devem ter tamanhos iguais
    int getNumFrames();


};

#endif // FRAME_H
