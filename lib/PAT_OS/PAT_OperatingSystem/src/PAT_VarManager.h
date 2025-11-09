#ifndef PAT_VAR_MANAGER__H
#define PAT_VAR_MANAGER__H

#include <map>
#include <string>
#include <stdexcept>
#include "PAT_EventVariable.h" // Ensure this includes the Event class

// class VarBase
// {
// public:
//   virtual ~VarBase() {}
//   virtual void *getValue() = 0;
// };

// template <typename T>
// class Var : public VarBase
// {
//   Event<T> event;

// public:
//   Var(T initialValue) : event(initialValue) {}

//   void *getValue() override
//   {
//     return static_cast<void *>(&event);
//   }

//   Event<T> &getEvent()
//   {
//     return event;
//   }
// };

// class VarManager
// {
//   std::map<std::string, VarBase *> varMap;

// public:
//   ~VarManager()
//   {
//     for (auto &pair : varMap)
//     {
//       delete pair.second; // Clean up
//     }
//   }

//   // Access operator to create or get a variable
//   template <typename T>
//   Event<T> &operator[](const std::string &key)
//   {
//     // Check if the variable already exists
//     if (varMap.find(key) != varMap.end())
//     {
//       // Return the existing event
//       return static_cast<Var<T> *>(varMap[key])->getEvent();
//     }

//     // If it doesn't exist, create a new one with default value
//     return create<T>(key, T());
//   }

//   // Function to create a variable
//   template <typename T>
//   Event<T> &create(const std::string &key, T initialValue)
//   {
//     VarBase *var = new Var<T>(initialValue);
//     varMap[key] = var;
//     return static_cast<Var<T> *>(var)->getEvent();
//   }

//   // Function to get a variable, throws if key doesn't exist
//   template <typename T>
//   Event<T> &get(const std::string &key)
//   {
//     auto it = varMap.find(key);
//     if (it != varMap.end())
//     {
//       return static_cast<Var<T> *>(it->second)->getEvent();
//     }
//     throw std::runtime_error("Variable key not found: " + key);
//   }

//   void remove(const std::string &key)
//   {
//     auto it = varMap.find(key);
//     if (it != varMap.end())
//     {
//       delete it->second; // Clean up
//       varMap.erase(it);
//     }
//   }
// };

#endif // PAT_VAR_MANAGER__H
