# HFSM (Hierarchical Finite State Machine) Framework


## Description
Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.


## Compiler Support
- Visual Studio 2015+
- Visual Studio 2017 with Clang codegen v2
- GCC 6.3.0


## Basic Usage
```cpp
// 1. Include HFSM header:
#include <hfsm/machine.hpp>

// 2. Define interface class between the FSM and its owner
//    (also ok to use the owner object itself):
struct Context { /* ... */ };

// 3. (Optional) Typedef hfsm::Machine for convenience:
using M = hfsm::Machine<OwnerClass>;

// 4. Define states:
struct MyState1 : M::Bare {

   // 5. Override some of the following state functions:
   void substitute(Control& c, Context& _, const Time t);
   void enter(Context& _, const Time t);
   void update(Context& _, const Time t);
   void transition(Control& c, Context& _, const Time t);
   void leave(Context& _, const Time t);
};

// 6. Declare state machine structure:
using MyFSM = M::CompositeRoot<
   M::State<MyState1>,
   M::State<MyState2>,
   M::State<MyState3>
>;

int main() {
    // 7. Create context and state machine instances:
    Context context;
    MyFSM fsm(context);

    // 8. Kick off periodic updates:
    while (true)
        fsm.update(deltaTime);

    return 0;
}
```


## Transitions

From within a state:
```cpp
struct MyState1
    : M::Base
{
    void transition(Control& c, Context& _, const Time t) {
        c.changeTo<MyState2>();
    }
}
```
<br>

Initiated from the outside of a state machine:
```cpp
int main() {
    /* ... */

    fsm.changeTo<MyState2>(); // processed during the following .update():
    fsm.update(deltaTime);

    /* ... */
}
```


## Framework Update Sequence (Pseudo-Code)
```cpp
template <...>
void Machine<...>::Root::update() {
    Control control;

    activeState.update();
    activeState.transition(control);

    while (control.requests.size() > 0) {
        nextState = control.requests[0].state;
        nextState.substitute(control);
    }

    if (nextState != activeState) {
        activeState.leave();
        nextState.enter();
    }
}
```


## State Method Call Sequence During State Transitions (Pseudo-Code)
```cpp
    activeState.update();
    activeState.transition() {
        fsm.changeTo<NextState>();
    }

    nextState.substitute() {} // no state transitions initiated

    activeState.leave();
    activeState = nextState;
    activeState.enter();

    activeState.update();
    activeState.transition();
```
