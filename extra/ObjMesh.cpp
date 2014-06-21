#include "ObjMesh.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <GL/gl.h>

#include <QFileInfo>

int up = 1;
using namespace std;


ObjMesh::ObjMesh(const string &fileName)
{
    QFileInfo info(QString::fromStdString(fileName));
    m_name = info.baseName().toStdString();


    hasTextureCoord = false;

    ifstream file(fileName.data(), ios::in);
    if(!file)
    {
        cerr << "File \"" << fileName << "\" not found." << endl;
        return;
    }

    string type;
    while(file >> type)
    {
        if(type == "v")
        {
            float x, y, z;
            file >> x >> y >> z;

            Vec4 v(x, y, z);
            vertices.push_back(v);

        }
        else if(type == "vt")
        {
            float u, v;
            file >> u >> v;

            Vec4 p(u, v);
            textureCoord.push_back(p);

            hasTextureCoord = true;
        }
        else if(type == "vn")
        {
            float x, y, z;
            file >> x >> y >> z;

            normals.push_back(Vec4(x, y, z));
        }
        else if(type == "f")
        {
            int vi[3];
            int ni[3];
            int ti[3];
            string token;

            file >> token;
            int count = sscanf(token.c_str(), "%d/%d/%d", &vi[0], &ti[0], &ni[0]);
            if(count == 1)
            {
                count = sscanf(token.c_str(), "%d//%d", &vi[0], &ni[0]);
                if(count == 1)
                    cerr << "Objeto sem normais" << endl;
            }

            file >> token;
            count = sscanf(token.c_str(), "%d/%d/%d", &vi[1], &ti[1], &ni[1]);
            if(count == 1)
            {
                count = sscanf(token.c_str(), "%d//%d", &vi[1], &ni[1]);
                if(count == 1)
                    cerr << "Objeto sem normais" << endl;
            }

            file >> token;
            count = sscanf(token.c_str(), "%d/%d/%d", &vi[2], &ti[2], &ni[2]);
            if(count == 1)
            {
                count = sscanf(token.c_str(), "%d//%d", &vi[2], &ni[2]);
                if(count == 1)
                    cerr << "Objeto sem normais" << endl;
            }

            Face face;
            face.vertices[0] = &vertices[vi[0]-1];
            face.vertices[1] = &vertices[vi[1]-1];
            face.vertices[2] = &vertices[vi[2]-1];
            face.normals[0] = &normals[ni[0]-1];
            face.normals[1] = &normals[ni[1]-1];
            face.normals[2] = &normals[ni[2]-1];
            face.textureCoord[0] = &textureCoord[ti[0]-1];
            face.textureCoord[1] = &textureCoord[ti[1]-1];
            face.textureCoord[2] = &textureCoord[ti[2]-1];

            faces.push_back(face);
        }
    }

    displayListId = glGenLists(up);
    glNewList(displayListId, GL_COMPILE);
    if(hasTextureCoord)
    {
        glBegin(GL_TRIANGLES);
        for(unsigned int i=0; i<faces.size(); ++i)
        {
            glNormal3fv(&faces[i].normals[0]->x1);
            glTexCoord2fv(&faces[i].textureCoord[0]->x1);
            glVertex3fv(&faces[i].vertices[0]->x1);

            glNormal3fv(&faces[i].normals[1]->x1);
            glTexCoord2fv(&faces[i].textureCoord[1]->x1);
            glVertex3fv(&faces[i].vertices[1]->x1);

            glNormal3fv(&faces[i].normals[2]->x1);
            glTexCoord2fv(&faces[i].textureCoord[2]->x1);
            glVertex3fv(&faces[i].vertices[2]->x1);
        }
        glEnd();
    }
    else
    {
        glBegin(GL_TRIANGLES);
        for(unsigned int i=0; i<faces.size(); ++i)
        {
            glNormal3fv(&faces[i].normals[0]->x1);
            glVertex3fv(&faces[i].vertices[0]->x1);

            glNormal3fv(&faces[i].normals[1]->x1);
            glVertex3fv(&faces[i].vertices[1]->x1);

            glNormal3fv(&faces[i].normals[2]->x1);
            glVertex3fv(&faces[i].vertices[2]->x1);
        }
        glEnd();
    }
    glEndList();
    up++;
}


void ObjMesh::draw()
{
    glCallList(displayListId);
}


string ObjMesh::getName()
{
    return m_name;
}

bool ObjMesh::hasTextureCoords()
{
    return hasTextureCoord;
}
