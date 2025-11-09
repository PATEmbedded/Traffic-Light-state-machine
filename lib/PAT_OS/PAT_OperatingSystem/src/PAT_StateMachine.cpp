#include "PAT_StateMachine.h"

StateMachine::StateMachine() : currentState(""), nextState(""), errorHandler(nullptr),
                               Q_1_State(""), Q_2_State(""), Q_3_State(""),
                               Q_4_State(""), Q_5_State(""), Q_6_State("")
{
}
// Destructor: Delete all created queues
// StateMachine::~StateMachine()
// {
//     for (auto &queuePair : sendQueues)
//     {
//         if (queuePair.second != nullptr)
//         {
//             vQueueDelete(queuePair.second);
//         }
//     }
//     for (auto &queuePair : receiveQueues)
//     {
//         if (queuePair.second != nullptr)
//         {
//             vQueueDelete(queuePair.second);
//         }
//     }
// }
// Create a queue if it doesn't exist for the specified key

void StateMachine::StartState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc;
    if (currentState == "")
    {
        currentState = stateName; // Set the initial state
    }
}
void StateMachine::StartState(const String &stateName)
{

    if (currentState == "")
    {
        currentState = stateName; // Set the initial state
    }
}
void StateMachine::addState(const String &stateName, StateFunction stateFunc)
{
    states[stateName] = stateFunc; // Add or replace state function
}

void StateMachine::onEnterState(StateTransitionHandler handler)
{
    enterHandler = handler;
}

void StateMachine::onExitState(StateTransitionHandler handler)
{
    exitHandler = handler;
}

void StateMachine::setErrorHandler(ErrorHandler handler)
{
    errorHandler = handler;
}

void StateMachine::update()
{
    // Check if a state transition is needed
    if (nextState != "")
    {
        executeTransition();
    }

    // Execute the current state's function
    if (states.count(currentState) > 0)
    {
        states[currentState]();
    }
    else if (errorHandler)
    {
        handleError("State not found: " + currentState);
    }
}

void StateMachine::setNextState(const String &stateName)
{
    nextState = stateName;
}
String StateMachine::getState(stateCode state)
{
    switch (state)
    {
    case current:
        return currentState;
    case next:
        return nextState;
    case Q_1:
        return Q_1_State;
    case Q_2:
        return Q_2_State;
    case Q_3:
        return Q_3_State;
    case Q_4:
        return Q_4_State;
    case Q_5:
        return Q_5_State;
    case Q_6:
        return Q_6_State;
    default:
        return "";
    }
}

void StateMachine::executeTransition()
{
    // Call exit handler for the current state
    if (exitHandler)
    {
        exitHandler(currentState);
    }
    // Perform state transition (shift states down the line)
    Q_6_State = Q_5_State;
    Q_5_State = Q_4_State;
    Q_4_State = Q_3_State;
    Q_3_State = Q_2_State;
    Q_2_State = Q_1_State;
    Q_1_State = currentState;
    // Update current state
    currentState = nextState;
    nextState = "";

    // Call enter handler for the new state
    if (enterHandler)
    {
        enterHandler(currentState);
    }
}

void StateMachine::handleError(const String &errorMessage)
{
    if (errorHandler)
    {
        errorHandler(errorMessage);
    }
}