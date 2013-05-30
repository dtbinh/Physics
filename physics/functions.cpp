#include "functions.h"
#include "math/quaternion.h"

void to_dQuaternion(Quaternion q,dQuaternion qi)
{
    qi[0] = q.qw();
    qi[1] = q.qx();
    qi[2] = q.qy();
    qi[3] = q.qz();
}
