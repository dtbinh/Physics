#include "frame.h"

Frame::Frame()
{
    this->footRight = true;
    this->footLeft = true;
}

Frame::Frame(std::vector<Vec4> pos, std::vector<QuaternionQ> quat)
{
    this->positions = pos;
    this->orientations = quat;
    this->footRight = true;
    this->footLeft = true;
}

void Frame::appendPosition(Vec4 pos)
{
    this->positions.push_back(pos);

}

void Frame::appendOrientation(QuaternionQ quat)
{
    this->orientations.push_back(quat);
}

void Frame::setPosition(int i, Vec4 pos)
{
    this->positions.at(i) = pos;
}

void Frame::setOrientation(int i, QuaternionQ quat)
{
    this->orientations.at(i) = quat;
}

void Frame::setFootRightGround(bool b)
{
    this->footRight = b;
}

void Frame::setFootLeftGround(bool b)
{
    this->footLeft = b;
}

bool Frame::getFootRightGround()
{
    return this->footRight;
}

bool Frame::getFootLeftGround()
{
    return this->footLeft;
}

Vec4 Frame::getPosition(int i)
{
    return this->positions.at(i);
}

QuaternionQ Frame::getOrientation(int i)
{
    //if(this->orientations.size()>=i) return QuaternionQ();
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
