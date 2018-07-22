// HFSM (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk
//
// Transition payload example:

// Full output:
//
//	---------- ctor: ---------
//
//	Top::enter()
//	Top::Origin::enter()
//
//	--------- update: --------
//
//	Top::update()
//	Top::Origin::update()
//
//	--------- react: ---------
//
//	Top::react()
//	Top::Origin::react()
//	changeTo<Top::Destination>()
//	Top::Destination::guard()
//	Top::Origin::exit()
//	Top::Destination::enter()
//
//	-- external transition: --
//
//	changeTo<Top::Origin>()
//
//	--------- detach: --------
//
//
//	---------- dtor: ---------
//
//	Top::Destination::exit()
//	Top::exit()
//
//	---------- done! ---------

#include <hfsm/machine.hpp>

#include <iostream>

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	bool boolPayload;
	char charPayload;
};

using Payloads = hfsm::PayloadList<char, bool>;

// convenience typedef
using M = hfsm::Machine<Context, Payloads>;

////////////////////////////////////////////////////////////////////////////////

// top-level state in the hierarchy
struct Top
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for Red::transition()
	struct Destination;

	// initial state
	struct Origin
		: M::Base // necessary boilerplate!
	{
		void enter(Control&)							{}
		void update(TransitionControl&)					{}
		void exit(Control&)								{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// transition target state
	struct Destination
		: M::Base
	{
		void enter(Control&)							{}
		void update(TransitionControl&)					{}
		void exit(Control&)								{}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

int main() {
	using FSM = M::Root<Top,
					Top::Origin,
					Top::Destination
				>;

	// shared data storage instance
	Context context;

	// state machine instance - all initial states are activated
	FSM machine(context);

	assert(!machine.isStateDataSet<Top::Origin>());

	context.boolPayload = true;
	machine.setStateData<Top::Origin>(&context.boolPayload);
	assert( machine.isStateDataSet<Top::Origin>());

	bool* const payBool = machine.getStateData<Top::Origin, bool>();
	assert(payBool);
	assert(*payBool == true);

	machine.resetStateData<Top::Origin>();
	assert(!machine.isStateDataSet<Top::Origin>());

	machine.setStateData<Top::Origin>(&context.charPayload);

	// first update
	machine.update();

	machine.changeTo<Top::Destination>(&context.charPayload);
	machine.update();

	machine.resume  <Top::Destination>(&context.boolPayload);
	machine.update();

	machine.schedule<Top::Destination>(&context.boolPayload);

	machine.react(1);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
