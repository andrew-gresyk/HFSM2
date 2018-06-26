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

```cpp
// 1. Include HFSM header:
#include <hfsm/machine_single.hpp>

// 2. Define interface class between the FSM and its owner
//    (also ok to use the owner object itself):
struct Context { /* ... */ };

// 3. (Optional) Typedef hfsm::Machine for convenience:
using M = hfsm::Machine<OwnerClass>;

// 4. Define states:
struct MyState1 : M::Bare {
    // 5. Override some of the following state functions:
    void enter(Context& _);
    void update(Context& _);
    void transition(Control& c, Context& _);
    void leave(Context& _);
};

struct MyState2 : M::Bare { /* .. */ };
struct MySubState1 : M::Bare { /* .. */ };
struct MySubState2 : M::Bare { /* .. */ };

struct MyState3 : M::Bare { /* .. */ };
struct MyOrthogonalState1 : M::Bare { /* .. */ };
struct MyOrthogonalState2 : M::Bare { /* .. */ };

// 6. Declare state machine structure:
using MyFSM = M::PeerRoot<
    MyState1,
    M::Composite<MyState2,
        MySubState1,
        MySubState2,
    >,
    M::Orthogonal<MyState3,
        MyOrthogonalState1,
        MyOrthogonalState2,
    >
>;

int main() {
    // 7. Create context and state machine instances:
    Context context;
    MyFSM fsm(context);

    // 8. Kick off periodic updates:
    bool running = true;
    while (running)
        fsm.update();

    return 0;
}
```

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

## Documentation

See [Wiki](../../wiki) for [Tutorial](../../wiki/Tutorial) and docs.

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