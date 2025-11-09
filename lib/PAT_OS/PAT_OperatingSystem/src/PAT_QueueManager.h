#ifndef PAT_queueManager__H
#define PAT_queueManager__H
#include <Arduino.h>
#include <map>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

class queueManager
{
    std::map<String, QueueHandle_t> myQueue; // Map to store queues by key
    SemaphoreHandle_t queueMutex;            // Mutex to ensure thread safety
    int defaultQueueLength;                  // Default queue length

public:
    // Constructor to set the default queue length (default = 10)
    queueManager(int queueLength = 10) : defaultQueueLength(queueLength)
    {
        queueMutex = xSemaphoreCreateMutex(); // Initialize mutex
    }

    ~queueManager()
    {
        if (queueMutex != NULL)
        {
            vSemaphoreDelete(queueMutex); // Clean up mutex
        }
    }

    // Create the queue once for a given key and based on item size
    template <typename T>
    void createQueue(const String &key, int queueLength = -1)
    {
        xSemaphoreTake(queueMutex, portMAX_DELAY); // Ensure thread safety
        if (myQueue.find(key) == myQueue.end())
        {                                                                                  // Check if queue already exists
            size_t itemSize = sizeof(T);                                                   // Dynamically set the item size
            int finalQueueLength = (queueLength == -1) ? defaultQueueLength : queueLength; // Use provided queue length or default
            myQueue[key] = xQueueCreate(finalQueueLength, itemSize);
            if (myQueue[key] != NULL)
            {
                Serial.println("Queue created for key: " + key + " with item size: " + String(itemSize) + " and length: " + String(finalQueueLength));
            }
            else
            {
                Serial.println("Failed to create queue for key: " + key);
            }
        }
        xSemaphoreGive(queueMutex);
    }

    // Send data to the queue associated with a key
    template <typename T>
    bool send(const String &key, const T &data, TickType_t timeout = portMAX_DELAY)
    {
        createQueue<T>(key); // Ensure queue is created before sending
        if (xQueueSend(myQueue[key], &data, timeout) == pdPASS)
        {
            Serial.print("Sent to queue: ");
            Serial.println(data);
            return true;
        }
        return false; // Failed to send
    }

    // Receive data from the queue associated with a key
    template <typename T>
    bool receive(const String &key, T &receivedData, TickType_t timeout = portMAX_DELAY)
    {
        createQueue<T>(key); // Ensure queue is created before receiving
        if (xQueueReceive(myQueue[key], &receivedData, timeout) == pdTRUE)
        {
            Serial.print("Received from queue: ");
            Serial.println(receivedData);
            return true;
        }
        return false; // Failed to receive
    }

    // Check if the queue exists for the given key
    bool queueExists(const String &key)
    {
        return myQueue.find(key) != myQueue.end();
    }
};
//------------------------------------------------------------------
#endif // PAT_queueManager__H
