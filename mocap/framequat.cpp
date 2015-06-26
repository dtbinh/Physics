#include "framequat.h"

FrameQuat::FrameQuat()
{
}

FrameQuat::FrameQuat(std::vector<QuaternionQ> quatDes)
{
  this->quatDes = quatDes;
}
//---------------------------------------------------------------------------
//Destrutor
FrameQuat::~FrameQuat()
{
  this->quatDes.clear();
}
//---------------------------------------------------------------------------
//retorna o tamanho do vetor quatDes
int FrameQuat::getSize() {
  return quatDes.size();
}
