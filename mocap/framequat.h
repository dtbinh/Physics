#ifndef FRAMEQUAT_H
#define FRAMEQUAT_H
#include "math/quaternion.h"
#include <vector>

class FrameQuat
{
public:
    std::vector<QuaternionQ> quatDes;
    FrameQuat();
    FrameQuat( std::vector<QuaternionQ> quatDes );
    ~FrameQuat();

    //retorna o tamanho do vetor quatDes
    int getSize();
};

#endif // FRAMEQUAT_H
