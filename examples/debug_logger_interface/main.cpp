// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Attachable logger example

// Full output:
//
//	--- ctor: ---
//
//	Top::Enter()
//	Top::From::Enter()
//
//	-- update: --
//
//	Top::Update()
//	Top::Transition()
//	Top::From::Update()
//	Top::From::Transition()
//
//	-- react: ---
//
//	Top::React()
//	Top::From::React()
//	Top::To::Substitute()
//	Top::From::Leave()
//	Top::To::Enter()
//
//	-- detach: --
//
//
//	--- dtor: ---
//
//	Top::To::Leave()
//	Top::Leave()
//
//	--- done! ---

// enable logger functionality
#define HFSM_ENABLE_LOG_INTERFACE
#include <hfsm/machine_single.hpp>

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
	// hfsm::LoggerInterface
	void record(const std::type_index& /*state*/,
				const char* const stateName,
				const Method /*method*/,
				const char* const methodName) override
	{
		std::cout << stateName << "::" << methodName << "\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

// top-level state in the hierarchy
struct Top
	: M::Base
{
	// all state methods:
	void substitute(Control&, Context&)				{}	// not going to be called in this example
	void enter(Context&)							{}
	void update(Context&)							{}
	void transition(Control&, Context&)				{}
	template <typename TEvent>
	void react(const TEvent&, Control&, Context&)	{}
	void leave(Context&)							{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for Red::transition()
	struct To;

	// initial state
	struct From
		: M::Base // necessary boilerplate!
	{
		// all state methods:
		void substitute(Control&, Context&)				{}	// not going to be called in this example
		void enter(Context&)							{}
		void update(Context&)							{}
		void transition(Control&, Context&)				{}
		template <typename TEvent>
		void react(const TEvent&, Control& control, Context&)	{ control.changeTo<To>(); }
		void leave(Context&)							{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// transition target state
	struct To
		: M::Base
	{
		// all state methods:
		void substitute(Control&, Context&)				{}
		void enter(Context&)							{}
		void update(Context&)							{}
		void transition(Control&, Context&)				{}
		template <typename TEvent>
		void react(const TEvent&, Control&, Context&)	{}	// not going to be called in this example
		void leave(Context&)							{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	using FSM = M::Root<Top,
					Top::From,
					Top::To
				>;

	{
		// shared data storage instance
		Context context;

		// logger
		Logger logger;

		std::cout << "--- ctor: ---\n\n";

		// state machine instance - all initial states are activated
		FSM machine(context, &logger);

		// output:
		//	Top::Enter()
		//	Top::From::Enter()

		std::cout << "\n-- update: --\n\n";

		// first update
		machine.update();

		// output:
		//	Top::Update()
		//	Top::Transition()
		//	Top::From::Update()
		//	Top::From::Transition()

		std::cout << "\n-- react: ---\n\n";

		machine.react(1);

		// output:
		//	Top::React()
		//	Top::From::React()
		//	Top::To::Substitute()
		//	Top::From::Leave()
		//	Top::To::Enter()

		std::cout << "\n-- detach: --\n\n";

		// detach logger and update again
		machine.attachLogger(nullptr);
		machine.update();

		// no output, since logger is detached

		std::cout << "\n--- dtor: ---\n\n";

		// re-attach logger for destruction log
		machine.attachLogger(&logger);

		// state machine instance gets destroyed
	}

	// output:
	//	Top::To::Leave()
	//	Top::Leave()

	std::cout << "\n--- done! ---\n\n";

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
