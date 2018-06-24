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

#define HFSM_ENABLE_STRUCTURE_REPORT
#include <hfsm/machine_single.hpp>

#include <iostream>

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm::Machine<Context>;

struct PrimaryEvent {};
struct SecondaryEvent { int payload; };

using TransitionEvent = char;

////////////////////////////////////////////////////////////////////////////////

// forward declared for Reactive::transition()
struct Target;

struct Reactive
	: M::Base
{
	// handle a single event type - TransitionEvent
	void react(const TransitionEvent&, Control& control, Context&)  {
		std::cout << "  Reactive: reacting to TransitionEvent\n";

		control.changeTo<Target>();
	}

	// and ignore the other event types
	using M::Base::react;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct NonHandler
		: M::Base
	{
		// events are totally opt-in
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct ConcreteHandler
		: M::Base
	{
		// handle two event types - PrimaryEvent
		void react(const PrimaryEvent&, Control&, Context&)  {
			std::cout << "  ConcreteHandler: reacting to PrimaryEvent\n";
		}

		// and SecondaryEvent
		void react(const SecondaryEvent&, Control&, Context&)  {
			std::cout << "  ConcreteHandler: reacting to SecondaryEvent\n";
		}

		// and ignore the other event types
		using M::Base::react;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct TemplateHandler
		: M::Base
	{
		// handle all possible event types
		template <typename TEvent>
		void react(const TEvent&, Control&, Context&)  {
			std::cout << "  TemplateHandler: reacting to TEvent\n";
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct EnableIfHandler
		: M::Base
	{
		// use std::enable_if to build more complex conditional event handling
		template <typename TEvent>
		typename std::enable_if<std::is_class<TEvent>::value>::type
		react(const TEvent&, Control&, Context&)  {
			std::cout << "  EnableIfHandler: reacting to a <class event>\n";
		}

		// but remember to cover all the remaining cases
		template <typename TEvent>
		typename std::enable_if<!std::is_class<TEvent>::value>::type
		react(const TEvent&, Control&, Context&)  {
			std::cout << "  EnableIfHandler: reacting to a <non-class event>\n";
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

struct Target
	: M::Base
{
	void enter(Context&) {
		std::cout << "    changed to Target\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	Context context;

	M::PeerRoot<
		M::Orthogonal<Reactive,
			Reactive::NonHandler,
			Reactive::ConcreteHandler,
			Reactive::TemplateHandler,
			Reactive::EnableIfHandler
		>,
		Target
	> machine(context);

	std::cout << "sending PrimaryEvent:\n";
	machine.react(PrimaryEvent{});

	std::cout << "\nsending SecondaryEvent:\n";
	machine.react(SecondaryEvent{});

	std::cout << "\nsending TransitionEvent (aka 'char'):\n";
	machine.react(TransitionEvent{});

	std::cout<< std::endl;
	return 0;
};

////////////////////////////////////////////////////////////////////////////////
