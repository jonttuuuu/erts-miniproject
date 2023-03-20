#include "hikeSession.h"

HikeSession::HikeSession(int id, float stepLength_m)
                : id(id), stepLength(stepLength_m)
{
    stepcount = 0;
    distance = 0;

    //gpsPositions = std::vector<GPS_pos>();
}

HikeSession::~HikeSession()
{
    //gpsPositions.clear();
}

int HikeSession::getStepcount()
{
    return stepcount;
}

int HikeSession::getDistance()
{
    return distance;
}

int HikeSession::getId()
{
    return id;
}

//std::vector<GPS_pos> HikeSession::getGPSPositions()
//{
//    return gpsPositions;
//}

void HikeSession::updateStepcount(int stepcount)
{
    this->stepcount = stepcount;
    updateDistance();
}

//void HikeSession::addGPSPosition(GPS_pos pos)
//{
//    gpsPositions.push_back(pos);
//}

void HikeSession::updateDistance()
{
    distance = stepcount * stepLength / 100;
}
