// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Attachable logger example:

// Full output:
//
//	---------- ctor: ---------
//
//	Top::enter()
//	Top::From::enter()
//
//	--------- update: --------
//
//	Top::update()
//	Top::From::update()
//
//	--------- react: ---------
//
//	Top::react()
//	Top::From::react()
//	changeTo<Top::To>()
//	Top::To::guard()
//	Top::From::exit()
//	Top::To::enter()
//
//	-- external transition: --
//
//	changeTo<Top::From>()
//
//	--------- detach: --------
//
//
//	---------- dtor: ---------
//
//	Top::To::exit()
//	Top::exit()
//
//	---------- done! ---------

// enable logger functionality
#define HFSM_ENABLE_LOG_INTERFACE
#include <hfsm/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {};

// convenience typedef
using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm::LoggerInterface
{
	void recordMethod(const std::type_index state,
					  const Method method) override
	{
		std::cout << hfsm::stateName(state) << "::" << hfsm::methodName(method) << "()\n";
	}

	void recordTransition(const Transition transition,
						  const std::type_index state) override
	{
		std::cout << hfsm::transitionName(transition) << "<" << hfsm::stateName(state) << ">()\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

// top-level state in the hierarchy
struct Top
	: M::Base
{
	// all state methods:
	void guard(TransitionControl&)					{}	// not going to be called in this example
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	template <typename TEvent>
	void react(const TEvent&, TransitionControl&)	{}
	void exit(Control&)								{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for Red::transition()
	struct To;

	// initial state
	struct From
		: M::Base // necessary boilerplate!
	{
		// all state methods:
		void guard(TransitionControl&)					{}	// not going to be called in this example
		void enter(Control&)							{}
		void update(TransitionControl&)					{}
		template <typename TEvent>
		void react(const TEvent&, TransitionControl& control)	{ control.changeTo<To>(); }
		void exit(Control&)								{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// transition target state
	struct To
		: M::Base
	{
		// all state methods:
		void guard(TransitionControl&)					{}
		void enter(Control&)							{}
		void update(TransitionControl&)					{}
		template <typename TEvent>
		void react(const TEvent&, TransitionControl&)	{}	// not going to be called in this example
		void exit(Control&)								{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	using FSM = M::Root<Top,
						Top::From,
						Top::To
				>;

	static_assert(FSM::StateList::contains<Top>(), "");
	static_assert(FSM::StateList::contains<Top::From>(), "");
	static_assert(FSM::StateList::contains<Top::To>(), "");

	static_assert(FSM::StateList::index<Top>() == 0, "");
	static_assert(FSM::StateList::index<Top::From>() == 1, "");
	static_assert(FSM::StateList::index<Top::To>() == 2, "");

	{
		// shared data storage instance
		Context context;

		// logger
		Logger logger;

		std::cout << "\n---------- ctor: ---------\n\n";

		// state machine instance - all initial states are activated
		FSM machine(context, &logger);

		// output:
		//	Top::enter()
		//	Top::From::enter()

		std::cout << "\n--------- update: --------\n\n";

		// first update
		machine.update();

		// output:
		//	Top::update()
		//	Top::From::update()

		std::cout << "\n--------- react: ---------\n\n";

		machine.react(1);

		// output:
		//	Top::react()
		//	Top::From::react()
		//	changeTo<Top::To>()
		//	Top::To::guard()
		//	Top::From::exit()
		//	Top::To::enter()

		std::cout << "\n-- external transition: --\n\n";

		machine.changeTo<Top::From>();

		// output:
		//	changeTo<Top::From>()

		std::cout << "\n--------- detach: --------\n\n";

		// detach logger and update again
		machine.attachLogger(nullptr);
		machine.update();

		// no output, since logger is detached

		std::cout << "\n---------- dtor: ---------\n\n";

		// re-attach logger for destruction log
		machine.attachLogger(&logger);

		// state machine instance gets destroyed
	}

	// output:
	//	Top::To::exit()
	//	Top::exit()

	std::cout << "\n---------- done! ---------\n\n";

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
