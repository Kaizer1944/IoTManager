#pragma once
#include <Arduino.h>

#include "Global.h"

class LoggingClass;

typedef std::vector<LoggingClass> MyLoggingVector;

class LoggingClass {
   public:

    LoggingClass(unsigned long period, unsigned int maxPoints, String loggingValueKey, String key);
    ~LoggingClass();

    void loop();
    void execute(String keyOrValue);

   private:

    unsigned long currentMillis;
    unsigned long prevMillis;
    unsigned long difference;
    unsigned long _period;
    unsigned int _maxPoints;
    String _loggingValueKey;
    String _key;

    
};

extern MyLoggingVector* myLogging;

extern void logging();
extern void loggingExecute();
extern void choose_log_date_and_send();
extern void sendLogData(String file, String topic);
extern void sendLogData2(String file, String topic);
extern void cleanLogAndData();
