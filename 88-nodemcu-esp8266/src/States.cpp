#include <Arduino.h> // For Serial

// Define the states of the state machine
enum class State
{
    Searching,
    Engaging,
    Waiting
};

// State machine class that transitions between states and updates based on the current state
class StateMachine
{
    // Public methods and variables for the StateMachine class
public:
    // Constructor initializes the state machine with the Searching state
    StateMachine() : currentState(State::Searching) {}
    // Transitions the state machine to a new state
    void transitionTo(State newState)
    {
        currentState = newState;
    }

    // Updates the state machine based on the current state
    void update()
    {
        switch (currentState)
        {
        case State::Searching:
            Serial.println("Currently in Searching state.");
            // Add logic for Searching state
            break;
        case State::Engaging:
            Serial.println("Currently in Engaging state.");
            // Add logic for Engaging state
            break;
        case State::Waiting:
            Serial.println("Currently in Waiting state.");
            // Add logic for Waiting state
            break;
        }
    }

    // Private methods and variables for the StateMachine class
private:
    State currentState;
};

void stateSetup()
{
    Serial.begin(9600); // Initialize serial communication at 9600 baud
    // Create an instance of the state machine
    StateMachine sm;
    // Transition to the Engaging state
    sm.update(); // Output: Currently in Searching state.

    sm.transitionTo(State::Engaging); // Transition to the Engaging state
    sm.update();                      // Output: Currently in Engaging state.

    sm.transitionTo(State::Waiting);
    sm.update();
}
