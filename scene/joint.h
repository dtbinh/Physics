#ifndef JOINT_H
#define JOINT_H
#include "physics/physics.h"
#include "math/vec4.h"

#define JOINT_FIXED 0
#define JOINT_HINGE 1
#define JOINT_BALL  2

class Object;
class Character;
class Material;


class Joint
{
public:

    JointID     joint;            //id da junta
    Object*     parent;           //pai da junta
    Object*     child;            //filho da junta
    Material*   material;         //material utilizado para desenho da junta
    Vec4        limSup;           //guarda os limites superiores
    Vec4        limInf;           //guarda os limites inferiores
    //Matrix4f *transform;        //Matriz de transformação da junta
    int         type;             //tipo de junta (fixa, hinge, ball,...)
    bool        selected;         //informa qual junta esta selecionada
    QString     name;


    Vec4 *initialAnchor;          //posição inicial da ancoragem da junta

    Character *character;

    Joint(Character* chara,int type);
    Joint(JointID joint,Character *chara,Object* parent,Object *child,int type,Vec4 limSup, Vec4 limInf);
    ~Joint();

    void          initJoint(Vec4 anchor=Vec4());
    void          setJoint(JointID joint);
    JointID       getJoint();
    void          setParent(Object *parent);
    Object*       getParent();
    void          setChild(Object *child);
    Object*       getChild();
    Character*    getCharacter();
    void          restartJoint();
    void          setSelected(bool selected);
    bool          isSelected();
    void          setName(QString name);
    QString       getName();




    //void setColor(int color);

    void draw();
};

#endif // JOINT_H
