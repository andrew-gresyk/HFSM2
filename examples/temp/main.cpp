// HFSM2 (hierarchical state machine for games and interactive applications)
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
#define HFSM2_ENABLE_PLANS
#define HFSM2_ENABLE_LOG_INTERFACE
#include <hfsm2/machine.hpp>

#include <iostream>
#include <Windows.h>


//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount = 0;
	bool fail_this_time = true;
};

//template<typename Machine>
//class HFSM2Logger : public Machine::Logger
//{
//public:
//	void recordMethod(const Machine::Context& context,
//		const hfsm2::StateID origin,
//		const hfsm2::Method method) override
//	{
//		Debug::Event("HFSM2: origin=%d method=%d, origin, method");
//	}
//};

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
	void enter(PlanControl& control) {
		control.context().cycleCount = 0;
		std::cout << "On" << std::endl;

		auto plan = control.plan();

		plan.change<Red, YellowDownwards>();
		plan.change<YellowDownwards, Green>();
		//plan.change<Green, YellowUpwards>();
		//plan.change<YellowUpwards, Red>();
	}

	void exit(PlanControl& /*control*/)               // called on state deactivation
	{
		std::cout << "On::Exit()" << std::endl;
	}

	void planSucceeded(FullControl& control) {        // called on the successful completion
		std::cout << "Plan succeeded" << std::endl;
		control.changeTo<Off>();                      //   of all plan steps
	}

	void planFailed(FullControl& control)
	{
		std::cout << "Plan failed, restarting..." << std::endl;
		control.context().fail_this_time = false;
		control.changeTo<On>();
		//control.plan().restart<On, On>();
	}

	virtual void update(FullControl& /*control*/)
	{
		std::cout << "On Update()" << std::endl;
	}
};

//------------------------------------------------------------------------------

// sub-states
struct Red
	: FSM::State
{
	void enter(Control& control) {
		++control.context().cycleCount;
		std::cout << "Red::Enter()" << std::endl;
	}

	// state can initiate transitions to _any_ other state
	void update(FullControl& control) {
		std::cout << "Red Update()" << std::endl;
		control.succeed();
	}
		// multiple transitions can be initiated, can be useful in a hierarchy
	//	if (control.context().cycleCount > 3)
	//		control.changeTo<Off>();
	//	else
	//		control.changeTo<YellowDownwards>();
	//}

	void exit(PlanControl& control)
	{
		std::cout << "Red::Exit()" << std::endl;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowDownwards
	: FSM::State
{
	void enter(PlanControl& control) {
		std::cout << "Yellow v::Enter()" << std::endl;
	}

	void update(FullControl& control) {
		std::cout << "YellowDownwards Update()" << std::endl;
		//control.changeTo<Green>();
		if (control.context().fail_this_time)
			control.fail();
		else
			control.succeed();
	}

	void exit(PlanControl& control)
	{
		std::cout << "YellowDownwards::Exit()" << std::endl;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct YellowUpwards
	: FSM::State
{
	void enter(Control&) {
		std::cout << "    Yellow ^" << std::endl;
	}

	void update(FullControl& control) {
		std::cout << "YellowUpwards Update()" << std::endl;
		//control.changeTo<Red>();
	}

	void exit(PlanControl& control)
	{
		std::cout << "YellowUpwards::Exit()" << std::endl;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Green
	: FSM::State
{
	void enter(Control&) {
		std::cout << "Green::Enter()" << std::endl;
	}

	void update(FullControl& control) {
		std::cout << "Green Update()" << std::endl;
		//control.changeTo<YellowUpwards>();
		control.succeed();
	}

	void exit(PlanControl& control)
	{
		std::cout << "Green::Exit()" << std::endl;
	}
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: FSM::State
{
	void enter(Control&) {
		std::cout << "Off" << std::endl;
	}
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	// shared data storage instance
	Context context;

	FSM::Instance machine{context};

	while (machine.isActive<Off>() == false)
	{
		std::cout << "update start" << std::endl;
		machine.update();
		std::cout << "update complete" << std::endl << std::endl;
		Sleep(1000);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
