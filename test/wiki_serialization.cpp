// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_SERIALIZATION

#include <hfsm2/machine.hpp>

#include <doctest/doctest.h>

using M = hfsm2::Machine;

using FSM = M::PeerRoot<
				struct State1,
				struct State2
			>;

struct State1 : FSM::State { /* .. */ };
struct State2 : FSM::State { /* .. */ };

//------------------------------------------------------------------------------

TEST_CASE("Docs.Serialization") {
	// Buffer for serialization
	//  Members:
	//   bitSize - Number of payload bits used
	//   payload - Serialized data
	FSM::Instance::SerialBuffer buffer;

	{
		FSM::Instance fsm;              // Create a new FSM instance
		fsm.changeTo<State2>();         // Request a transition to 'State2'
		fsm.update();                   // Process transitions
		REQUIRE(fsm.isActive<State2>()); // Check if transition completed

		fsm.save(buffer);               // Serialize FSM configuration into 'buffer'
	}

	{
		FSM::Instance fsm;              // Create a fresh FSM instance
		REQUIRE(fsm.isActive<State1>()); // Initial 'State1' is activated by default

		fsm.load(buffer);               // De-serialize FSM from 'buffer'
		REQUIRE(fsm.isActive<State2>()); // Check its configuration is restored
	}
}

////////////////////////////////////////////////////////////////////////////////
