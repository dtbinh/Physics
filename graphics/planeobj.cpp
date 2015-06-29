#include "planeobj.h"

#define ERRORN 0.0001

PlaneObj::PlaneObj()
{
    //default: side = 1.0
    //configurar o cubo no quadrante positivo do eixo global no canto
    double side = 1.0;
    //lista das normais da face
    vertexs[0].setVec4(-side ,0.0 , side);
    vertexs[1].setVec4( side ,0.0 , side);
    vertexs[2].setVec4( side ,0.0 ,-side);
    vertexs[3].setVec4(-side ,0.0 ,-side);

    initvertexs[0].setVec4(-side ,0.0 , side);
    initvertexs[1].setVec4( side ,0.0 , side);
    initvertexs[2].setVec4( side ,0.0 ,-side);
    initvertexs[3].setVec4(-side ,0.0 ,-side);

    transform.setIdentity();
    refreshNormals();
    mesh = new MeshObj();

    Face face;
    face.vertexs.push_back(&vertexs[0]);
    face.vertexs.push_back(&vertexs[1]);
    face.vertexs.push_back(&vertexs[2]);
    face.vertexs.push_back(&vertexs[3]);
    face.normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
//    face[0].normals.push_back(&normals[0]);
    mesh->faces.push_back(face);
    mesh->setMaterial(4);

    enabled = true;
    selected = false;

}

//calculo das normais dos vertices
void PlaneObj::refreshNormals()
{
    //normais a face

    normals[0] = Vec4::crossProduct(vertexs[1]-vertexs[0],vertexs[3]-vertexs[0]).unitary(); //vertices:0,1,2,3


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

void PlaneObj::draw()
{

    if(isEnabled()){
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    refreshNormals();
    glColor3f(0.5,0.5,0.5);
    for (int i=0;i<4;i++){
        //transform.transpose();
        vertexs[i] = transform.transpose().vector(initvertexs[i]);
    }
    refreshNormals();
    mesh->draw();
    }
    if (selected){

        glColor3f(1,1,1);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        mesh->draw();

    }

}

void PlaneObj::wireframe()
{
    for (int i=0;i<4;i++){
        //transform.transpose();
        vertexs[i] = transform.transpose().vector(initvertexs[i]);
    }
    refreshNormals();
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    mesh->draw();

}



void PlaneObj::aplyTransform(Matrix4x4 t)
{
    transform = transform.multMatrix(transform,t);
    transform = t;
    //mesh->transform = transform;
    //refreshNormals();
}

void PlaneObj::setIdentityTransform()
{
        transform.setIdentity();
}



void PlaneObj::setMaterial(int material)
{
    mesh->setMaterial(material);
}

void PlaneObj::changeCoords(Matrix4x4 matrix)
{
    //transform.showMatrix4x4();
    transform = transform.multMatrix(transform,matrix);
    //transform.showMatrix4x4();
    for (int i=0;i<4;i++){
        //transform.transpose();
        vertexs[i] = transform.transpose().vector(initvertexs[i]);
        //vertexs[i].showVec4();
    }

    refreshNormals();
}

MeshObj * PlaneObj::getMesh()
{
    return mesh;
}

void PlaneObj::setName(QString nm)
{
    name = nm;
}

QString PlaneObj::getName()
{
    return name;
}

int PlaneObj::getIdMaterial()
{
   return mesh->getMaterial();
}


void PlaneObj::setSelected(bool b)
{
    selected = b;
}

bool PlaneObj::isSelected()
{
    return selected;
}

void PlaneObj::setEnabled(bool b)
{
    enabled = b;
}

bool PlaneObj::isEnabled()
{
    return enabled;
}

Matrix4x4 PlaneObj::getMatrixTransformation()
{
    return transform;
}

void PlaneObj::setTransform(Matrix4x4 m)
{
    transform = m;
}

QString PlaneObj::saveObject()
{
    QString obj;
    QString aux;
    obj += "t ";
    Vec4 parameters;
    //translacao
    parameters = transform.getTranslateSeted();
    obj += aux.sprintf("%.3f %.3f %.3f ",parameters.x(),parameters.y(),parameters.z());
    //escala
    parameters = transform.getScaleSeted();
    obj += aux.sprintf("%.3f %.3f %.3f ",parameters.x(),parameters.y(),parameters.z());
    //rotação
    parameters = transform.getRotationSeted();
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

PlaneObj::~PlaneObj()
{
    delete []mesh;
}
