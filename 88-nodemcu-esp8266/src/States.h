#ifndef STATES_H
#define STATES_H

enum class State
{
    Searching,
    Engaging,
    Waiting
};

class StateMachine
{
public:
    StateMachine();
    void transitionTo(State newState);
    void update();

private:
    State currentState;
};

void stateSetup();

#endif // STATES_H
