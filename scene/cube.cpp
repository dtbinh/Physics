#include "cube.h"
#include "object.h"
#include "math/matrix4x4.h"
#include "graphics/mesh.h"
#include "math/quaternion.h"
#include "scene.h"
#define ERRORN 0.0001
float side = 1.0;
Cube::Cube()
{
    //default: side = 1.0
    //configurar o cubo no quadrante positivo do eixo global no canto
    double side = 1.0;
    //lista das normais da face
    vertexs[0].setVec4((-0.5)*side  ,(-0.5)*side  ,(-0.5)*side );
    vertexs[1].setVec4((-0.5)*side  ,(-0.5)*side  ,(0.5)*side);
    vertexs[2].setVec4((0.5)*side ,(-0.5)*side  ,(0.5)*side);
    vertexs[3].setVec4((0.5)*side ,(-0.5)*side  ,(-0.5)*side );
    vertexs[4].setVec4((-0.5)*side  ,(0.5)*side  ,(-0.5)*side );
    vertexs[5].setVec4((-0.5)*side  ,(0.5)*side ,(0.5)*side);
    vertexs[6].setVec4((0.5)*side ,(0.5)*side ,(0.5)*side);
    vertexs[7].setVec4((0.5)*side ,(0.5)*side ,(-0.5)*side );
    initvertexs[0].setVec4((-0.5)*side  ,(-0.5)*side  ,(-0.5)*side );
    initvertexs[1].setVec4((-0.5)*side  ,(-0.5)*side  ,(0.5)*side);
    initvertexs[2].setVec4((0.5)*side ,(-0.5)*side  ,(0.5)*side);
    initvertexs[3].setVec4((0.5)*side ,(-0.5)*side  ,(-0.5)*side );
    initvertexs[4].setVec4((-0.5)*side  ,(0.5)*side  ,(-0.5)*side );
    initvertexs[5].setVec4((-0.5)*side  ,(0.5)*side ,(0.5)*side);
    initvertexs[6].setVec4((0.5)*side ,(0.5)*side ,(0.5)*side);
    initvertexs[7].setVec4((0.5)*side ,(0.5)*side ,(-0.5)*side );
    transform->setIdentity();
    mesh = new Mesh();

    Face face[6];
    face[0].vertexs.push_back(&vertexs[0]);
    face[0].vertexs.push_back(&vertexs[3]);
    face[0].vertexs.push_back(&vertexs[2]);
    face[0].vertexs.push_back(&vertexs[1]);
    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
    mesh->faces.push_back(face[0]);
    face[1].vertexs.push_back(&vertexs[4]);
    face[1].vertexs.push_back(&vertexs[5]);
    face[1].vertexs.push_back(&vertexs[6]);
    face[1].vertexs.push_back(&vertexs[7]);
    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
    mesh->faces.push_back(face[1]);
    face[2].vertexs.push_back(&vertexs[2]);
    face[2].vertexs.push_back(&vertexs[6]);
    face[2].vertexs.push_back(&vertexs[5]);
    face[2].vertexs.push_back(&vertexs[1]);
    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
    mesh->faces.push_back(face[2]);
    face[3].vertexs.push_back(&vertexs[3]);
    face[3].vertexs.push_back(&vertexs[7]);
    face[3].vertexs.push_back(&vertexs[6]);
    face[3].vertexs.push_back(&vertexs[2]);
    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
    mesh->faces.push_back(face[3]);
    face[4].vertexs.push_back(&vertexs[0]);
    face[4].vertexs.push_back(&vertexs[4]);
    face[4].vertexs.push_back(&vertexs[7]);
    face[4].vertexs.push_back(&vertexs[3]);
    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
    mesh->faces.push_back(face[4]);
    face[5].vertexs.push_back(&vertexs[4]);
    face[5].vertexs.push_back(&vertexs[0]);
    face[5].vertexs.push_back(&vertexs[1]);
    face[5].vertexs.push_back(&vertexs[5]);
    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
    mesh->faces.push_back(face[5]);
    enabled = true;
    selected = false;

}

Cube::Cube(Scene *scene)
{
    this->scene = scene;

    transform = new Matrix4x4();
    transform->setIdentity();

    position = Vec4();
    rotation = Quaternion();
    this->mass = new Mass();

    //material = new Material();
    vertexs[0].setVec4((-0.5)*side  ,(-0.5)*side  ,(-0.5)*side );
    vertexs[1].setVec4((-0.5)*side  ,(-0.5)*side  ,(0.5)*side);
    vertexs[2].setVec4((0.5)*side ,(-0.5)*side  ,(0.5)*side);
    vertexs[3].setVec4((0.5)*side ,(-0.5)*side  ,(-0.5)*side );
    vertexs[4].setVec4((-0.5)*side  ,(0.5)*side  ,(-0.5)*side );
    vertexs[5].setVec4((-0.5)*side  ,(0.5)*side ,(0.5)*side);
    vertexs[6].setVec4((0.5)*side ,(0.5)*side ,(0.5)*side);
    vertexs[7].setVec4((0.5)*side ,(0.5)*side ,(-0.5)*side );
    initvertexs[0].setVec4((-0.5)*side  ,(-0.5)*side  ,(-0.5)*side );
    initvertexs[1].setVec4((-0.5)*side  ,(-0.5)*side  ,(0.5)*side);
    initvertexs[2].setVec4((0.5)*side ,(-0.5)*side  ,(0.5)*side);
    initvertexs[3].setVec4((0.5)*side ,(-0.5)*side  ,(-0.5)*side );
    initvertexs[4].setVec4((-0.5)*side  ,(0.5)*side  ,(-0.5)*side );
    initvertexs[5].setVec4((-0.5)*side  ,(0.5)*side ,(0.5)*side);
    initvertexs[6].setVec4((0.5)*side ,(0.5)*side ,(0.5)*side);
    initvertexs[7].setVec4((0.5)*side ,(0.5)*side ,(-0.5)*side );

    transform->setIdentity();
    refreshNormals();
    mesh = new Mesh();

    Face face[6];
    face[0].vertexs.push_back(&vertexs[0]);
    face[0].vertexs.push_back(&vertexs[3]);
    face[0].vertexs.push_back(&vertexs[2]);
    face[0].vertexs.push_back(&vertexs[1]);
    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
    mesh->faces.push_back(face[0]);
    face[1].vertexs.push_back(&vertexs[4]);
    face[1].vertexs.push_back(&vertexs[5]);
    face[1].vertexs.push_back(&vertexs[6]);
    face[1].vertexs.push_back(&vertexs[7]);
    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
//    face[1].normals.push_back(&normals[1]);
    mesh->faces.push_back(face[1]);
    face[2].vertexs.push_back(&vertexs[2]);
    face[2].vertexs.push_back(&vertexs[6]);
    face[2].vertexs.push_back(&vertexs[5]);
    face[2].vertexs.push_back(&vertexs[1]);
    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
//    face[2].normals.push_back(&normals[2]);
    mesh->faces.push_back(face[2]);
    face[3].vertexs.push_back(&vertexs[3]);
    face[3].vertexs.push_back(&vertexs[7]);
    face[3].vertexs.push_back(&vertexs[6]);
    face[3].vertexs.push_back(&vertexs[2]);
    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
//    face[3].normals.push_back(&normals[3]);
    mesh->faces.push_back(face[3]);
    face[4].vertexs.push_back(&vertexs[0]);
    face[4].vertexs.push_back(&vertexs[4]);
    face[4].vertexs.push_back(&vertexs[7]);
    face[4].vertexs.push_back(&vertexs[3]);
    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
//    face[4].normals.push_back(&normals[5]);
    mesh->faces.push_back(face[4]);
    face[5].vertexs.push_back(&vertexs[4]);
    face[5].vertexs.push_back(&vertexs[0]);
    face[5].vertexs.push_back(&vertexs[1]);
    face[5].vertexs.push_back(&vertexs[5]);
    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
//    face[5].normals.push_back(&normals[4]);
    mesh->faces.push_back(face[5]);
    enabled = true;
    selected = false;
}

Cube::~Cube()
{
}


//calculo das normais dos vertices
void Cube::refreshNormals()
{
    //normais a face

    normals[0] = Vec4::crossProduct(vertexs[3]-vertexs[0],vertexs[2]-vertexs[3]).unitary(); //vertices:0,1,2,3
    normals[1] = Vec4::crossProduct(vertexs[4]-vertexs[7],vertexs[5]-vertexs[4]).unitary(); //vertices:4,5,6,7
    normals[2] = Vec4::crossProduct(vertexs[6]-vertexs[2],vertexs[5]-vertexs[6]).unitary(); //vertices:1,2,5,6
    normals[3] = Vec4::crossProduct(vertexs[7]-vertexs[3],vertexs[6]-vertexs[7]).unitary(); //vertices:2,3,6,7
    normals[4] = Vec4::crossProduct(vertexs[4]-vertexs[5],vertexs[0]-vertexs[4]).unitary(); //vertices:0,1,4,5
    normals[5] = Vec4::crossProduct(vertexs[4]-vertexs[0],vertexs[7]-vertexs[4]).unitary(); //vertices:0,3,4,7

//    //normal do vertice é a média das normais as faces que o vertice pertence
//    normals[0] = ((normal[0]+normal[4]+normal[5])).unitary();
//    normals[1] = ((normal[0]+normal[2]+normal[4])).unitary();
//    normals[2] = ((normal[0]+normal[2]+normal[3])).unitary();
//    normals[3] = ((normal[0]+normal[3]+normal[5])).unitary();
//    normals[4] = ((normal[1]+normal[4]+normal[5])).unitary();
//    normals[5] = ((normal[1]+normal[2]+normal[4])).unitary();
//    normals[6] = ((normal[1]+normal[2]+normal[3])).unitary();
//    normals[7] = ((normal[1]+normal[3]+normal[5])).unitary();
}

void Cube::draw()
{
    //printf("oi");

    Physics::getGeomTransform(geometry, transform); //important!
//   transform->showMatrix4x4();
//   printf("\n");
   // if(isEnabled()){
    glColor3f(1,0.5,0);
    mesh->setMaterial(MATERIAL_BLACK_RUBBER);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    //refreshNormals();
    for (int i=0;i<8;i++){
        //transform.transpose();
        vertexs[i] = transform->transpose().vector(initvertexs[i]);
    }
    refreshNormals();
    mesh->draw();
    //}
//    if (selected){

//        glColor3f(1,1,1);
//        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//        mesh->draw();

//    }

}

void Cube::wireframe()
{
    for (int i=0;i<8;i++){
        //transform.transpose();
        vertexs[i] = transform->transpose().vector(initvertexs[i]);
    }
    refreshNormals();
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    mesh->draw();

}



//void Cube::aplyTransform(Matrix4x4 t)
//{
//    transform = transform->multMatrix(transform,t);
//    transform = t;
//    //mesh->transform = transform;
//    //refreshNormals();
//}

void Cube::setIdentityTransform()
{
        transform->setIdentity();
}



void Cube::setMaterial(int material)
{
    mesh->setMaterial(material);
}

//void Cube::changeCoords(Matrix4x4 matrix)
//{
//    //transform.showMatrix4x4();
//    transform = transform->multMatrix(transform,matrix);
//    //transform.showMatrix4x4();
//    for (int i=0;i<8;i++){
//        //transform.transpose();
//        vertexs[i] = transform->transpose().vector(initvertexs[i]);
//        //vertexs[i].showVec4();
//    }

//    refreshNormals();
//}

Mesh * Cube::getMesh()
{
    return mesh;
}

void Cube::setName(QString nm)
{
    name = nm;
}

QString Cube::getName()
{
    return name;
}

int Cube::getIdMaterial()
{
   return mesh->getMaterial();
}



//void Cube::tryIntersection(RayIntersection *intersect,Ray ray)
//{
//    float pmax[3] = {-INFINITY};
//    float pmin[3] = {INFINITY};
//    Vec4 bounds[2];
//    for (int i=0;i<8;i++){
//        if(pmin[0]>=vertexs[i].x1 && pmin[1]>=vertexs[i].x2 && pmin[2]>=vertexs[i].x3){
//            pmin[0] = vertexs[i].x1;
//            pmin[1] = vertexs[i].x2;
//            pmin[2] = vertexs[i].x3;
//            bounds[0] = vertexs[i];
//        }
//        if(pmax[0]<=vertexs[i].x1 && pmax[1]<=vertexs[i].x2 && pmax[2]<=vertexs[i].x3){
//            pmax[0] = vertexs[i].x1;
//            pmax[1] = vertexs[i].x2;
//            pmax[2] = vertexs[i].x3;
//            bounds[1] = vertexs[i];
//        }
//    }
//    intersect->rayBoxIntersection(mesh,ray,transform,bounds[1],bounds[0]);

//}

void Cube::setSelected(bool b)
{
    selected = b;
}

bool Cube::isSelected()
{
    return selected;
}

void Cube::setEnabled(bool b)
{
    enabled = b;
}

bool Cube::isEnabled()
{
    return enabled;
}

Matrix4x4* Cube::getMatrixTransformation()
{
    return transform;
}



int Cube::getNumFaces()
{
    return mesh->faces.size();
}

int Cube::getNumVertexs()
{
    return 8;
}

void Cube::setTransform(Matrix4x4* m)
{
    transform = m;
}

QString Cube::saveObject()
{
    QString obj;
    QString aux;
    obj += "c ";
    Vec4 parameters;
    //translacao
    parameters = transform->getTranslateSeted();
    obj += aux.sprintf("%.3f %.3f %.3f ",parameters.x(),parameters.y(),parameters.z());
    //escala
    parameters = transform->getScaleSeted();
    obj += aux.sprintf("%.3f %.3f %.3f ",parameters.x(),parameters.y(),parameters.z());
    //rotação
    parameters = transform->getRotationSeted();
    obj += aux.sprintf("%.3f %.3f %.3f ",parameters.x(),parameters.y(),parameters.z());
    obj += aux.sprintf("%d ",this->getIdMaterial());
    if (this->enabled)
        obj += "t ";
    else
        obj += "f ";
    if (this->selected)
        obj += "t ";
    else
        obj += "f ";
    obj +=this->name+"\n";
    return obj;


}

void Cube::setProperties(Vec4 v)
{
    properties.setVec4(v.x(),v.y(),v.z());
    Matrix4x4 *m = new Matrix4x4();
    m->scale(v.x(),v.y(),v.z());
    for(int i=0;i<8;i++){
         initvertexs[i] = m->transpose().vector(initvertexs[i]);
    }

    //this->setTransform (m);
}

