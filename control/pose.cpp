#include "pose.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "control.h"
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

void Pose::setName(QString name)
{
    this->name = name;
}

QString Pose::getName()
{
    return this->name;
}

void Pose::setCharacterShape()
{
    std::vector<Vec4> poseVectorActual = this->angles;

    for (int i = 0; i < poseVectorActual.size(); i++){
        ControlPD* jointController = this->character->getController(i);
        jointController->setQuaternionWanted(Quaternion(poseVectorActual.at(i)));
    }
}

Pose *Pose::interpolateWith(Pose *next, double instant, double interval)
{
    std::vector<Vec4> interpolatedAngles;

    for (int i = 0; i < this->angles.size(); i++){
        Vec4 angle;
        angle.setX( this->angles[i].x() + (interval - instant) * (next->getAngles()[i].x() - this->angles[i].x())/interval );
        angle.setY( this->angles[i].y() + (interval - instant) * (next->getAngles()[i].y() - this->angles[i].y())/interval );
        angle.setZ( this->angles[i].z() + (interval - instant) * (next->getAngles()[i].z() - this->angles[i].z())/interval );

        interpolatedAngles.push_back(angle);
    }

    return new Pose(this->character, interpolatedAngles);
}
