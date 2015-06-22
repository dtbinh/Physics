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
    this->active = false;
    this->advancingTime = false;

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

QString GraphicalPose::getName()
{
    return this->name;
}

bool GraphicalPose::getActive() const
{
    return active;
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

void GraphicalPose::modifyInterval(double poseInterval, int position)
{
    if (position < this->timeIntervals.size()){
        timeIntervals.at(position) = poseInterval;
    }
}

Pose *GraphicalPose::getCurrentPose()
{
    return this->poses.at(this->currentPos);
}

unsigned int GraphicalPose::getCurrentPos()
{
    return this->currentPos;
}

unsigned int GraphicalPose::getNextPos()
{
    return this->nextPos;
}

void GraphicalPose::advanceTimeEnergic(double increment)
{
    if (this->timeIntervals.size() > 0) {
        if (this->time+increment > this->timeIntervals.at(this->currentPos)){
            this->currentPos += 1;

            if (this->currentPos >= this->timeIntervals.size()){
                this->currentPos = 0;
            }

            this->nextPos = this->currentPos + 1;

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

    //std::cout << totalTime << " total time e " << newTime << " new time\n";

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
    this->currentPos = pos;
    this->nextPos = nextPos;
    //Agora vamos criar uma pose cujos valores são a interpolação dos valores das poses nos locais correspondentes
    this->time = newTime;

    if (this->active == true){
        Pose* interpolatedPose = this->poses[pos]->interpolateWith(this->poses[nextPos], newTime-this->cumulativeTimeIntervals[pos], this->timeIntervals[pos]);
        if (interpolatedPose != NULL){
            this->setCharacterShape(interpolatedPose);
            delete interpolatedPose;
            interpolatedPose = NULL;
        }
    }
}

void GraphicalPose::setCharacterShape()
{
    if (this->poses.size() > 0){
        Pose* poseActual = this->poses.at(this->currentPos);
        poseActual->setCharacterShape();
    }
}

void GraphicalPose::setCharacterShape(Pose* pose)
{
    if (pose->getCharacter() == this->character){
        pose->setCharacterShape();
    }
}

void GraphicalPose::setName(QString name)
{
    this->name = name;
}

void GraphicalPose::setActive(bool value)
{
    active = value;
}

bool GraphicalPose::getAdvancingTime() const
{
    return advancingTime;
}

void GraphicalPose::setAdvancingTime(bool value)
{
    advancingTime = value;
}
