#ifndef GRAPHICALPOSE_H
#define GRAPHICALPOSE_H

#include "scene/character.h"
#include "pose.h"
#include <vector>

class GraphicalPose
{
private:

    Character* character;
    std::vector<Pose*> poses;
    std::vector<double> timeIntervals;
    double time;

public:
    GraphicalPose(std::vector<Pose*> poses, std::vector<double> timeIntervals);
    GraphicalPose(Character* character);

    Character*          getCharacter();
    std::vector<Pose*>  getPoses();
    std::vector<double> getTimeIntervals();
    void                pushBackPose(Pose* newPose, double poseInterval); //Adiciona uma nova pose ao final da lista de poses, caso o character da pose seja o mesmo que o character do controle de pose
    void                insertPose(Pose* newPose, double poseInterval, int position); //Adiciona uma nova pose a uma posição específica da lista de poses, caso o character da pose seja o mesmo que o character do controle de pose
    void                advanceTime(double increment); //Função que avança o tempo interno do controle de pose no intervalo especificado

};

#endif // GRAPHICALPOSE_H
