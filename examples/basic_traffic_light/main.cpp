// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Traffic light example:
// Cycle between R-Y-G-Y-B three times, then turn off

// State structure:
//
// Root
//  ├ On
//  │  ├ Red
//  │  ├ YellowDownwards
//  │  ├ YellowUpwards
//  │  └ Green
//  └ Off

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

// optional: enable FSM structure report in debugger
#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount;
};

// convenience typedef
using M = hfsm::Machine<Context>;

#define S(s) struct s

// state machine structure
using FSM = M::PeerRoot<
				// sub-machine ..
				M::Composite<S(On),
					// .. with 4 sub-states
					S(Red),
					S(YellowDownwards),
					S(YellowUpwards),
					S(Green)
				>,
				S(Off)
			>;

#undef S

////////////////////////////////////////////////////////////////////////////////

// top-level region in the hierarchy
struct On
	: FSM::Base // necessary boilerplate!
{
	// called on state entry
	void enter(Control& control) {
		control.context().cycleCount = 0;
		std::cout << "On" << std::endl;
	}
};

//------------------------------------------------------------------------------

// sub-states
struct Red
	: FSM::Base
{
	void enter(Control& control) {
		++control.context().cycleCount;
		std::cout << "  Red" << std::endl;
	}

	// state can initiate transitions to _any_ other state
	void update(TransitionControl& control) {
		// multiple transitions can be initiated, can be useful in a hierarchy
		if (control.context().cycleCount > 3)
			control.changeTo<Off>();
		else
			control.changeTo<YellowDownwards>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowDownwards
	: FSM::Base
{
	void enter(Control&) {
		std::cout << "    Yellow v" << std::endl;
	}

	void update(TransitionControl& control) {
		control.changeTo<Green>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowUpwards
	: FSM::Base
{
	void enter(Control&) {
		std::cout << "    Yellow ^" << std::endl;
	}

	void update(TransitionControl& control) {
		control.changeTo<Red>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Green
	: FSM::Base
{
	void enter(Control&) {
		std::cout << "      Green" << std::endl;
	}

	void update(TransitionControl& control) {
		control.changeTo<YellowUpwards>();
	}
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: FSM::Base
{
	void enter(Control&) {
		std::cout << "Off" << std::endl;
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	// shared data storage instance
	Context context;

	FSM::Instance machine(context);

	while (machine.isActive<Off>() == false)
		machine.update();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
