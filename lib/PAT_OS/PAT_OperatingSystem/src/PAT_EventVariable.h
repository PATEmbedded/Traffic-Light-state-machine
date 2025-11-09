#ifndef PAT_EVENT_VARIABLE_H
#define PAT_EVENT_VARIABLE_H

#include <Arduino.h>
#include <atomic>
#include <functional>
#include <mutex>

#define log(xxx) \
  { \
    Serial.println("(" + String(__LINE__) + ") " + String(pcTaskGetName(NULL)) + " -> " + String(xxx)); \
    Serial.flush(); \
    for (int i = 0; i < 100000; i++) asm("NOP"); \
  }

template<typename T>
class Event {
private:
  std::atomic<T> value;
  std::function<void()> onChangeCallback;
  std::mutex mutex;

  void triggerCallback() {
    if (onChangeCallback) {
      onChangeCallback();
    }
  }

public:
  Event(T initialValue=0)
    : value(initialValue), onChangeCallback(nullptr) {}

  T operator=(T newVal) {
    std::lock_guard<std::mutex> lock(mutex);
    if (value.load() != newVal) {
      value.store(newVal);
      triggerCallback();
    }
    return value.load();
  }

  void setOnChangeCallback(std::function<void()> callback) {
    onChangeCallback = callback;
  }

  operator T() const {
    return value.load();
  }

  T operator++() {
    std::lock_guard<std::mutex> lock(mutex);
    T newVal = ++value;
    triggerCallback();
    return newVal;
  }

  T operator++(int) {
    std::lock_guard<std::mutex> lock(mutex);
    T oldVal = value.load();
    value.store(value.load() + 1);
    triggerCallback();
    return oldVal;
  }

  T operator--() {
    std::lock_guard<std::mutex> lock(mutex);
    T newVal = --value;
    triggerCallback();
    return newVal;
  }

  T operator--(int) {
    std::lock_guard<std::mutex> lock(mutex);
    T oldVal = value.load();
    value.store(value.load() - 1);
    triggerCallback();
    return oldVal;
  }

  T operator+=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() + rhs);
    triggerCallback();
    return value.load();
  }

  T operator-=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() - rhs);
    triggerCallback();
    return value.load();
  }

  T operator*=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() * rhs);
    triggerCallback();
    return value.load();
  }

  T operator/=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() / rhs);
    triggerCallback();
    return value.load();
  }

  T operator%=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() % rhs);
    triggerCallback();
    return value.load();
  }

  T operator&=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() & rhs);
    triggerCallback();
    return value.load();
  }

  T operator|=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() | rhs);
    triggerCallback();
    return value.load();
  }

  T operator^=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() ^ rhs);
    triggerCallback();
    return value.load();
  }

  T operator<<=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() << rhs);
    triggerCallback();
    return value.load();
  }

  T operator>>=(const T& rhs) {
    std::lock_guard<std::mutex> lock(mutex);
    value.store(value.load() >> rhs);
    triggerCallback();
    return value.load();
  }

  bool operator==(const T& rhs) const {
    return value.load() == rhs;
  }

  bool operator>=(const T& rhs) const {
    return value.load() >= rhs;
  }

  bool operator<=(const T& rhs) const {
    return value.load() <= rhs;
  }

  bool operator!=(const T& rhs) const {
    return value.load() != rhs;
  }
};

#endif // PAT_EVENT_VARIABLE_H