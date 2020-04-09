#include "test_access.hpp"

using namespace test_access;

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Access", "[machine]") {
	FSM::Instance machine;

	REQUIRE(machine.access<A	>().id == FSM::stateId<A	>());
	REQUIRE(machine.access<A_1	>().id == FSM::stateId<A_1	>());
	REQUIRE(machine.access<A_2	>().id == FSM::stateId<A_2	>());
	REQUIRE(machine.access<A_2_1>().id == FSM::stateId<A_2_1>());
	REQUIRE(machine.access<A_2_2>().id == FSM::stateId<A_2_2>());
	REQUIRE(machine.access<B	>().id == FSM::stateId<B	>());
	REQUIRE(machine.access<B_1	>().id == FSM::stateId<B_1	>());
	REQUIRE(machine.access<B_1_1>().id == FSM::stateId<B_1_1>());
	REQUIRE(machine.access<B_1_2>().id == FSM::stateId<B_1_2>());
	REQUIRE(machine.access<B_2	>().id == FSM::stateId<B_2	>());
	REQUIRE(machine.access<B_2_1>().id == FSM::stateId<B_2_1>());
	REQUIRE(machine.access<B_2_2>().id == FSM::stateId<B_2_2>());
}

////////////////////////////////////////////////////////////////////////////////
