#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include "physics/physics.h"
#include "math/vec4.h"
#include "math/quaternion.h"
#include "extra/ObjMesh.h"

class Material;
class Matrix4x4;
class Matrix;
class Mesh;

#define TYPE_CUBE      dBoxClass
#define TYPE_CYLINDER  dCylinderClass
#define TYPE_SPHERE    dSphereClass


class Object
{
private:
    // --------Scene
    Scene        *scene;
    Character    *chara;
    QString      name;
    bool         selected;
    bool         bodyBalance;
    bool         isFoot;
    QString      objFile;
    ObjMesh      *objMesh;
    bool         rendermesh;

    // --------Physics
    BodyID       body;                                                    //body ODE
    GeomID       geometry;                                                //geom ODE
    Mass         *mass;                                                   //massa do objeto ODE


    // --------Geometry
    Vec4         position;                                                //posição inicial do centro do objeto
    Quaternion   rotation;                                                //rotação inicial do objeto
    Vec4         properties;                                              //propriedades de dimensão do objeto
    int          type;                                                    //tipo do objeto em vinculação com o ODE
    Material     *material;                                               //tipo de material do objeto para calculo de desenho e iluminação
    int          id_material;
    float        fmass;                                                   //massa em float na inicialização

    // --------Strategy Equilibrium
    float        compensable;

    // --------Use Cases: Control PD Positional
    bool         show_effector;
    bool         show_target;
    bool         enabled_cpdp;
    bool         collide_ground;
    Vec4         target;
    Vec4         ks;
    Vec4         kd;
    // --------Use Cases: Control Torque Coffee Cup
    bool         has_cup;
    Vec4         kscup;
    Vec4         kdcup;

public:
    Object();
    Object(Scene *scene);
    Object(Vec4 position,Quaternion rotation,Vec4 properties,int type,Scene *scene,QString name="");
    void setCharacter(Character *chara);
    Character* getCharacter();
    ~Object();
    // --------Scene
    void        setSelected(bool b);                                       //seta o objeto como selecionado para possíveis alterações
    bool        isSelected();                                              //verifica se o objeto esta selecionado
    void        setScene(Scene *scene);                                    //adiciona o scene ao qual o objeto está associado
    Scene*      getScene();                                                //verifica a que scene o objeto está
    QString     getName();                                                 //extraí o nome do objeto
    void        setName(QString name);                                     //seta um nome para o objeto
    QString     saveObject();                                              //adiconar parametros do abjeto ao arquivo
    void        setObjFile(QString obj);
    QString     getObjFile();

    // --------Physics
    BodyID      getBody();                                                 //esta parametro de corpo do ODE
    void        setBody(BodyID body);                                      //extraí parâmetro de corpo do ODE
    GeomID      getGeometry();                                             //extraí geom do ODE
    void        setGeometry(GeomID geom);                                  //seta geom do ODE
    Mass*       getMass();                                                 //extraí atributo de massa do objeto
    void        setMass(Mass *mass);                                       //adiciona a massa ao objeto
    void        appTorque(float x, float y, float z);                      //aplicar um torque ao objeto
    void        appTorque(Vec4 *torque);                                   //aplicar um torque ao objeto
    void        appTorque(Vec4 torque);                                   //aplicar um torque ao objeto
    void        addTorque(Vec4 torque);                                   //aplicar um torque ao objeto
    void        addTorque(float x, float y, float z);                      //aplicar um torque ao objeto
    void        addTorque(Vec4 *torque);                                   //aplicar um torque ao objeto
    void        appForce(float x, float y, float z);                       //aplicar uma força ao objeto
    void        appForce(Vec4 *force);                                     //aplicar uma força ao objeto
    void        appForce(Vec4 force);                                     //aplicar uma força ao objeto
    void        addForce(Vec4 force);                                     //aplicar uma força ao objeto
    void        updatePhysics();                                           //atualiza os valores físicos do objeto
    Vec4        getRelVelAngular();                                        //retorna a velocidade angular em coordenadas globais
    Vec4        getRelVelLinear();                                         //retorna a velocidade linear em coordenadas globais
    void        clearPhysics();
    // --------Matrix
    Matrix      getAd(); //calcula matriz Adjunta que muda as coordenadas do frame do mundo para o frame do corpo
    Matrix      getAd(Character* chara);//calcula matriz Adjunta que muda as coordenadas do frame do COM para o frame do corpo
    Matrix      getAd(Vec4 pos);//calcula matriz Adjunta que muda as coordenadas do frame do COM para o frame do corpo
    Matrix      getIM(); //calcula a matriz 6x6 de inercia com a massa do objeto
    // --------Control
    void        setBodyBalance(bool b);
    bool        getBodyBalance();
    void        setCollideWithGround(bool b);
    bool        isCollideWithGround();



    // --------Geometry
    void        setMaterial(int);                                          //aplica um determinado material pré-determinado ao objeto
    int         getIntMaterial();
    Mesh*       getMesh();                                                 //extraí a malha do objeto
    Matrix4x4*  getMatrixTransformation();                                 //extraí a matriz de transformação do objeto (corrente)
    Matrix4x4   getMatrixTransformationODE();                                 //extraí a matriz de transformação do objeto (corrente)
    void        setMaterial(Vec4 amb,Vec4 diff,Vec4 spe,float shininess);  //inclui propriedades de material ao objeto
    void        wireframe();                                               //desenha o objeto como wireframe
    void        draw(bool wire=false);                                                    //desenha o objeto
    void        drawShadow();                                                    //desenha o objeto
    void        draw(Vec4 position,Quaternion q,int mat=-1);
    Vec4        getProperties();                                           //extraí propriedades de escala do objeto
    void        setProperties(Vec4 properties);                            //seta propriedades de escala do objeto
    void        setPosition(Vec4 position);                                //seta posição inicial do centro de massa do objeto
    void        setPositionCurrent(Vec4 position);
    Vec4        getPosition();                                             //extraí posição inicial do objeto
    void        setRotation(Quaternion rotation);                          //seta orientação inicial do objeto
    void        setRotationCurrent(Quaternion rotation);
    Quaternion  getRotation();                                             //extraí rotação inicial do objeto
    int         getType();                                                 //extraí o tipo do objeto
    void        setType(int type);                                         //seta o tipo do objeto
    void        setFMass(float fmass);                                     //seta a massa para o objeto
    float       getFMass();                                                //extraí a massa do objeto
    Vec4        getPositionCurrent();                                      //extraí a posição atual do objeto
    Quaternion  getRotationCurrent();                                      //extraí a posição atual do objeto
    void        setFoot(bool b);                                           //seta se o objeto é um pé ou não
    bool        getFoot();                                                 //extraí se o objeto é um pé ou não
    Vec4        posEffectorForward();
    Vec4        posEffectorBackward();
    static Vec4 posEffectorForward(Vec4 pos,Quaternion rot,Object* obj);
    static Vec4 posEffectorBackward(Vec4 pos,Quaternion rot,Object* obj);
    QString     showInfo();
    void        setRenderMesh(bool b);

    // --------Strategy Equilibrium
    float       getCompensableFactor();
    void        setCompensableFactor(float val);

    // --------Use Cases: Control PD Positional
    void        setShowEffector(bool b);
    bool        isShowEffector();
    void        setShowTarget(bool b);
    bool        isShowTarget();
    void        setEnableCPDP(bool b); //habilitar controle posicional
    bool        isEnableCPDP();
    void        setTarget(Vec4 pos);
    Vec4        getTarget();
    void        setKs(Vec4 pos);
    Vec4        getKs();
    void        setKd(Vec4 pos);
    Vec4        getKd();
    void        evaluate(int val=1);
    // -------Use Cases: Control Torque Coffee Cup
    void        setCoffeeCup(bool b);
    bool        hasCoffeeCup();
    void        setKsCup(Vec4 pos);
    Vec4        getKsCup();
    void        setKdCup(Vec4 pos);
    Vec4        getKdCup();
};



#endif // OBJECT_H
