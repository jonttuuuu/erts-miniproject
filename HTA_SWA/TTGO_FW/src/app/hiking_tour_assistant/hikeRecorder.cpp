#include "hikeRecorder.h"
#include "hardware/motion.h"
#include "FS.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

HikeRecorder::HikeRecorder()
{
    log_w("HikeRecorder Object created");
    currentSession = nullptr;
    lastSessionId = 0;

    // load last session id
    loadLastSessionId();
}

HikeRecorder::~HikeRecorder()
{
    if (currentSession != nullptr)
    {
        delete currentSession;
    }
}

void HikeRecorder::startSession(int stepLength)
{
    if (currentSession != nullptr)
    {
        stopSession();
    }
    log_w("startSession");

    // reset bma stepcounter
    bma_reset_stepcounter();

    // create new session
    int id = lastSessionId + 1;
    currentSession = new HikeSession(id, stepLength);
}

void HikeRecorder::stopSession()
{
    if (currentSession != nullptr)
    {
        log_w("stopSession");
        // save session as json file
        saveCurrentSession();
        delete currentSession;
        currentSession = nullptr;

        // reset bma stepcounter
        bma_reset_stepcounter();
    }
}

void HikeRecorder::updateStepcount()
{
    if (currentSession != nullptr)
    {
        uint32_t stp = bma_get_stepcounter();
        currentSession->updateStepcount(stp);
        log_w("updateStepcount: %d", stp);
    }
}

HikeSession *HikeRecorder::getCurrentSession()
{
    return currentSession;
}

char * HikeRecorder::getLogsAsString() {
    File log = SPIFFS.open(LOG_FILE, FILE_READ);

    const size_t capacity = JSON_ARRAY_SIZE(MAX_SESSIONS) + MAX_SESSIONS * JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc,log);

    log.close();

    // Serialize the JSON document to a char buffer
    size_t bytesWritten = measureJson(doc);

    // Allocate a new C-type string and copy the serialized JSON data
    char* jsonString = new char[bytesWritten + 1];
    serializeJson(doc, jsonString, bytesWritten + 1);

    if (jsonString == NULL) {
        return NULL;
    }
    
    return jsonString;
}

bool HikeRecorder::deleteLogs()
{
    // delete log file 
    bool res = SPIFFS.remove(LOG_FILE);

    if (res)
    {
        log_w("hikeLog.json deleted");
        lastSessionId = 0;
        return res;
    }

    log_w("hikeLog.json not deleted");
    return res;
}

void HikeRecorder::saveCurrentSession()
{
    if (currentSession != nullptr)
    {
        log_w("saveCurrentSession");

        // open file to read and save all the objects
        File file = SPIFFS.open(LOG_FILE, FILE_READ);

        if (!file)
        {
            log_w("Failed to open file for reading");
            return;
        }

        // deserialize existing json to array, add new element and serialize it again
        const size_t capacity = JSON_ARRAY_SIZE(MAX_SESSIONS) + MAX_SESSIONS * JSON_OBJECT_SIZE(3);
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, file);

        // close the file
        file.close();

        // add new element
        JsonObject obj = doc.createNestedObject();
        obj["id"] = currentSession->getId();
        obj["stepcount"] = currentSession->getStepcount();
        obj["distance"] = currentSession->getDistance();

        // open file to write
        file = SPIFFS.open(LOG_FILE, FILE_WRITE);

        if (!file)
        {
            log_w("Failed to open file for writing");
            return;
        }

        // serialize json
        serializeJson(doc, file);

        // save last session id
        lastSessionId = currentSession->getId();

        file.close();
    }
}

void HikeRecorder::loadLastSessionId()
{
    // open
    File file = SPIFFS.open(LOG_FILE, FILE_READ);

    if (!file)
    {
        log_w("Failed to open file for reading");
        return;
    }

    //deserialize json
    const size_t capacity = JSON_ARRAY_SIZE(MAX_SESSIONS) + MAX_SESSIONS * JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        log_w("deserializeJson() failed: %s", error.c_str());
        return;
    }

    JsonArray array = doc.as<JsonArray>();

    if(array.size() == 0) {
        log_w("no sessions found");
        return;
    }

    // get last element
    for (JsonObject obj : array)
    {
        lastSessionId = obj["id"];
    }

    log_w("lastSessionId: %d", lastSessionId);

    file.close();
}
