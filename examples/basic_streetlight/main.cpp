// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Streetlight FSM example:
// Cycle between R-Y-G-Y-B three times, then turn off

// Output:
//
// On
//   Red
//     Yellow v
//       Green
//     Yellow ^
//   Red
//     Yellow v
//       Green
//     Yellow ^
//   Red
//     Yellow v
//       Green
//     Yellow ^
//   Red
// Off

#define HFSM_MACHINE_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine_single.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount;
};

// convenience typedef
using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

// forward declared state to transition into
struct Off;

// top-level region in the hierarchy
struct On
	: M::Base // necessary boilerplate!
{
	// called on state entry
	void enter(Context& context) {
		context.cycleCount = 0;
		std::cout << "On" << std::endl;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for Red::transition()
	struct YellowDownwards;

	// sub-states
	struct Red
		: M::Base
	{
		void enter(Context& context) {
			++context.cycleCount;
			std::cout << "  Red" << std::endl;
		}

		// state can initiate transitions to _any_ other state
		void transition(Control& control, Context& context) {
			// multiple transitions can be initiated, can be useful in a hierarchy
			if (context.cycleCount > 3)
				control.changeTo<Off>();
			else
				control.changeTo<YellowDownwards>();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: M::Base
{
	void enter(Context&) {
		std::cout << "Off" << std::endl;
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	// shared data storage instance
	Context context;

	// state machine structure
	M::PeerRoot<
		// sub-machine ..
		M::Composite<On,
			// .. with 4 sub-states
			On::Red,
			On::YellowDownwards,
			On::YellowUpwards,
			On::Green
		>,
		Off
	> machine(context);

	while (machine.isActive<Off>() == false)
		machine.update();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
