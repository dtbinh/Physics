#ifndef JOINT_H
#define JOINT_H
#include "physics/physics.h"
#include "math/vec4.h"

#define JOINT_FIXED 0
#define JOINT_HINGE 1
#define JOINT_BALL  2

class Object;
class Character;
class MaterialObj;


class Joint
{
public:

    JointID     joint;            //id da junta
    Object*     parent;           //pai da junta
    Object*     child;            //filho da junta
    MaterialObj *material;         //material utilizado para desenho da junta
    Vec4        limSupA;           //guarda os limites superiores angular
    Vec4        limInfA;           //guarda os limites inferiores angular
    Vec4        tqMax;            //guarda o torque máximo
    //Matrix4f *transform;        //Matriz de transformação da junta
    int         type;             //tipo de junta (fixa, hinge, ball,...)
    bool        selected;         //informa qual junta esta selecionada
    QString     name;
    float       radius_hinge;     //adiciona o raio para a junta do tipo hinge


    Vec4 *initialAnchor;          //posição inicial da ancoragem da junta
    Vec4 *initialAxis;            //eixo inicial de ancoragem da junta hinge

    Character *character;
    Scene *scene;

    Joint(Character* chara,int type);
    Joint(int type);
    Joint(JointID joint,Character *chara,Object* parent,Object *child,int type,Vec4 limSup, Vec4 limInf);
    void setLimitsAngularSuperior(Vec4 sup);
    void setLimitsAngularInferior(Vec4 inf);
    void setTorqueMax(Vec4 tq);
    void setScene(Scene *scn);
    void setRadiusHinge(float radius);
    Vec4 getTorqueMax();
    Vec4 getLimitAngularMax();
    Vec4 getLimitAngularMin();
    Vec4 getPositionAnchorInit();
    Vec4 getPositionAnchor();
    ~Joint();
    //----Matrix
    MatrixF getAd();
    MatrixF getAd(Vec4 pos); //calcula matriz Adjunta que muda as coordenadas do frame da junta para o frame do mundo (R=identity) posicionado num dado ponto (em coordenadas globais)
    MatrixF getAd(Object* obj); //calcula matriz Adjunta que muda as coordenadas do frame do objeto para o frame da junta




    void          initJoint(Vec4 anchor=Vec4(), Vec4 axis=Vec4());
    void          setJoint(JointID joint);
    JointID       getJoint();
    void          setParent(Object *parent);
    Object*       getParent();
    int           getType();
    void          setChild(Object *child);
    Object*       getChild();
    Character*    getCharacter();
    void          restartJoint();
    void          setSelected(bool selected);
    bool          isSelected();
    void          setName(QString name);
    QString       getName();
    Vec4          getPositionCurrent();
    QString       showInfo();


    //void setColor(int color);
    //-------Graphics

    void draw();
    void drawShadow();
    void drawPreShadow();
};

#endif // JOINT_H
