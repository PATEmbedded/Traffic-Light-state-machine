#ifndef PAT_OperatingSystem__H
#define PAT_OperatingSystem__H
#include <Arduino.h>
#include <map>
#include <functional>
#include <string>
#include <variant>
#include <typeindex>
#include <stdexcept>
#include <memory>
//--------------------------
#include "PAT_ESP.h"
#include "PAT_QueueManager.h"
#include "PAT_StateMachine.h"
#include "PAT_EventVariable.h"
#include "PAT_VarManager.h"
#include "esp_task_wdt.h"
#include "PAT_JsonStorage.h"
#include "PAT_DataBase.h"
//---------------------------
#define ANY_KEY (String(__FILE__) + "_" + String(__LINE__))
//------------------------------------------------------

// extern const char *db_Configuration_json;
class operatingSystem
{

    queueManager queue;
    std::map<String, bool> runOnceFlags;
    std::map<String, unsigned long> lastExecutionTimes;
    using StateFunction = std::function<void()>;
    // std::map<std::string, std::unique_ptr<VarBase>> var;

public:
    static Class_ESP esp;
    std::map<String, StateMachine> sm;
    // template <typename T>
    Event<void *> *ptr;
    static JsonStorage db;

    //---------------------------------------------------------

    operatingSystem()
    {
    }

    ~operatingSystem()
    {
    }
    void init();
    void initVerbose();
    String randomKey(int tokenLength = 20);
    void executeOnce(const String &key, StateFunction func);
    // void executeAtInterval(const String &key, unsigned long interval, unsigned long timeUpdated, StateFunction func);
    inline void executeAtInterval(const String &key, unsigned long interval, unsigned long timeUpdated, StateFunction func)
    {
        unsigned long currentTime = millis();
        unsigned long lastTime = lastExecutionTimes[key];
        unsigned long timeX = timeUpdated;

        // Adjust the condition to correctly handle the interval logic
        if (currentTime > (lastTime + interval) || timeX > lastTime)
        {
            lastExecutionTimes[key] = currentTime;
            func();
        }
    }
    void executeAtInterval(String key, unsigned long interval, StateFunction func)
    {
        executeAtInterval(key, interval, 0, func);
    }
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // PAT_OperatingSystem
//------------------------------------------------------
extern operatingSystem os;
// Define a macro for executing functions at intervals
