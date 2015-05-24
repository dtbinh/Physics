#ifndef POSE_H
#define POSE_H

#include "scene/character.h"
#include "scene/joint.h"
#include "math/vec4.h"
#include <vector>
#include <QString>

class Pose
{
private:
    Character*          character;
    std::vector<Vec4>   angles;
    QString             name;
public:
    Pose(Character* character);
    Pose(Character *character, std::vector<Vec4> angles);
    ~Pose();

    Character*          getCharacter(); //retorna o caractere a que se referencia aquela pose
    std::vector<Vec4>   getAngles();    //retorna o vetor de ângulos da pose
    Vec4                getAngleJoint(int searchIndex);  //retorna o ângulo correspondente a uma determinada junta
    Vec4                getAngleJoint(Joint* searchJoint);
    void                updateAngle(int updateIndex, Vec4 newAngle); //atualiza o ângulo de uma determinada junta do personagem
    void                updateAngle(Joint* updatedJoint, Vec4 newAngle);
    void                setName(QString name); //getters e setters para o nome da pose
    QString             getName();
    void                setCharacterShape(); //muda a pose do personagem de acordo com os ângulos internos
    Pose*               interpolateWith(Pose* next, double instant, double interval); //recebe duas poses e retorna uma nova pose que corresponde à interpolação delas no instante x do intervalo
    void                interpolateAndApply(Pose* next, double instant, double interval); //recebe duas poses e coloca o personagem como uma interpolação entre as duas poses
};

#endif // POSE_H
