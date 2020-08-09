// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Event handling example

// Output:
//
//	sending PrimaryEvent:
//	  ConcreteHandler: reacting to PrimaryEvent
//	  TemplateHandler: reacting to TEvent
//	  EnableIfHandler: reacting to a <class event>
//
//	sending SecondaryEvent:
//	  ConcreteHandler: reacting to SecondaryEvent
//	  TemplateHandler: reacting to TEvent
//	  EnableIfHandler: reacting to a <class event>
//
//	sending TransitionEvent:
//	  Reactive: reacting to TransitionEvent (aka 'char')
//	  TemplateHandler: reacting to TEvent
//	  EnableIfHandler: reacting to a <non-class event>
//	    changed to Target

// optional: enable FSM structure report in debugger
#define HFSM2_ENABLE_STRUCTURE_REPORT
#include <hfsm2/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

struct PrimaryEvent {};
struct SecondaryEvent { int payload; };

using TransitionEvent = char;

//------------------------------------------------------------------------------

#if 0

// states need to be forward declared to be used in FSM struct declaration
class Reactive;
class NonHandler;
class ConcreteHandler;
class TemplateHandler;
class EnableIfHandler;
class Target;

using FSM = M::PeerRoot<
				M::Orthogonal<Reactive,
					NonHandler,
					ConcreteHandler,
					TemplateHandler,
					EnableIfHandler
				>,
				Target
			>;

#else

// alternatively, some macro magic can be invoked to simplify FSM structure declaration
#define S(s) struct s

using FSM = M::PeerRoot<
				M::Orthogonal<S(Reactive),
					S(NonHandler),
					S(ConcreteHandler),
					S(TemplateHandler),
					S(EnableIfHandler)
				>,
				S(Target)
			>;

#undef S

#endif

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Reactive>()		  ==  1, "");

static_assert(FSM::stateId<Reactive>()		  ==  1, "");
static_assert(FSM::stateId<NonHandler>()	  ==  2, "");
static_assert(FSM::stateId<ConcreteHandler>() ==  3, "");
static_assert(FSM::stateId<TemplateHandler>() ==  4, "");
static_assert(FSM::stateId<EnableIfHandler>() ==  5, "");
static_assert(FSM::stateId<Target>()		  ==  6, "");

////////////////////////////////////////////////////////////////////////////////

struct Reactive
	: FSM::State
{
	// handle a single event type - TransitionEvent
	void react(const TransitionEvent&, FullControl& control)  {
		std::cout << "  Reactive: reacting to TransitionEvent\n";

		control.changeTo<Target>();
	}

	// and ignore the other event types
	using FSM::State::react;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct NonHandler
	: FSM::State
{
	// events are totally opt-in
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct ConcreteHandler
	: FSM::State
{
	// handle two event types - PrimaryEvent
	void react(const PrimaryEvent&, FullControl&)  {
		std::cout << "  ConcreteHandler: reacting to PrimaryEvent\n";
	}

	// and SecondaryEvent
	void react(const SecondaryEvent&, FullControl&)  {
		std::cout << "  ConcreteHandler: reacting to SecondaryEvent\n";
	}

	// and ignore the other event types
	using FSM::State::react;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct TemplateHandler
	: FSM::State
{
	// handle all possible event types
	template <typename TEvent>
	void react(const TEvent&, FullControl&)  {
		std::cout << "  TemplateHandler: reacting to TEvent\n";
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct EnableIfHandler
	: FSM::State
{
	// use std::enable_if to build more complex conditional event handling
	template <typename TEvent>
	typename std::enable_if<std::is_class<TEvent>::value>::type
	react(const TEvent&, FullControl&)  {
		std::cout << "  EnableIfHandler: reacting to a <class event>\n";
	}

	// but remember to cover all the remaining cases
	template <typename TEvent>
	typename std::enable_if<!std::is_class<TEvent>::value>::type
	react(const TEvent&, FullControl&)  {
		std::cout << "  EnableIfHandler: reacting to a <non-class event>\n";
	}
};

//------------------------------------------------------------------------------

struct Target
	: FSM::State
{
	void enter(Control&) {
		std::cout << "    changed to Target\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	FSM::Instance machine;

	std::cout << "sending PrimaryEvent:\n";
	machine.react(PrimaryEvent{});

	// output:
	//	ConcreteHandler: reacting to PrimaryEvent
	//	TemplateHandler: reacting to TEvent
	//	EnableIfHandler: reacting to a <class event>

	std::cout << "\nsending SecondaryEvent:\n";
	machine.react(SecondaryEvent{});

	// output:
	//	ConcreteHandler: reacting to SecondaryEvent
	//	TemplateHandler: reacting to TEvent
	//	EnableIfHandler: reacting to a <class event>

	std::cout << "\nsending TransitionEvent (aka 'char'):\n";
	machine.react(TransitionEvent{});

	// output:
	//	Reactive: reacting to TransitionEvent (aka 'char')
	//	TemplateHandler: reacting to TEvent
	//	EnableIfHandler: reacting to a <non-class event>
	//	  changed to Target

	std::cout<< std::endl;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
