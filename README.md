# HFSM (Hierarchical Finite State Machine) Framework

Header-only heriarchical FSM framework in C++14, completely static (no dynamic allocations), built with variadic templates.


## Compiler Support

- Visual Studio 2015+
- Visual Studio 2017 with Clang codegen v2
- GCC 6.3.1
- Clang 3.9.1


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
	void substitute(Control& c, Context& _);
	void enter(Context& _);
	void update(Context& _);
	void transition(Control& c, Context& _);
	void leave(Context& _);
};
struct MyState2 : M::Bare { /* .. */ };
struct MyState3 : M::Bare { /* .. */ };

// 6. Declare state machine structure:
using MyFSM = M::PeerRoot<
	MyState1,
	MyState2,
	MyState3
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


## Transitions

Initiated from within a state:
```cpp
struct MyState2;

struct MyState1
	: M::Base
{
	void transition(Control& c, Context& _) {
		c.changeTo<MyState2>();
	}
}

struct MyState2
	: M::Base
{
	/* .. */
}
```
<br>

Initiated from the outside of a state machine:
```cpp
int main() {
	/* ... */

	fsm.changeTo<MyState2>(); // processed during the following .update():
	fsm.update();

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

## Special Thanks

- [philsquared](https://github.com/philsquared)
- [romaincheminade](https://github.com/romaincheminade)
- [tcbrindle](https://github.com/tcbrindle)
- [kgreenek](https://github.com/kgreenek)
- programming community at [Splash Damage](http://www.splashdamage.com/)
- everybody at [C++::London](https://www.meetup.com/CppLondon/) meetup