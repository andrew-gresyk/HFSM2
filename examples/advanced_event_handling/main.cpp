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
#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm::Machine<Context>;

struct PrimaryEvent {};
struct SecondaryEvent { int payload; };

using TransitionEvent = char;

//------------------------------------------------------------------------------

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

////////////////////////////////////////////////////////////////////////////////

struct Reactive
	: FSM::Base
{
	// handle a single event type - TransitionEvent
	void react(const TransitionEvent&, TransitionControl& control)  {
		std::cout << "  Reactive: reacting to TransitionEvent\n";

		control.changeTo<Target>();
	}

	// and ignore the other event types
	using FSM::Base::react;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct NonHandler
	: FSM::Base
{
	// events are totally opt-in
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct ConcreteHandler
	: FSM::Base
{
	// handle two event types - PrimaryEvent
	void react(const PrimaryEvent&, TransitionControl&)  {
		std::cout << "  ConcreteHandler: reacting to PrimaryEvent\n";
	}

	// and SecondaryEvent
	void react(const SecondaryEvent&, TransitionControl&)  {
		std::cout << "  ConcreteHandler: reacting to SecondaryEvent\n";
	}

	// and ignore the other event types
	using FSM::Base::react;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct TemplateHandler
	: FSM::Base
{
	// handle all possible event types
	template <typename TEvent>
	void react(const TEvent&, TransitionControl&)  {
		std::cout << "  TemplateHandler: reacting to TEvent\n";
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct EnableIfHandler
	: FSM::Base
{
	// use std::enable_if to build more complex conditional event handling
	template <typename TEvent>
	typename std::enable_if<std::is_class<TEvent>::value>::type
	react(const TEvent&, TransitionControl&)  {
		std::cout << "  EnableIfHandler: reacting to a <class event>\n";
	}

	// but remember to cover all the remaining cases
	template <typename TEvent>
	typename std::enable_if<!std::is_class<TEvent>::value>::type
	react(const TEvent&, TransitionControl&)  {
		std::cout << "  EnableIfHandler: reacting to a <non-class event>\n";
	}
};

//------------------------------------------------------------------------------

struct Target
	: FSM::Base
{
	void enter(Control&) {
		std::cout << "    changed to Target\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	Context context;

	FSM::Instance machine(context);

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
};

////////////////////////////////////////////////////////////////////////////////
