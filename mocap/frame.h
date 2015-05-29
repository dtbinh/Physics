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
    std::vector<QuaternionQ> orientations;
    bool footLeft; //se está ou não em contato com o solo
    bool footRight; //se está ou não em contato com o solo
public:
    Frame();
    Frame( std::vector<Vec4> pos, std::vector<QuaternionQ> quat);
    void appendPosition(Vec4 pos);
    void appendOrientation(QuaternionQ quat);
    void setPosition(int i,Vec4 pos);
    void setOrientation(int i, QuaternionQ quat);
    void setFootRightGround(bool b);
    void setFootLeftGround(bool b);
    bool getFootRightGround();
    bool getFootLeftGround();
    Vec4 getPosition(int i);
    QuaternionQ getOrientation(int i);
    ~Frame();

    //retorna o tamanho dos vetores pos e quat, que devem ter tamanhos iguais
    int getNumFrames();


};

#endif // FRAME_H
