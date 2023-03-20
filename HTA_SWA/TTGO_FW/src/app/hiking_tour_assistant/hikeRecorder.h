/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/

#ifndef HIKERECORDER_H
#define HIKERECORDER_H

#include "hikeSession.h"
#include <string>

#define LOG_FILE "/hikeLogs.json"
#define MAX_SESSIONS 100

class HikeRecorder
{

public:
    HikeRecorder();
    ~HikeRecorder();

    void startSession(int stepLength);
    void stopSession();
    void updateStepcount();

    HikeSession * getCurrentSession();
    char * getLogsAsString();
    bool deleteLogs();

private:
    int lastSessionId;
    HikeSession * currentSession;

    void loadLastSessionId();
    void saveCurrentSession();
};

#endif // HIKERECORDER_H