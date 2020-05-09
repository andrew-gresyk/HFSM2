// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Attachable logger example:

// Full output:
//
//	---------- ctor: ---------
//
//	enter()
//	From::enter()
//
//	--------- update: --------
//
//	update()
//	From::update()
//
//	--------- react: ---------
//
//	react()
//	From::react()
//	changeTo<To>()
//	To::entryGuard()
//	From::exit()
//	To::enter()
//
//	-- external transition: --
//
//	changeTo<From>()
//
//	--------- detach: --------
//
//
//	---------- dtor: ---------
//
//	To::exit()
//	exit()
//
//	---------- done! ---------

// enable logger functionality
#define HFSM_ENABLE_LOG_INTERFACE
#include <hfsm2/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// convenience typedef
using M = hfsm2::Machine;

#define S(s) struct s

using FSM = M::Root<S(Top),
				S(From),
				S(To)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Top>()	==  0, "");

static_assert(FSM::stateId<Top>()	==  0, "");
static_assert(FSM::stateId<From>()	==  1, "");
static_assert(FSM::stateId<To>()	==  2, "");

////////////////////////////////////////////////////////////////////////////////

struct Logger
	: hfsm2::LoggerInterface
{
	void recordMethod(Context& /*context*/,
					  const hfsm2::StateID /*origin*/,
					  const Method method) override
	{
		std::cout //<< hfsm2::stateName(origin) << "::"
				  << hfsm2::methodName(method) << "()\n";
	}

	void recordTransition(Context& /*context*/,
						  const hfsm2::StateID /*origin*/,
						  const TransitionType transitionType,
						  const hfsm2::StateID /*target*/) override
	{
		std::cout //<< hfsm2::stateName(origin) << ": "
				  << hfsm2::transitionName(transitionType) << "<"
				  //<< hfsm2::stateName(target) << ">()"
				  "\n";
	}
};

////////////////////////////////////////////////////////////////////////////////

// top-level state in the hierarchy
struct Top
	: FSM::State // necessary boilerplate!
{
	// all state methods:
	void entryGuard(GuardControl&)					{}	// not going to be called in this example
	void enter(Control&)							{}
	void update(FullControl&)						{}
	template <typename TEvent>
	void react(const TEvent&, FullControl&)			{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// initial state
struct From
	: FSM::State
{
	// all state methods:
	void entryGuard(GuardControl&)					{}	// not going to be called in this example
	void enter(Control&)							{}
	void update(FullControl&)						{}
	template <typename TEvent>
	void react(const TEvent&, FullControl& control)	{ control.changeTo<To>(); }
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// transition target state
struct To
	: FSM::State
{
	// all state methods:
	void entryGuard(GuardControl&)					{}
	void enter(Control&)							{}
	void update(FullControl&)						{}
	template <typename TEvent>
	void react(const TEvent&, FullControl&)			{}	// not going to be called in this example
	void exit(Control&)								{}
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	{
		// logger
		Logger logger;

		std::cout << "\n---------- ctor: ---------\n\n";

		// state machine instance - all initial states are activated
		FSM::Instance machine{&logger};

		// output:
		//	enter()
		//	From::enter()

		std::cout << "\n--------- update: --------\n\n";

		// first update
		machine.update();

		// output:
		//	update()
		//	From::update()

		std::cout << "\n--------- react: ---------\n\n";

		machine.react(1);

		// output:
		//	react()
		//	From::react()
		//	changeTo<To>()
		//	To::entryGuard()
		//	From::exit()
		//	To::enter()

		std::cout << "\n-- external transition: --\n\n";

		machine.changeTo<From>();

		// output:
		//	changeTo<From>()

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
	//	To::exit()
	//	exit()

	std::cout << "\n---------- done! ---------\n\n";

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
