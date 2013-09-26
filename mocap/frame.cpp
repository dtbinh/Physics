#include "frame.h"

Frame::Frame()
{
}

Frame::Frame(std::vector<Vec4> pos, std::vector<Quaternion> quat)
{
    this->positions = pos;
    this->orientations = quat;
}

void Frame::appendPosition(Vec4 pos)
{
    this->positions.push_back(pos);
}

void Frame::appendOrientation(Quaternion quat)
{
    this->orientations.push_back(quat);
}

void Frame::setPosition(int i, Vec4 pos)
{
    this->positions.at(i) = pos;
}

void Frame::setOrientation(int i, Quaternion quat)
{
    this->orientations.at(i) = quat;
}

Vec4 Frame::getPosition(int i)
{
    return this->positions.at(i);
}

Quaternion Frame::getOrientation(int i)
{
    return this->orientations.at(i);
}

Frame::~Frame()
{
    positions.clear();
    orientations.clear();
}

int Frame::getNumFrames()
{
    return positions.size();
}
