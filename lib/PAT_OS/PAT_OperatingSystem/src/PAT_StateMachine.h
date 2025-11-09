#ifndef PAT_STATE_MACHINE_H
#define PAT_STATE_MACHINE_H

#include <Arduino.h>
#include <map>
#include <functional>
#include <string>
enum stateCode
{
    current = 0,
    next,
    Q_1,
    Q_2,
    Q_3,
    Q_4,
    Q_5,
    Q_6,
};
class StateMachine
{
private:
    using StateFunction = std::function<void()>;
    using StateTransitionHandler = std::function<void(const String &)>;
    using ErrorHandler = std::function<void(const String &)>;

    String nextState, currentState, Q_1_State, Q_2_State, Q_3_State, Q_4_State, Q_5_State, Q_6_State; // name os state to transition
    std::map<String, StateFunction> states;                                                           // Map of state names to functions
    StateTransitionHandler enterHandler;                                                              // Handler for entering a state
    StateTransitionHandler exitHandler;                                                               // Handler for exiting a state
    ErrorHandler errorHandler;                                                                        // Handler for errors

    // Execute state transitions if needed
    void executeTransition();
    void handleError(const String &errorMessage);
    //-----------------------------------------------------------------------------------
public:
    StateMachine();

    // Initialize the state machine with an initial state
    void StartState(const String &stateName, StateFunction stateFunc);
    void StartState(const String &stateName);

    // Dynamically add a new state
    void addState(const String &stateName, StateFunction stateFunc);

    // Register transition handlers for entering and exiting states
    void onEnterState(StateTransitionHandler handler);
    void onExitState(StateTransitionHandler handler);

    // Register an error handler
    void setErrorHandler(ErrorHandler handler);

    // Update method to execute the current state logic
    void update();

    // Set the next state to transition to
    void setNextState(const String &stateName);

    String getState(stateCode state);
};

#endif // PAT_STATE_MACHINE_H