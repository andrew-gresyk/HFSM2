// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Event handling example:

#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine_single.hpp>

#include <iostream>

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm::Machine<Context>;

struct Event {};

////////////////////////////////////////////////////////////////////////////////

// forward declared state to transition into
struct Target;

struct Reactive
	: M::Base
{
	void react(const Event&, Control& control, Context&)  {
		std::cout << "reacting.." << std::endl;

		control.changeTo<Target>();
	}
};

//------------------------------------------------------------------------------

struct Target
	: M::Base
{
	void enter(Context&) {
		std::cout << "changed to Target" << std::endl;
	}
};

//------------------------------------------------------------------------------

int
main() {
	Context context;

	M::PeerRoot<
		Reactive,
		Target
	> machine(context);

	machine.react(Event{});

	return 0;
};

////////////////////////////////////////////////////////////////////////////////
