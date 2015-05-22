#include "graphicalpose.h"
#include "pose.h"
#include "control.h"
#include <vector>

GraphicalPose::GraphicalPose(std::vector<Pose*> poses, std::vector<double> timeIntervals)
{
    this->character = poses.at(0)->getCharacter();
    this->poses = poses;
    this->timeIntervals = timeIntervals;
}

GraphicalPose::GraphicalPose(Character *character)
{
    this->character = character;
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

void GraphicalPose::pushBackPose(Pose *newPose, double poseInterval)
{
    if (newPose->getCharacter() == this->character) {
        this->poses.push_back(newPose);
        this->timeIntervals.push_back(poseInterval);
    }
}

void GraphicalPose::insertPose(Pose *newPose, double poseInterval, int position)
{
    if (newPose->getCharacter() == this->character) {
        this->poses.insert(this->poses.begin()+position, newPose);
        this->timeIntervals.insert(this->timeIntervals.begin()+position, poseInterval);
    }
}

void GraphicalPose::modifyPose(Pose *modifiedPose, double poseInterval, int position)
{
    if ((modifiedPose->getCharacter() == this->character) && (this->poses.size() > position)){
        this->poses.at(position) = modifiedPose;
        this->timeIntervals.at(position) = poseInterval;
    }
}

Pose *GraphicalPose::getCurrentPose()
{
    return this->poses.at(this->current);
}

void GraphicalPose::advanceTimeEnergic(double increment)
{
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

void GraphicalPose::setCharacterShape()
{
    Pose* poseActual = this->poses.at(this->current);
    poseActual->setCharacterShape();
}

void GraphicalPose::setCharacterShape(Pose* pose)
{
    if (pose->getCharacter() == this->character){
        pose->setCharacterShape();
    }
}

