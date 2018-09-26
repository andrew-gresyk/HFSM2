[![Standard](https://img.shields.io/badge/c%2B%2B-14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build status](https://ci.appveyor.com/api/projects/status/49gona9jtghvvi6g?svg=true)](https://ci.appveyor.com/project/andrew-gresyk/hfsm)
[![Build Status](https://travis-ci.org/andrew-gresyk/HFSM.svg?branch=master)](https://travis-ci.org/andrew-gresyk/HFSM)

# HFSM (Hierarchical Finite State Machine) Framework

Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.

## Compiler Support

- Visual Studio 14.u3, 15.7
- GCC 4.9, 5.4, 6.3, 7.3, 8.0
- Clang 3.6, 3.7, 3.8, 3.9, 4.0, 5.0, 6.0

---

## Basic Usage

1. Include HFSM header:

```cpp
#include <hfsm2/machine.hpp>
```

2. Define interface class between the state machine and its host
(also ok to use the host object itself):

```cpp
struct Context { /* ... */ };
```

3. For goodness sake, please don't:

```cpp
// using namesplace hfsm2;
```

4. (Optional) Typedef hfsm::Machine for convenience:

```cpp
using M = hfsm2::Machine<Context>;
```

5. Declare state machine structure:

Option 1 - with forward declared states:

```cpp
struct On;
struct Red;
struct Yellow;
struct Green;
struct Off;

using FSM = M::PeerRoot<
                // sub-machine region with a head state and..
                M::Composite<On,
                    // .. with 3 sub-states
                    Red,
                    Yellow,
                    Green
				>,
                Off
            >;
```

Option 2 - with a helper macro (don't forget to `#undef` it afterwards!):

```cpp
#define S(s) struct s

using FSM = M::PeerRoot<
                // sub-machine region with a head state and..
                M::Composite<S(On),
                    // .. 3 sub-states
                    S(Red),
                    S(Yellow),
                    S(Green)
				>,
                S(Off)
            >;

#undef S
```

6. (Optional) Define events to have external code interact with your state machine directly:

```cpp
struct SomeEvent) { /* ... */ };
```

7. Define states and override any or all of the optional state methods:

```cpp
struct On
    : FSM::State // necessary boilerplate!
{
    // called before state activation, use to re-route transitions
    void guard(FullControl& control) { /* ... */ }

    // called on state activation
    void enter(Control& control) { /* ... */ }

    // called on periodic state machine updates
    void update(FullControl& control) { /* ... */ }

    // use this to handle events if needed
    template <typename TEvent>
    void react(const TEvent&, FullControl& control) { /* ... */ }

    // called on state deactivation
    void exit(Control& control) { /* ... */ }
};

struct Red    : FSM::State { /* ... */ };
struct Yellow : FSM::State { /* ... */ };
struct Green  : FSM::State { /* ... */ };
struct Off    : FSM::State { /* ... */ };
```

8. Write the client code to use your new state machine:

```cpp
int main() {
```

9. Create context and state machine instances:

```cpp
    Context context;
    MyFSM fsm(context);
```

10. Kick off periodic updates:

```cpp
    bool running = true;
    while (running)
        fsm.update();
```

11. (Optional) Handle your special events:

```cpp
    machine.react(SomeEvent{});

    return 0;
}
```

12. Check [Wiki](../../wiki) for [Tutorial](../../wiki/Tutorial) and docs!

---

## Feature Highlights

- Permissive [MIT License](LICENSE.md)
- Written in widely-supported modern(ish) C++ 14
- 100% NoUML-compliant
- Not hamstrung by restrictive event reaction-based approach, but supports powerful event handling
- Hierarchical, with composite (sub-machine) and orthogonal regions
- Header-only
- Fully static, no dynamic allocations
- Uses inline-friendly compile-time pylymorphism, no virtual methods were harmed
- Type-safe transitions: `FSM.changeTo<TargetState>()`
- Gamedev-friendly, supports explicit `State::update()`
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