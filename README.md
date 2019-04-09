[![Standard](https://img.shields.io/badge/c%2B%2B-14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/egs56khk70ud35un?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/hfsm2)
[![Build Status](https://travis-ci.org/andrew-gresyk/HFSM2.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM2)
[![codecov](https://codecov.io/gh/andrew-gresyk/HFSM2/branch/master/graph/badge.svg)](https://codecov.io/gh/andrew-gresyk/HFSM2)
[![Coverage Status](https://coveralls.io/repos/github/andrew-gresyk/HFSM2/badge.svg?branch=master)](https://coveralls.io/github/andrew-gresyk/HFSM2?branch=master)

# HFSM2: Hierarchical Finite State Machine Framework with **Planning Support**

Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14, 15, 16
- GCC 5, 6, 7, 8
- Clang 3.6, 3.7, 3.8, 3.9, 4, 5, 6, 7

---

## Basic Usage

1. Include HFSM2 header:

```cpp
#include <hfsm2/machine.hpp>
```

2. Define interface class between the state machine and its host
(also ok to use the host object itself):

```cpp
struct Context {
    bool powerOn;
};
```

3. For goodness sake, please don't do:

```cpp
// using namesplace hfsm2;
```

4. (Optional, recommended) Typedef hfsm2::Machine for convenience:

```cpp
using M = hfsm2::Machine<Context>;
```

5. Declare state machine structure:

Option 1 - with forward declared states:

```cpp
struct Off;
struct On;
struct Red;
struct Yellow;
struct Green;
struct Done;

using FSM = M::PeerRoot<
                // initial state
                Off,
                // sub-machine region with a head state (On) and and 3 sub-states
                M::Composite<On,
                    // initial state of the region, will be activated with the it
                    Red,
                    Yellow,
                    Green
                >,
                Done
            >;
```

Option 2 - with a helper macro (don't forget to `#undef` it afterwards!):

```cpp
#define S(s) struct s

using FSM = M::PeerRoot<
                // initial state
                S(Off),
                // sub-machine region with a head state (On) and and 3 sub-states
                M::Composite<S(On),
                    // initial state of the region, will be activated with the it
                    S(Red),
                    S(Yellow),
                    S(Green)
                >,
                S(Done)
            >;

#undef S
```

6. (Optional) Define events to have external code interact with your state machine directly:

```cpp
struct SomeEvent { /* ... */ };
```

7. Define states and override any or all of the optional state methods:

```cpp
struct Off
    // necessary boilerplate!
    : FSM::State
{
    // called before state activation, use to re-route transitions
    void guard(FullControl& control) {
        // access context data
        if (control.context().powerOn)
            // initiate an immediate transition into 'On' region
            control.changeTo<On>();
    }
};

// region's head state is active for the entire duration of the region being active
struct On
    : FSM::State
{
    // called on state activation
    void enter(Control& control) {
        // access the plan for the region
        auto plan = control.plan();

        // sequence plan steps, executed when the previous state succeeds
        plan.add<Red, Yellow>();
        plan.add<Yellow, Green>();
        plan.add<Green, Yellow>();
        plan.add<Yellow, Red>();
    }

    // called on state deactivation
    void exit(Control& control) { /* ... */ }

    // called on the successful completion of all plan steps
    void planSucceeded(FullControl& control) {
        // we're done here
        control.changeTo<Done>();
    }

    // called if any of the plan steps fails
    void planFailed(FullControl& control) { /* ... */ }
};

struct Red
    : FSM::State
{
    // called on periodic state machine updates
    void update(FullControl& control) {
        // notify successful completion of the plan step
        // plan will advance to the 'Yellow' state
        control.succeed();
    }
};

struct Yellow
    : FSM::State
{
    void update(FullControl& control) {
        // plan will advance to the 'Green' state on the first entry
        // and 'Red' state on the second one
        control.succeed();
    }
};

struct Green
    : FSM::State
{
    // called on external events
    // it's possible to have multiple 'react()' methods, covering multiple events
    template <typename TEvent>
    void react(const TEvent&, FullControl& control) {
        // can also notify successful completion on external event too
        control.succeed();
    }
};

struct Done
    : FSM::State
{};
```

8. Write the client code to use your new state machine:

```cpp
int main() {
```

9. Create context and state machine instances:

```cpp
    Context context;
    FSM fsm(context);
```

10. Kick off periodic updates:

```cpp
    context.powerOn = true;

    // API to check if a state is active
    while (!fsm.isActive<Green>())
        fsm.update();
```

11. (Optional) Handle your special events:

```cpp
    // gentle push for 'Green' state to yield to 'Yellow'
    machine.react(SomeEvent{});
```

12. Keep updating state machine until it's done:

```cpp
    // more updates
    while (!fsm.isActive<Off>())
        fsm.update();

    return 0;
}
```

12. Check [Wiki](../../wiki) for [Tutorial](../../wiki/Tutorial) and docs!

---

## Feature Highlights

- Permissive [MIT License](LICENSE.md)
- Written in widely-supported modern(ish) C++ 14
- Header-only
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
- Type-safe transitions: `FSM.changeTo<TargetState>()`
- 100% NoUML-compliant
- Hierarchical, with composite (sub-machine) and orthogonal regions
- Gamedev-friendly, supports explicit `State::update()`
- Also supports traditional event-based workflow with `State::react()`
- **New: Supports planning! Schedule state-task execution with scripted transitions.**
- **New: Can be used to implement your favorite planner algorithms - BT, HTN, GOAP, etc.**
- Scaleable, supports state re-use via state injections
- Debug-assisted, includes automatic structure and activity visualization API with `#define HFSM_ENABLE_STRUCTURE_REPORT`
- Convenient, minimal boilerplate

---

## Get Updates

- [Blog](https://andrew-gresyk.github.io/)
- [Twitter](https://www.twitter.com/andrew_gresyk)

---

## Special Thanks

- [Phil Nash](https://github.com/philsquared)
- [Romain Cheminade](https://github.com/romaincheminade)
- [Tristan Brindle](https://github.com/tcbrindle)
- [Kevin Greene](https://github.com/kgreenek)
- everybody at [C++::London](https://www.meetup.com/CppLondon/) meetup
- programming community at [Splash Damage](http://www.splashdamage.com/)
