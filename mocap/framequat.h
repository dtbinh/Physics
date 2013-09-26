#ifndef FRAMEQUAT_H
#define FRAMEQUAT_H
#include "math/quaternion.h"
#include <vector>

class FrameQuat
{
public:
    std::vector<Quaternion> quatDes;
    FrameQuat();
    FrameQuat( std::vector<Quaternion> quatDes );
    ~FrameQuat();

    //retorna o tamanho do vetor quatDes
    int getSize();
};

#endif // FRAMEQUAT_H
