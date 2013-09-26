#ifndef CUBE_H
#define CUBE_H

#include "object.h"
#include "math/quaternion.h"
class Matrix4x4;
class Mesh;

class Scene;

#include "physics/physics.h"
#include <vector>

class Cube: public Object
{

public:
    Scene *scene;

    //------------------
    // Physics

    BodyID body;
    GeomID geometry;
    Mass *mass;

    Mesh* mesh;
    Vec4 vertexs[8];
    Vec4 initvertexs[8];
    Vec4 normals[6];
    Vec4 initnormals[8];
    Matrix4x4 *transform;
    QString name;
    Vec4 position; //posição do centro do objeto
    Quaternion rotation; //rotação do objeto
    bool selected;
    bool enabled;
    Vec4 properties;

public:
    Cube();
    Cube(Scene *scene);
    ~Cube();

    //void draw();
    //void drawSelected();
    void draw();
    void aplyTransform(Matrix4x4 t){};
    void refreshNormals();
    void setIdentityTransform();
    void setMaterial(int material);
    void wireframe();
    void changeCoords(Matrix4x4 matrix){};
    Mesh* getMesh();
    void setName(QString nm);
    QString getName();
    int getIdMaterial();
    //void tryIntersection(RayIntersection *intersect,Ray ray);
    void setSelected(bool b);
    bool isSelected();
    void setEnabled(bool b);
    bool isEnabled();
    Matrix4x4* getMatrixTransformation();
    int getNumFaces();
    int getNumVertexs();
    void setTransform(Matrix4x4 *m);
    QString saveObject();
    BodyID getBody(){ return this->body;}
    void        setBody(BodyID body) {this->body = body;}
    GeomID      getGeometry() {return this->geometry;}
    void       setGeometry(GeomID geom){this->geometry = geom;}
    //void        setMass(Mass mass){ this->mass = mass;}
    Mass*        getMass(){return this->mass;}
    Scene*      getScene(){return this->scene;}
    Vec4        getProperties(){return Vec4(this->properties.x(),this->properties.y(),this->properties.z())  ;}
    void setPosition(Vec4 p){this->position = p ;}
    void setRotation(Quaternion q) {this->rotation = q;}
    Vec4 getPosition(){return this->position;}
    Quaternion getRotation(){ return this->rotation ;}
    void setProperties(Vec4 v);



};


#endif // CUBE_H
