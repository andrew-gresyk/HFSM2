// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Transition payload example:

// Full output:
//
//	---------- ctor: ---------
//
//	enter()
//	Origin::enter()
//
//	--------- update: --------
//
//	update()
//	Origin::update()
//
//	--------- react: ---------
//
//	react()
//	Origin::react()
//	changeTo<Destination>()
//	Destination::guard()
//	Origin::exit()
//	Destination::enter()
//
//	-- external transition: --
//
//	changeTo<Origin>()
//
//	--------- detach: --------
//
//
//	---------- dtor: ---------
//
//	Destination::exit()
//	exit()
//
//	---------- done! ---------

#define HFSM_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	bool boolPayload;
	char charPayload;
};

using Payloads = hfsm2::TransitionPayloads<char, bool>;

// convenience typedef
using M = hfsm2::Machine<Context, Payloads>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Top),
				S(Origin),
				S(Destination)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Top>()		  ==  0, "");

static_assert(FSM::stateId<Top>()		  ==  0, "");
static_assert(FSM::stateId<Origin>()	  ==  1, "");
static_assert(FSM::stateId<Destination>() ==  2, "");

////////////////////////////////////////////////////////////////////////////////

// top-level state in the hierarchy
struct Top
	: FSM::State // necessary boilerplate!
{
	void enter(Control&)							{}
	void update(FullControl&)					{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// initial state
struct Origin
	: FSM::State
{
	void enter(Control&)							{}
	void update(FullControl&)					{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// transition target state
struct Destination
	: FSM::State
{
	void enter(Control&)							{}
	void update(FullControl&)					{}
	void exit(Control&)								{}
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	// shared data storage instance
	Context context;

	// state machine instance - all initial states are activated
	FSM::Instance machine(context);

	context.boolPayload = true;
	machine.setStateData<Origin>(&context.boolPayload);
	HFSM_ASSERT( machine.isStateDataSet<Origin>());

	HSFM_IF_ASSERT(bool* const payBool = machine.getStateData<Origin, bool>());
	HFSM_ASSERT(payBool);
	HFSM_ASSERT(*payBool == true);

	machine.resetStateData<Origin>();
	HFSM_ASSERT(!machine.isStateDataSet<Origin>());

	machine.setStateData<Origin>(&context.charPayload);

	// first update
	machine.update();

	machine.changeTo<Destination>(&context.charPayload);
	machine.update();

	machine.resume  <Destination>(&context.boolPayload);
	machine.update();

	machine.schedule<Destination>(&context.boolPayload);

	machine.react(1);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
