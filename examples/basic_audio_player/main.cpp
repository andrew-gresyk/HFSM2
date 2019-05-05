// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// An HFSM2 port of https://gist.github.com/martinmoene/797b1923f9c6c1ae355bb2d6870be25e
// by Martin Moene (see https://twitter.com/MartinMoene/status/1118453128834232320)

#define HFSM_ENABLE_LOG_INTERFACE
#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm2/machine.hpp>

#include <assert.h>

#include <iostream>
#include <string>

//------------------------------------------------------------------------------

// events
struct Play   { std::string title; };
struct Pause  {};
struct Resume {};
struct Stop   {};

// shared data stored externally to the fsm
using Context = std::string;
using M = hfsm2::MachineT<Context>;

// fsm structure
#define S(s) struct s
using FSM = M::PeerRoot<
				S(Idle),
				S(Playing),
				S(Paused)
			>;
#undef S

//------------------------------------------------------------------------------

// double-check state ids for Logger::stateName()
static_assert(FSM::stateId<Idle>()	  ==  1, "");
static_assert(FSM::stateId<Playing>() ==  2, "");
static_assert(FSM::stateId<Paused>()  ==  3, "");

// custom logger for recording all transitions
struct Logger
	: hfsm2::LoggerInterface
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

	void recordTransition(const StateID origin,
						  const Transition /*transition*/,
						  const StateID target) override
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
	void react(const Event&, FullControl&) {
		std::cout << "[unsupported transition]\n";
	}
};

// states
struct Idle
	: Base
{
	using Base::react;

	void react(const Play& event, FullControl& control) {
		control.context() = event.title;
		control.changeTo<Playing>();
	}
};

struct Playing
	: Base
{
	using Base::react;

	void react(const Pause&, FullControl& control) {
		control.changeTo<Paused>();
	}

	void react(const Stop&, FullControl& control) {
		control.changeTo<Idle>();
	}
};

struct Paused
	: Base
{
	using Base::react;

	void react(const Resume&, FullControl& control) {
		control.changeTo<Playing>();
	}

	void react(const Stop&, FullControl& control) {
		control.changeTo<Idle>();
	}
};

//------------------------------------------------------------------------------

int main() {
	// construct everything
	Context title;
	Logger logger;
	FSM::Instance machine(title, &logger);

	// do work :)
	machine.react(Play{"any"});
	machine.react(Stop{});

	machine.react(Play{"optional"});
	machine.react(Pause{});
	machine.react(Stop{});

	machine.react(Play{"variant"});
	machine.react(Pause{});
	machine.react(Resume{});
	machine.react(Stop{});

	machine.react(Pause{});
	machine.react(Resume{});
	machine.react(Stop{});

	return 0;
}

//------------------------------------------------------------------------------
