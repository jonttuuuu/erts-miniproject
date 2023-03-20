/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/

#ifndef HIKESESSION_H
#define HIKESESSION_H

#include <vector>

class HikeSession
{
public:
    HikeSession(int id, float stepLength_m);
    ~HikeSession();

    int getId();
    int getStepcount();
    int getDistance();

    void updateStepcount(int stepcount);

private:
    int id;
    int stepcount;
    int distance;
    float stepLength;

    void updateDistance();
};

#endif // HIKESESSION_H
