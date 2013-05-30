#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include "physics/physics.h"
#include "math/vec4.h"
#include "math/quaternion.h"

class Material;
class Matrix4x4;
class Mesh;

#define TYPE_CUBE      dBoxClass
#define TYPE_CYLINDER  dCylinderClass
#define TYPE_SPHERE    dSphereClass


class Object
{
private:
    // --------Scene
    Scene        *scene;
    QString      name;
    bool         selected;

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
    float        fmass;                                                   //massa em float na inicialização

public:
    Object();
    Object(Scene *scene);
    Object(Vec4 position,Quaternion rotation,Vec4 properties,int type,Scene *scene,QString name="");
    ~Object();
    // --------Scene
    void        setSelected(bool b);                                       //seta o objeto como selecionado para possíveis alterações
    bool        isSelected();                                              //verifica se o objeto esta selecionado
    void        setScene(Scene *scene);                                    //adiciona o scene ao qual o objeto está associado
    Scene*      getScene();                                                //verifica a que scene o objeto está
    QString     getName();                                                 //extraí o nome do objeto
    void        setName(QString name);                                     //seta um nome para o objeto
    QString     saveObject();                                              //adiconar parametros do abjeto ao arquivo

    // --------Physics
    BodyID      getBody();                                                 //esta parametro de corpo do ODE
    void        setBody(BodyID body);                                      //extraí parâmetro de corpo do ODE
    GeomID      getGeometry();                                             //extraí geom do ODE
    void        setGeometry(GeomID geom);                                  //seta geom do ODE
    Mass*       getMass();                                                 //extraí atributo de massa do objeto
    void        setMass(Mass *mass);                                       //adiciona a massa ao objeto
    void        appTorque(float x, float y, float z);                      //aplicar um torque ao objeto
    void        appTorque(Vec4 *torque);                                   //aplicar um torque ao objeto
    void        appForce(float x, float y, float z);                       //aplicar uma força ao objeto
    void        appForce(Vec4 *force);                                     //aplicar uma força ao objeto

    // --------Geometry
    void        setMaterial(int);                                          //aplica um determinado material pré-determinado ao objeto
    Mesh*       getMesh();                                                 //extraí a malha do objeto
    Matrix4x4*  getMatrixTransformation();                                 //extraí a matriz de transformação do objeto (corrente)
    void        setMaterial(Vec4 amb,Vec4 diff,Vec4 spe,float shininess);  //inclui propriedades de material ao objeto
    void        wireframe();                                               //desenha o objeto como wireframe
    void        draw();                                                    //desenha o objeto
    Vec4        getProperties();                                           //extraí propriedades de escala do objeto
    void        setProperties(Vec4 properties);                            //seta propriedades de escala do objeto
    void        setPosition(Vec4 position);                                //seta posição inicial do centro de massa do objeto
    Vec4        getPosition();                                             //extraí posição inicial do objeto
    void        setRotation(Quaternion rotation);                          //seta orientação inicial do objeto
    Quaternion  getRotation();                                             //extraí rotação inicial do objeto
    int         getType();                                                 //extraí o tipo do objeto
    void        setType(int type);                                         //seta o tipo do objeto
    void        setFMass(float fmass);                                     //seta a massa para o objeto
    float       getFMass();                                                //estraí a massa do objeto
    Vec4        getPositionCurrent();                                      //extraí a posição atual do objeto
    Quaternion  getRotationCurrent();                                      //extraí a posição atual do objeto

};



#endif // OBJECT_H
