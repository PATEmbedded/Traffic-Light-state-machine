#include "PAT_JsonStorage.h"
//_____________________________________________________________________________________________________________________________________
// Load JSON from a given path
bool JsonStorage::loadFromFile(const char *path, int maxRetries)
{
  int attempts = 0;
  while (attempts <= maxRetries)
  {
    File file = SPIFFS.open(path, FILE_READ);
    if (!file)
    {
      Serial.println("Failed to open file, retrying...");
      attempts++;
      delay(50); // Small delay before retrying
    }
    else
    {
      DeserializationError error = deserializeJson(*jsonDoc, file);
      file.close();
      if (!error)
      {
        Serial.printf("loaded %s\n", path);
        return true;
      }
      else
      {
        Serial.printf("error failed to load %s\n", path);
        return false;
      }
    }
  }
  Serial.printf("failed to load %s\n", path);
  return false;
}
//_____________________________________________________________________________________________________________________________________
const char *JsonStorage::getBackupFilePath(const char *filePath) const
{
  size_t length = strlen(filePath);
  const char *dotPos = strrchr(filePath, '.');

  size_t baseNameLength = (dotPos != nullptr) ? (dotPos - filePath) : length;
  size_t extensionLength = (dotPos != nullptr) ? (length - baseNameLength) : 0;
  size_t backupLength = baseNameLength + strlen("_backup") + extensionLength;

  char *result = new char[backupLength + 1]; // +1 for null terminator

  strncpy(result, filePath, baseNameLength);
  strncpy(result + baseNameLength, "_backup", strlen("_backup"));

  if (dotPos != nullptr)
  {
    strncpy(result + baseNameLength + strlen("_backup"), dotPos, extensionLength);
  }

  result[backupLength] = '\0'; // Null-terminate the result

  return result;
}

//_____________________________________________________________________________________________________________________________________
fileStatus JsonStorage::initDefaultJson()
{
  Serial.println("Initializing with default JSON...");
  DeserializationError error = deserializeJson(*jsonDoc, defaultJsonString);
  if (!error)
  {
    return LOADED_DEFAULT_FILE;
  }
  else
  {
    Serial.print("Failed to parse default JSON string: ");
    Serial.println(error.c_str());
  }
  return LOADED_ERROR;
}
//_____________________________________________________________________________________________________________________________________
// Constructor
JsonStorage::JsonStorage(const char *filePath, const char *defaultJsonString, size_t json_size)
    : filePath(filePath), defaultJsonString(defaultJsonString), jsonDoc(nullptr), json_size(json_size), mutex(nullptr)
{
  filePath_backup = getBackupFilePath(filePath);
}
//_____________________________________________________________________________________________________________________________________
// Destructor
JsonStorage::~JsonStorage()
{
  if (jsonDoc != nullptr)
  {
    jsonDoc->clear();  // Free the allocated memory inside the document
    delete jsonDoc;    // Delete the DynamicJsonDocument object
    jsonDoc = nullptr; // Set the pointer to nullptr to avoid dangling pointers
  }
}
//_____________________________________________________________________________________________________________________________________
// Destructor
bool JsonStorage::close()
{
  if (jsonDoc != nullptr)
  {
    jsonDoc->clear();  // Free the allocated memory inside the document
    delete jsonDoc;    // Delete the DynamicJsonDocument object
    jsonDoc = nullptr; // Set the pointer to nullptr to avoid dangling pointers
  }
  return (jsonDoc == nullptr);
}
//_____________________________________________________________________________________________________________________________________
// Initialize SPIFFS and create mutex, load or initialize JSON
bool JsonStorage::init()
{
  if (jsonDoc == nullptr)
  {
    jsonDoc = new DynamicJsonDocument((json_size + 1) * 2);
  }
  if (mutex == nullptr)
  {
    mutex = xSemaphoreCreateMutex(); // Initialize the mutex
  }
  if (!SPIFFS.begin(true))
  {
    Serial.println("Failed to mount SPIFFS");
    delay(500);
    if (!SPIFFS.begin(true))
    {
      Serial.println("Failed to mount SPIFFS again");
      delay(500);
      if (!SPIFFS.begin(true))
      {
        Serial.println("Failed to mount SPIFFS again");
        return false;
      }
    }
  }
  // Serial.println("Mounted SPIFFS");
  return true;
}
//_____________________________________________________________________________________________________________________________________
// Initialize SPIFFS and create mutex, load or initialize JSON
fileStatus JsonStorage::open()
{
  static fileStatus status = LOADED_ERROR;
  if (!this->init())
  {
    status = LOADED_ERROR;
    return status;
  }
  if (status != LOADED_FILE)
  {
    fileStatus status = this->load();
    if (status != LOADED_FILE)
    {
      this->save();
    }
    return status;
  }
  return status;
}
//_____________________________________________________________________________________________________________________________________
fileStatus JsonStorage::load(int maxRetries)
{
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    bool loaded = false;

    // Try to load from the primary file
    if (loadFromFile(filePath, maxRetries))
    {
      loaded = true;
      xSemaphoreGive(mutex); // Release mutex before returning
      return LOADED_FILE;
    }

    // Try to load from the backup file
    if (loadFromFile(filePath_backup, maxRetries))
    {
      loaded = true;
      xSemaphoreGive(mutex); // Release mutex before returning
      return LOADED_BACKUP_FILE;
    }

    // If loading failed, initialize default JSON
    initDefaultJson();
    xSemaphoreGive(mutex); // Release mutex before returning
    return LOADED_DEFAULT_FILE;
  }

  Serial.println("Failed to take mutex in load.");
  return LOADED_ERROR;
}

//_____________________________________________________________________________________________________________________________________
bool JsonStorage::saveToFile(const char *path, const char *data)
{
  if (!this->init())
  {
    return false;
  }

  File file = SPIFFS.open(path, FILE_WRITE); // Open file for writing

  if (!file)
  {
    Serial.printf("Failed to open file for writing path: %s\n", path);
    return false;
  }

  if (file.print(data))
  {
    file.close();
    return true; // Successfully written
  }
  else
  {
    Serial.println("Write failed");
    file.close();
    return false; // Write failed
  }
}
//_____________________________________________________________________________________________________________________________________
bool JsonStorage::save()
{
  bool fileDataSuccess = false;
  bool fileData_backupSuccess = false;
  int attempts = 0;

  String fileData;

  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    size_t bytesWritten = serializeJsonPretty(*jsonDoc, fileData); // Correct return type
    if (bytesWritten < 1)
    {
      Serial.printf("Error(saving) serializing JSON for %s\n", filePath);
      xSemaphoreGive(mutex); // Release mutex before returning
      return false;
    }

    // Attempt to save to the primary file
    while (attempts <= 3 && !fileDataSuccess)
    {
      attempts++;
      fileDataSuccess = saveToFile(this->filePath, fileData.c_str());
    }

    // Attempt to save to the backup file
    attempts = 0;
    while (attempts <= 3 && !fileData_backupSuccess)
    {
      attempts++;
      fileData_backupSuccess = saveToFile(this->filePath_backup, fileData.c_str());
    }

    xSemaphoreGive(mutex); // Release the mutex after saving

    // Return true only if both saves were successful
    return fileDataSuccess && fileData_backupSuccess;
  }

  Serial.println("Failed to take mutex in save.");
  return false;
}

//_____________________________________________________________________________________________________________________________________
DynamicJsonDocument &JsonStorage::json()
{
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
  {
    xSemaphoreGive(mutex);
    return *jsonDoc;
  }
  // Return a static empty document if mutex is not available
  static DynamicJsonDocument emptyDoc(1);
  return emptyDoc;
}
//_____________________________________________________________________________________________________________________________________
JsonVariant JsonStorage::operator[](const char *key)
{
  return (*jsonDoc)[key];
}
//_____________________________________________________________________________________________________________________________________
void JsonStorage::print()
{
  if (xSemaphoreTake(mutex, 5000) == pdTRUE)
  {
    String output;
    size_t bytesWritten = serializeJsonPretty(*jsonDoc, output); // Correct return type
    xSemaphoreGive(mutex);

    if (bytesWritten > 0) // Check if any bytes were written
    {
      Serial.println(output);
      Serial.flush();
    }
    else
    {
      Serial.printf("Error serializing JSON for %s\n", filePath);
      Serial.flush();
    }
  }
  else
  {
    Serial.printf("Mutex error for %s\n", filePath);
    Serial.flush();
  }
}

//_____________________________________________________________________________________________________________________________________
int JsonStorage::max(const char *key)
{
  // Check if the JSON data is an array
  if (!this->json().is<JsonArray>())
  {
    return -1; // Return -1 if the data isn't a valid JSON array
  }

  JsonArray jsonArray = json().as<JsonArray>();
  int maxValue = -1;

  // Loop through the JSON array and find the max value of the given key
  for (JsonObject obj : jsonArray)
  {
    if (obj.containsKey(key))
    {
      int value = obj[key].as<int>();
      if (value > maxValue)
      {
        maxValue = value;
      }
    }
  }
  return maxValue; // Return the maximum value found
}
