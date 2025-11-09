#ifndef PAT_JSONSTORAGE_H
#define PAT_JSONSTORAGE_H

// #include <ArduinoJson.h>
#include <SPIFFS.h>
#include <atomic>
#include <freertos/semphr.h>
#include "ArduinoJson.h"

enum fileStatus
{
  LOADED_ERROR = 0,
  LOADED_DEFAULT_FILE = 1,
  LOADED_BACKUP_FILE = 2,
  LOADED_FILE = 3,
};

class JsonStorage
{
private:
  const char *filePath;
  const char *filePath_backup;
  const char *defaultJsonString;
  const int json_size;
  DynamicJsonDocument *jsonDoc;
  SemaphoreHandle_t mutex;
  const char *getBackupFilePath(const char *filePath) const;
  bool loadFromFile(const char *path, int maxRetries = 3);
  bool init();
  fileStatus initDefaultJson();
  bool saveToFile(const char *path, const char *data);
  //--------------------------------------------------------------
public:
  JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size = 1024);
  ~JsonStorage();
  fileStatus open();
  fileStatus load(int maxRetries = 3);
  bool save();
  bool close();
  int max(const char *key);
  DynamicJsonDocument &json();
  JsonVariant operator[](const char *key);
  void print();
};

#endif // PAT_JSONSTORAGE_H

// DynamicJsonDocument *operator->()
// {
//   return jsonDoc;
// }
// const JsonArray &operator[](const char *key) const;
// DynamicJsonDocument &JsonStorage::operator->()
// {
//   return &getJson();
// }

// JsonVariant& JsonStorage::operator[](const char *key)
// {
//     if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
//     {
//         xSemaphoreGive(mutex);
//         return &jsonVariant;
//     }
//     static JsonVariant emptyVariant; // Return an empty JsonVariant if mutex is not available
//     return &emptyVariant;
// }

// DynamicJsonDocument &JsonStorage::operator->()
// {
//   return &getJson();
// }