// HFSM (hierarchical state machine for games and interactive applications)
// streetlight FSM example
//
// transition cycle:
// 1:
// - On (implicit initial top-level state)
// - On::Red (implicit initial sub-state)
// 2:
// - On::Red -> On::Yellow (explicit transition by On::Red)
// 3:
// - On::Yellow -> On::Green (explicit conditional transition by On::Yellow)
// 4:
// - On::Green -> On::Yellow (explicit transition by On::Green)
// 5:
// - On::Yellow -> On::Red (explicit conditional transition by On::Yellow)
// - On::Red -> Off (substitution by On::Red)

#define HFSM_MACHINE_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine.hpp>

enum class Direction { Up, Down };

// data shared between FSM states and outside code
struct Context {

	Direction direction;
	const char* state;    
};
// convenience typedef
using M = hfsm::Machine<Context>;

// forward declared state to transition into
struct Off;

// top-level in the hierarchy
struct On
	: M::Base // necessary boilerplate!
{
	// called on state entry
	void enter(Context& context) { context.state = "On"; }

	// forward declared for transition()
	struct Yellow;

	// sub-states
	struct Red : M::Base {
		// don't enter the state if reached full cycle
		// instead - substitute ourselves for another state
		// substituted transition will not activate the intermediate state
		// similar to 'state guards' in other FSMs, but more powerful!
		void substitute(Control& control, Context& context) {
			if (context.state[0] == 'Y')
				control.changeTo<Off>();
		}

		void enter(Context& context) {
			context.direction = Direction::Down;
			context.state = "Red";
		}

		// state can initiate transitions to _any_ other state
		void transition(Control& control, Context&) {
			// multiple transitions can be initiated, can be useful in a hierarchy
			control.changeTo<Yellow>();
		}
	};

	// forward declared for transition()
	struct Green;

	struct Yellow : M::Base {
		void enter(Context& context) { context.state = "Yellow"; }

		void transition(Control& control, Context& context) {
			if (context.direction == Direction::Up)
				control.changeTo<Red>();
			else
				control.changeTo<Green>();
		}
	};

	struct Green : M::Base {
		void enter(Context& context) {
			context.direction = Direction::Up;
			context.state = "Green";
		}

		void transition(Control& control, Context&) {
			control.changeTo<Yellow>();
		}
	};
};

// another top-level state
struct Off
	: M::Base
{
	void enter(Context& context) { context.state = "Off"; }
};

// root state, optional
struct StreetLight : M::Base {};

void
run() {
	// shared data storage instance
	Context context;

	// state machine structure
	M::Root<StreetLight,
		// sub-machine with 3 sub-states
		M::Composite<On,
			On::Red,
			On::Yellow,
			On::Green
		>,
		Off
	> machine(context);

	while (machine.isActive<Off>() == false)
		machine.update();
};
