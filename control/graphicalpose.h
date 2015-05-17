#ifndef GRAPHICALPOSE_H
#define GRAPHICALPOSE_H

#include "scene/character.h"
#include "pose.h"
#include <vector>

class GraphicalPose
{
private:

    Character*          character;
    std::vector<Pose*>  poses;
    std::vector<double> timeIntervals;
    double              time;
    unsigned int        current;

public:
    GraphicalPose(std::vector<Pose*> poses, std::vector<double> timeIntervals);
    GraphicalPose(Character* character);

    Character*          getCharacter();
    std::vector<Pose*>  getPoses();
    std::vector<double> getTimeIntervals();
    void                pushBackPose(Pose* newPose, double poseInterval); //Adiciona uma nova pose ao final da lista de poses, caso o character da pose seja o mesmo que o character do controle de pose
    void                insertPose(Pose* newPose, double poseInterval, int position); //Adiciona uma nova pose a uma posição específica da lista de poses, caso o character da pose seja o mesmo que o character do controle de pose
    void                modifyPose(Pose* modifiedPose, double poseInterval, int position); //Modifica a pose na posição position, caso exista
    Pose*               getCurrentPose(); //retorna o estado atual do controle
    void                advanceTime(double increment); //Função que avança o tempo interno que o controle passou na posição atual
    void                setCharacterShape(); //Função que atualiza os controladores do personagem de acordo com a pose corrente
};

#endif // GRAPHICALPOSE_H