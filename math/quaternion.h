#ifndef QUATERNION_H
#define QUATERNION_H

#define EPSILON 0.00001
#define HALFPI 1.570796326794895
#include "matrix4x4.h"
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "physics/physics.h"
#include "vec4.h"

class Quaternion
{
   public:
      float w,x,y,z;
   //construtores
      Quaternion();
      Quaternion( float w, Vec4 v );
      Quaternion( Vec4 euler ); //ordem XYZ
      Quaternion(float r,float x, float y, float z);
      //Quaternion(float r, Vec4 v);
      ~Quaternion();
      void setVector(Vec4 v);

      //retorna o quaternion que leva de qi para qf
      static Quaternion deltaQuat( Quaternion quatf, Quaternion quati );

      //retorna o modulo do quaternion
      float module();
      //retorna o quadrado do modulo do quaternion (nao tira a raiz quadrada)
      float module2();
      //normaliza o quaternion
      void normalize();

      //retorna o produto escalar entre this e quat
      float dot(Quaternion quat); //prodesc
      //retorna -quat
      Quaternion minusQuaternion(Quaternion quat); //menosq
      //calcula o menor arco (entre: this ate q ou this ate -q)
      Quaternion lessArc(Quaternion q); //menorArco
      void setQuaternion(float r,float x, float y, float z);
      float getScalar();
      float getPosX();
      float getPosY();
      float getPosZ();
      float qw();
      float qx();
      float qy();
      float qz();

      void setScalar(float s);
      void setPosX(float x);
      void setPosY(float y);
      void setPosZ(float z);
      Vec4 getVector();

      //interpolacao linear esferica
      Quaternion slerp( Quaternion quat, float t );
      void setQuaternion(Quaternion quat);
      void setQuaternion(float w, Vec4 quat);
      Quaternion operator*(float v);
      friend Quaternion operator+(Quaternion p,Quaternion q);
      friend Quaternion operator*(Quaternion p,Quaternion q);
      Quaternion conjugate();


//      double normal()
//      {
//          double res;
//          res = sqrt(this->getScalar()*this->getScalar()+
//                     this->getPosX()*this->getPosX()+
//                     this->getPosY()*this->getPosY()+
//                     this->getPosZ()*this->getPosZ());
//          return res;
//      }
      Matrix4x4 getMatrix();

      //os angulos aqui passados como parametro ou retornados sao considerados estar em graus
      //converte o quaternion em angulos de Euler (ordem XYZ)
      Vec4 toEuler(); //toEulerXYZ
      //converte os angulos de Euler em um quaternion e atribui a this (ordem XYZ)
      void fromEuler( Vec4 euler );
      //converte o quaternion em eixo e angulo (passados por referencia)
      void toAxisAngle( Vec4* axis, dReal* angle );
      //converte eixo e angulo em um quaternion e atribui a this
      void fromAxisAngle( Vec4 axis, dReal angle );

      //converte Quaternion em dQuaternion
//      void to_dQuaternion( dQuaternion q );
//      //converte em Quaternion a partir de dQuaternion
//      void from_dQuaternion( dQuaternion q );
//      void from_dQuaternion( const dQuaternion q );

//      //convert dVector3 (normalized_axis*angle) to Quaternion
//      void from_dVector3( dVector3 v );
//      //convert Vec4 (normalized_axis*angle) to Quaternion
//      void from_Vec4( Vec4 v3D );
//      //convert Quaternion to dVector3 (normalized_axis*angle)
//      void to_dVector3( dVector3 v );
//      //convert Quaternion to Vec4 (normalized_axis*angle)
//      void to_Vec4( Vec4& v3D );
};

#endif
