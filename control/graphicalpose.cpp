#include "graphicalpose.h"
#include "pose.h"
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

