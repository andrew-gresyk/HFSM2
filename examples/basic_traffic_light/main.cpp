﻿// HFSM2 (hierarchical state machine for games and interactive applications)
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
#define HFSM2_ENABLE_STRUCTURE_REPORT
#include <hfsm2/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount = 0;
};

// convenience typedef
using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

#if 0

// states need to be forward declared to be used in FSM struct declaration
struct On;
struct Red;
struct YellowDownwards;
struct YellowUpwards;
struct Green;
struct Off;

using FSM = M::PeerRoot<
				// sub-machine ..
				M::Composite<On,
					// .. with 4 sub-states
					Red,
					YellowDownwards,
					YellowUpwards,
					Green
				>,
				Off
			>;

#else

// alternatively, some macro magic can be invoked to simplify FSM structure declaration
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

#endif

//------------------------------------------------------------------------------

static_assert(FSM::regionId<On>()			  ==  1, "");

static_assert(FSM::stateId<On>()			  ==  1, "");
static_assert(FSM::stateId<Red>()			  ==  2, "");
static_assert(FSM::stateId<YellowDownwards>() ==  3, "");
static_assert(FSM::stateId<YellowUpwards>()	  ==  4, "");
static_assert(FSM::stateId<Green>()			  ==  5, "");
static_assert(FSM::stateId<Off>()			  ==  6, "");

////////////////////////////////////////////////////////////////////////////////

// top-level region in the hierarchy
struct On
	: FSM::State // necessary boilerplate!
{
	// called on state entry
	void enter(Control& control) noexcept {
		control.context().cycleCount = 0;
		std::cout << "On" << std::endl;
	}
};

//------------------------------------------------------------------------------

// sub-states
struct Red
	: FSM::State
{
	void enter(Control& control) noexcept {
		++control.context().cycleCount;
		std::cout << "  Red" << std::endl;
	}

	// state can initiate transitions to _any_ other state
	void update(FullControl& control) noexcept {
		// multiple transitions can be initiated, can be useful in a hierarchy
		if (control.context().cycleCount > 3)
			control.changeTo<Off>();
		else
			control.changeTo<YellowDownwards>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowDownwards
	: FSM::State
{
	void enter(Control&) noexcept {
		std::cout << "    Yellow v" << std::endl;
	}

	void update(FullControl& control) noexcept {
		control.changeTo<Green>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowUpwards
	: FSM::State
{
	void enter(Control&) noexcept {
		std::cout << "    Yellow ^" << std::endl;
	}

	void update(FullControl& control) noexcept {
		control.changeTo<Red>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Green
	: FSM::State
{
	void enter(Control&) noexcept {
		std::cout << "      Green" << std::endl;
	}

	void update(FullControl& control) noexcept {
		control.changeTo<YellowUpwards>();
	}
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: FSM::State
{
	void enter(Control&) noexcept {
		std::cout << "Off" << std::endl;
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() noexcept {
	// shared data storage instance
	Context context;

	FSM::Instance machine{context};

	while (machine.isActive<Off>() == false)
		machine.update();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
