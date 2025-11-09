

#include "PAT_OperatingSystem.h"
//===============================================================================================================================================================================================
//                                                                                         / Configuration
//===============================================================================================================================================================================================
operatingSystem os;

Class_ESP operatingSystem::esp;

const char *os_db_json = R"({
  "WiFi": {
    "Connection": {
      "Name": "PAT_IOT",
      "Password": "PAT_IOT123"
    }
  }
})";
// Initialize the static JsonStorage object
JsonStorage operatingSystem::db("/os_db.json", os_db_json, strlen(os_db_json) + 4000);

void operatingSystem::init()
{
  Serial.begin(115200);
  delay(1000);
  // db.open();
  // db.save();
  // db.print();
  // db.close();
}

String operatingSystem::randomKey(int tokenLength)
{
  const char charset[] PROGMEM = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$^&*()-_=+[]{}|;:',.<>?/~`";
  const int charsetLength = sizeof(charset) - 1;
  char token[tokenLength + 1]; // Static to retain memory after function returns
  for (int i = 0; i < tokenLength; ++i)
  {
    token[i] = charset[random(0, charsetLength)];
  }
  token[tokenLength] = '\0'; // Null-terminate the string

  return String(token);
}

void operatingSystem::initVerbose()
{
  init();
  Serial.println("------------------------OS Start------------------------");
  this->esp.resetReason();
  Serial.printf("Free heap: %d kb\n", ESP.getFreeHeap() / 1000);
  // esp_task_wdt_init(0, false); // 0 = Timeout in milliseconds, false = Don't enable watchdog
  esp_task_wdt_init(30, true); // 30 seconds timeout, panic if a task is not responsive
  disableCore0WDT();
  Serial.println("--------------------------------------------------------");
  Serial.flush();
  // disableCore1WDT();
  // queue.init();
}
void operatingSystem::executeOnce(const String &key, StateFunction func)
{
  if (runOnceFlags[key] == false)
  {
    runOnceFlags[key] = true;
    func();
  }
}

// void operatingSystem::executeAtInterval(const String &key, unsigned long interval, unsigned long timeUpdated, StateFunction func)
// {
//     unsigned long currentTime = millis();
//     unsigned long lastTime = lastExecutionTimes[key];
//     unsigned long timeX = timeUpdated;

//     // Adjust the condition to correctly handle the interval logic
//     if (currentTime > (lastTime + interval) || timeX > lastTime)
//     {
//         lastExecutionTimes[key] = currentTime;
//         func();
//     }
// }

// void operatingSystem::executeAtInterval(const String &key , unsigned long interval, StateFunction func)
// {
//     this->executeAtInterval(key, interval, 0, func);
// }
