#include "graphicalpose.h"
#include "pose.h"
#include "control.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

GraphicalPose::GraphicalPose(std::vector<Pose*> poses, std::vector<double> timeIntervals)
{
    this->character = poses.at(0)->getCharacter();
    this->poses = poses;
    this->timeIntervals = timeIntervals;

    //Cria o vetor de intervalos de tempo cumulativos
    double sum = 0.0;
    for (int i = 0; i < timeIntervals.size(); i++){
        this->cumulativeTimeIntervals[i] = sum;
        sum += timeIntervals.at(i);
    }
}

GraphicalPose::GraphicalPose(Character *character)
{
    this->character = character;
}

GraphicalPose::~GraphicalPose()
{
}

Character *GraphicalPose::getCharacter()
{
    return this->character;
}

std::vector<Pose *> GraphicalPose::getPoses()
{
    return this->poses;
}

std::vector<double> GraphicalPose::getTimeIntervals()
{
    return this->timeIntervals;
}

std::vector<double> GraphicalPose::getCumulativeTimeIntervals()
{
    return this->cumulativeTimeIntervals;
}


void GraphicalPose::pushBackPose(Pose *newPose, double poseInterval)
{
    if (newPose->getCharacter() == this->character) {
        this->poses.push_back(newPose);
        this->timeIntervals.push_back(poseInterval);

        double nextCumulativeTime = 0;
        if (cumulativeTimeIntervals.size() > 0) {
            nextCumulativeTime = this->cumulativeTimeIntervals.at(this->cumulativeTimeIntervals.size()-1) + this->timeIntervals.at(this->timeIntervals.size()-2);
        }
        this->cumulativeTimeIntervals.push_back(nextCumulativeTime);
    }
}

void GraphicalPose::insertPose(Pose *newPose, double poseInterval, int position)
{
    if ((newPose->getCharacter() == this->character) && (this->poses.size() > position)) {

        this->poses.insert(this->poses.begin()+position, newPose);
        this->timeIntervals.insert(this->timeIntervals.begin()+position, poseInterval);

        this->cumulativeTimeIntervals.push_back(0);
        int arraySize = this->timeIntervals.size();
        for (int i = position + 1; i < arraySize; ++i){
            this->cumulativeTimeIntervals.at(i) = this->cumulativeTimeIntervals.at(i-1) + this->timeIntervals.at(i-1);
        }
    }
}

void GraphicalPose::modifyPose(Pose *modifiedPose, double poseInterval, int position)
{
    if ((modifiedPose->getCharacter() == this->character) && (this->poses.size() > position)){
        this->poses.at(position) = modifiedPose;
        this->timeIntervals.at(position) = poseInterval;

        int arraySize = this->timeIntervals.size();
        for (int i = position + 1; i < arraySize; ++i){
            this->cumulativeTimeIntervals.at(i) = this->cumulativeTimeIntervals.at(i-1) + this->timeIntervals.at(i-1);
        }
    }
}

Pose *GraphicalPose::getCurrentPose()
{
    return this->poses.at(this->current);
}

void GraphicalPose::advanceTimeEnergic(double increment)
{
    if (this->timeIntervals.size() > 0) {
        if (this->time+increment > this->timeIntervals.at(this->current)){
            this->current += 1;

            if (this->current >= this->timeIntervals.size()){
                this->current = 0;
            }

            this->time = 0;
            //printf("current state is: %d\n", this->current);
            this->setCharacterShape();

        } else {
            this->time += increment;
        }
    }
}

void GraphicalPose::advanceTime(double increment)
{

    //Encontramos o novo tempo do controle
    double totalTime = this->cumulativeTimeIntervals.back() + this->timeIntervals.back();
    double newTime = (int)(this->time + increment) % (int)totalTime;

    std::cout << totalTime << " total time e " << newTime << " new time\n";

    //Agora temos que descobrir onde ele está no controle
    int pos, nextPos;
    if (newTime > this->cumulativeTimeIntervals.back()) {
        pos = this->cumulativeTimeIntervals.size() - 1;
        nextPos = 0;
    } else {
        std::vector<double>::iterator greaterValue = std::upper_bound(this->cumulativeTimeIntervals.begin(), this->cumulativeTimeIntervals.end(), newTime);
        nextPos = greaterValue - this->cumulativeTimeIntervals.begin();
        pos = nextPos - 1;
    }
    this->current = pos;
    //Agora vamos criar uma pose cujos valores são a interpolação dos valores das poses nos locais correspondentes
    std::cout << pos << " pos " << nextPos << " nextpos \n";
    this->time = newTime;

    //this->poses[pos]->interpolateAndApply(this->poses[nextPos], newTime, this->timeIntervals[pos]);
    Pose* interpolatedPose = this->poses[pos]->interpolateWith(this->poses[nextPos], newTime-this->cumulativeTimeIntervals[pos], this->timeIntervals[pos]);
    if (interpolatedPose != NULL){
        this->setCharacterShape(interpolatedPose);
        delete interpolatedPose;
        interpolatedPose = NULL;
    }
}

void GraphicalPose::setCharacterShape()
{
    if (this->poses.size() > 0){
        Pose* poseActual = this->poses.at(this->current);
        poseActual->setCharacterShape();
    }
}

void GraphicalPose::setCharacterShape(Pose* pose)
{
    if (pose->getCharacter() == this->character){
        pose->setCharacterShape();
    }
}

