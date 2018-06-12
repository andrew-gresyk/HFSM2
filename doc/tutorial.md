# Tutorial

Code: [Basic streetlight example](examples/basic_streetlight/main.cpp)

## Includes

HFSM and STD's iostream.

```cpp
#include <hfsm/machine_single.hpp>

#include <iostream>
```

`Context` is a data container for storing internal state variables, as well as data to be shared with the external code to FSM.
In this case, it holds a single `unsigned` cycle counter:

```cpp
// data shared between FSM states and outside code
struct Context {
    unsigned cycleCount;
};
```

`hfsm::Machine<TContext>` is a 'templatized namespace', where all the useful HFSM classes live:

- `Bare` (state base)
- `Root` (FSM root)
- `Orthogonal` and `Composite` (state regions)
- etc.

For convenience, typedef it to somehting short, e.g. `M`:

```cpp
// convenience typedef
using M = hfsm::Machine<Context>;
```

State transitions take target state type as a template parameter (`FSM.changeTo<TargetState>()`), therefore target states need to be forward declared before the transition:

```cpp
// forward declared for Red::transition()
struct Off;
```

`On` is the 'head' state of the composite region, declared below:

```cpp
// top-level region in the hierarchy
struct On
    : M::Base // necessary boilerplate!
{
```

All state methods (including `enter()` below) take at least the reference to `Context`:

```cpp
    // called on state entry
    void enter(Context& context) {
        context.cycleCount = 0;
        std::cout << "On" << std::endl;
    }
```

It is often convenient to define sub-states within their regions, so that the code organization reflects FSM structure:

```cpp
    // forward declared for Red::transition()
    struct YellowDownwards;

    // sub-states
    struct Red
        : M::Base
    {
        // called on state entry
        void enter(Context& context) {
            ++context.cycleCount;
            std::cout << "  Red" << std::endl;
        }
```

`transition()` method provides API for a state to request transitions:

- state is free to not request any transitions
- request one
- or multiple transitions

Transitions aren't processed immediately, instead they are recorded, and processed at the end of the state machine's `update()` call.
There are no limitations for target states, it is possible to initiate a transition to any state in the hierarchy.

If the target state is a region, then upon its activation, the initial sub-state will also be activated, recursively until the last leaf state in the chain.
Additionally, activating a region will also cause all of its parent regions to be activated.

At any point in time, an active FSM has:

- active root (implicitly)
- for any active region (including root, since it is a 'special' region):
  - a single active sub-state (for 'composite' regions)
  - all active sub-states (for 'orthogonal' regions)

```cpp
        // state can initiate transitions to _any_ other state
        void transition(Control& control, Context& context) {
            // multiple transitions can be initiated, can be useful in a hierarchy
            if (context.cycleCount > 3)
                control.changeTo<Off>();
            else
                control.changeTo<YellowDownwards>();
        }
    };

    // forward declared for transition()
    struct Green;

    struct YellowDownwards
        : M::Base
    {
        void enter(Context&) {
            std::cout << "    Yellow v" << std::endl;
        }

        void transition(Control& control, Context&) {
            control.changeTo<Green>();
        }
    };

    struct YellowUpwards
        : M::Base
    {
        void enter(Context&) {
            std::cout << "    Yellow ^" << std::endl;
        }

        void transition(Control& control, Context&) {
            control.changeTo<Red>();
        }
    };

    struct Green
        : M::Base
    {
        void enter(Context&) {
            std::cout << "      Green" << std::endl;
        }

        void transition(Control& control, Context&) {
            control.changeTo<YellowUpwards>();
        }
    };
};

// another top-level state
struct Off
    : M::Base
{
    void enter(Context&) {
        std::cout << "Off" << std::endl;
    }
};

int main() {
    // shared data storage instance
    Context context;
```

There are 4 root variants:

- Generic `Root`, a special case of 'composite' region, takes 3+ parameters:
  - 'head' state, to be embedded in the root itself
  - initial sub-state
  - remaining sub-states

- `PeerRoot`, a shortcut for a 'head-less' composite root, takes 2+ parameters:
  - initial sub-state
  - remaining sub-states

- `OrthogonalRoot`, a special case of 'orgthogonal' region, parameters are the same as thos of generic `Root`
- `OrthogonalPeerRoot`, a 'head-less' orgthogonal root

```cpp
    // state machine structure
    M::PeerRoot<
        // sub-machine ..
```

Similar to roots, regions also come in 4 variations:

- 'Generic', taking 3+ parameters:
  - 'head' state, to be embedded in the root itself
  - initial sub-state
  - remaining sub-states
- 'Head-less' (`Peer`*), taking 2+ parameters:
  - initial sub-state
  - remaining sub-states

- 'Composite' regions can have only one active sub-state
- 'Orthogonal' regions have all sub-states active if they are active themselves

Below is the generic composite region with its head state `On`, initial sub-state `On::Red`, and remaining sub-states `On::YellowDownwards`, `On::YellowUpwards` and `On::Green`:

```cpp
        M::Composite<On,
            // .. with 4 sub-states
            On::Red,
            On::YellowDownwards,
            On::YellowUpwards,
            On::Green
        >,
        Off
    > machine(context);
```

`Root::isActive<TState>()` method allows to check the activation status of a state:

```cpp
    while (machine.isActive<Off>() == false)
```

All state machine internal logic executes within `Root::update()` method:

```cpp
        machine.update();
```

It is also possible to initiate a transition from the outside of an FSM using `Root::changeTo<TState>()` method:

```cpp
    machine.changeTo<On>();

    return 0;
}
```