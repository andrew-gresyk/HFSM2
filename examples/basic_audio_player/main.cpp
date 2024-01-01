// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// An HFSM2 port of https://gist.github.com/martinmoene/797b1923f9c6c1ae355bb2d6870be25e
// by Martin Moene (see https://twitter.com/MartinMoene/status/1118453128834232320)

#include <assert.h>

#include <iostream>
#include <string>

#define HFSM2_ENABLE_LOG_INTERFACE
#define HFSM2_ENABLE_STRUCTURE_REPORT
#include <hfsm2/machine.hpp>

//------------------------------------------------------------------------------

// events
struct Play   { std::string title; };
struct Pause  {};
struct Resume {};
struct Stop   {};

// shared data stored externally to the fsm
using Context = std::string;
using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

#if 0

// states need to be forward declared to be used in FSM struct declaration
struct Idle;
struct Playing;
struct Paused;

using FSM = M::PeerRoot<
				Idle,
				Playing,
				Paused
			>;

#else

// alternatively, some macro magic can be invoked to simplify FSM structure declaration
#define S(s) struct s

using FSM = M::PeerRoot<
				S(Idle),
				S(Playing),
				S(Paused)
			>;

#undef S

#endif

//------------------------------------------------------------------------------

// double-check state ids for Logger::stateName()
static_assert(FSM::stateId<Idle>()	  ==  1, "");
static_assert(FSM::stateId<Playing>() ==  2, "");
static_assert(FSM::stateId<Paused>()  ==  3, "");

// custom logger for recording all transitions
struct Logger
	: M::LoggerInterface
{
	static const char* stateName(const StateID stateId) {
		switch (stateId) {
		case 1:
			return "Idle";
		case 2:
			return "Playing";
		case 3:
			return "Paused";
		default:
			assert(false);
			return "";
		}
	}

	void recordTransition(const Context& /*context*/,
						  const StateID origin,
						  const TransitionType /*transition*/,
						  const StateID target)							override
	{
		std::cout << stateName(origin) << " -> " << stateName(target) << "\n";
	}
};

//------------------------------------------------------------------------------

// state helper for error reporting
struct Base
	: FSM::State
{
	template <typename Event>
	void react(const Event&, EventControl&) {
		std::cout << "[unsupported transition]\n";
	}
};

// states
struct Idle
	: Base
{
	using Base::react;

	void react(const Play& event, EventControl& control) {
		control.context() = event.title;
		control.changeTo<Playing>();
	}
};

struct Playing
	: Base
{
	using Base::react;

	void react(const Pause&, EventControl& control) {
		control.changeTo<Paused>();
	}

	void react(const Stop&, EventControl& control) {
		control.changeTo<Idle>();
	}
};

struct Paused
	: Base
{
	using Base::react;

	void react(const Resume&, EventControl& control) {
		control.changeTo<Playing>();
	}

	void react(const Stop&, EventControl& control) {
		control.changeTo<Idle>();
	}
};

//------------------------------------------------------------------------------

int main() {
	// construct everything
	Context title;
	Logger logger;
	FSM::Instance machine(title, &logger);

	// do the work :)
	machine.react(Play{"any"});			// Idle -> Playing
	machine.react(Stop{});				// Playing -> Idle

	machine.react(Play{"optional"});	// Idle -> Playing
	machine.react(Pause{});				// Playing -> Paused
	machine.react(Stop{});				// Paused -> Idle

	machine.react(Play{"variant"});		// Idle -> Playing
	machine.react(Pause{});				// Playing -> Paused
	machine.react(Resume{});			// Paused -> Playing
	machine.react(Stop{});				// Playing -> Idle

	machine.react(Pause{});				// [unsupported transition]
	machine.react(Resume{});			// [unsupported transition]
	machine.react(Stop{});				// [unsupported transition]

	return 0;
}

//------------------------------------------------------------------------------
