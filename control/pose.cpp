#include "pose.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "math/vec4.h"
#include <vector>
#include <cstdio>

Pose::Pose(Character *character)
{
    if (character != NULL){
        this->character = character;
    }
}

Pose::Pose(Character *character, std::vector<Vec4> angles)
{
    if (character != NULL){
        this->character = character;
        int numJoints = this->character->getNumJoints();

        if (numJoints != angles.size()){
            printf("Quantidade de ângulos diferente da quantidade de juntas! Ângulos não inicializados\n");
        } else {
            this->angles = angles;
        }
    }
}

Character *Pose::getCharacter()
{
    return this->character;
}

std::vector<Vec4> Pose::getAngles()
{
    return this->angles;
}

Vec4 Pose::getAngleJoint(int searchIndex)
{
    if (this->angles.size() > searchIndex){
        return this->angles.at(searchIndex);
    }
}

Vec4 Pose::getAngleJoint(Joint *searchJoint)
{
    if (this->angles.size() != 0 && this->character != NULL){
        int positionJoint = this->character->findJointIndex(searchJoint);
        if (positionJoint >= 0) {
            return this->angles.at(positionJoint);
        }
    }
}

void Pose::updateAngle(int updateIndex, Vec4 newAngle)
{
    if (this->angles.size() > updateIndex) {
        this->angles.at(updateIndex) = newAngle;
    }
}

void Pose::updateAngle(Joint *updatedJoint, Vec4 newAngle)
{
    if (this->angles.size() != 0 && this->character != NULL){
        int positionJoint = this->character->findJointIndex(updatedJoint);
        if (positionJoint >= 0){
            this->angles.at(positionJoint) = newAngle;
        }
    }
}

