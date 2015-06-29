#ifndef MATERIALOBJ_H
#define MATERIALOBJ_H
#include <GL/glut.h>
#include "math/vec4.h"
#include <QColor>
//tipos pedra
#define MATERIAL_EMERALD        0
#define MATERIAL_JADE           1
#define MATERIAL_OBSIDIAN       2
#define MATERIAL_PEARL          3

#define MATERIAL_TURQUOSIE      4
//tipos metal
#define MATERIAL_BRONZE         5
#define MATERIAL_BRONZE_POLIERT 6
#define MATERIAL_CHROME         7
#define MATERIAL_GOLD           8
#define MATERIAL_GOLD_POLIERT   9
#define MATERIAL_COPPER         10 //KUPFER
#define MATERIAL_COPPER_POLIERT 11 //KUPFER
#define MATERIAL_BRASS          12 //MESSING
#define MATERIAL_ZINN           13
//tipos kunstoff
#define MATERIAL_GUMMI          14
//cores

#define MATERIAL_CYAN_PLASTIC   15
#define MATERIAL_GREEN_PLASTIC  16
#define MATERIAL_RED_PLASTIC    17
#define MATERIAL_YELLOW_PLASTIC 18
#define MATERIAL_CYAN_RUBBER    19
#define MATERIAL_GREEN_RUBER    20
#define MATERIAL_RED_RUBBER     21
#define MATERIAL_YELLOW_RUBBER  22

//outras utilidades
#define MATERIAL_WHITE_PLASTIC  23
#define MATERIAL_SILVER_POLIERT 24
#define MATERIAL_RUBY           25
#define MATERIAL_BLACK_PLASTIC  26
#define MATERIAL_WHITE_RUBBER   27
#define MATERIAL_PLASTIK        28
#define MATERIAL_BLACK_RUBBER   29
#define MATERIAL_SILVER         30
#define MATERIAL_ICE            31

class MaterialObj
{
public:
    MaterialObj();

public:
 float ambient[4];
 float diffuse[4];
 float specular[4];
 float shininess;

 void static setMaterial(MaterialObj *mat,int type);
 void static setMaterial(MaterialObj *mat,Vec4 ambient,Vec4 diffuse,Vec4 specular, float shininess);
 void static setMaterialOpenGL(int type);
 MaterialObj* getMaterial(int type);
 QColor static getColorMaterial(int type);
 Vec4 ambientMaterial();
 Vec4 specularMaterial();
 Vec4 diffuseMaterial();
 float shininessMaterial();


};

#endif // MATERIALOBJ_H
