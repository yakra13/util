
#include <stdio.h>
#include "dictionary.h"

typedef enum StateName { READ, WRITE, _STATE_COUNT };

typedef struct
{
    void* data;
    size_t size;
} Factory;

typedef struct
{
    State* next_state;
} Transition;

#define MAX_STATE_TRANSITIONS 3

typedef struct
{
    StateName name;
    StateAction action;
    Transition* transitions[MAX_STATE_TRANSITIONS];
} State;

typedef struct 
{
    Factory* factory;
    int terminate;
    State* current_state;
    State* state_map[_STATE_COUNT];
} FSM;



typedef void (*StateAction)(FSM*);

void state_func(FSM* fsm)
{
    fsm->factory->data = NULL;

}

int add_state(FSM* fsm, StateName name, StateAction action)
{
    *fsm->state_map[name] = { 
        .name = name,
        .action = action,
        .transitions = {0}
    };
}

int add_state_transition(FSM* fsm, StateName name)
{
    if (fsm->state_map[name] == NULL)
    {
        // State not in machine
        return false;
    }
    for (size_t i = 0; i < MAX_STATE_TRANSITIONS; i++)
    {
        if (fsm->state_map[name]->transitions[i] == NULL)
        {
            *fsm->state_map[name]->transitions[i] = { .next_state = NULL };

            return true;
        }
    }
    // transitions full
    return false;
}

int run_fsm(FSM* machine)
{
    while (!machine->terminate)
    {
        
    }
}

void test()
{
    Factory factory = {0};
    FSM fsm = {0};
    fsm.factory = &factory;
    // fsm.current_state->action(&fsm);

    State s = { 0 };
    s.name = "ready";

    Dictionary* d = DICT_NEW_DICT(char*, StateAction);
    DICT_ADD_ENTRY(d, s.name, state_func);

    StateAction a = (StateAction)DICT_GET_VALUE(d, fsm.current_state);
    a(&fsm);

    // create machine

    // add states

    // add transitions

    // loop machine
        // run current state
        // transition
        // check for end
    
}