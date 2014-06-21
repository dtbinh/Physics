#ifndef OBJMESH_H
#define OBJMESH_H
#include <math/vec4.h>
#include <vector>
#include <string>

class ObjMesh
{
public:
    ObjMesh(const std::string &fileName);

    std::string getName();

    bool hasTextureCoords();




    virtual void draw();

private:
    struct Face{
        Vec4* vertices[3];
        Vec4* normals[3];
        Vec4* textureCoord[3];
    };

    std::string m_name;

    std::vector<Vec4> vertices;
    std::vector<Vec4> normals;
    std::vector<Vec4> textureCoord;
    std::vector<Face> faces;
    bool hasTextureCoord;

    int displayListId;
};

#endif

