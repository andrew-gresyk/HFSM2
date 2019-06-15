#include "test_state_data.hpp"

using namespace test_state_data;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<Apex>(),
		FSM::stateId<Shotgun>(),
		FSM::stateId<Top>(),
		FSM::stateId<Origin>(),
		FSM::stateId<Destination>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("State Data", "[machine]") {
	FSM::Instance machine;

	WHEN("FSM is activated initially") {
		{
			const Types states = {
				FSM::stateId<Apex>(),
				FSM::stateId<Shotgun>(),
				FSM::stateId<Top>(),
				FSM::stateId<Origin>(),
			};
			assertActive(machine, all, states);
			assertResumable(machine, all, {});
		}

		THEN("state data is not set on any states") {
			REQUIRE(!machine.isStateDataSet<Top>()); //-V521
			REQUIRE(!machine.isStateDataSet<Origin>()); //-V521
			REQUIRE(!machine.isStateDataSet<Destination>()); //-V521
		}
	}

	WHEN("a bool is set on Origin") {
		machine.setStateData<Origin>(true);

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>()); //-V521
			REQUIRE( machine.isStateDataSet<Origin>()); //-V521
			REQUIRE(!machine.isStateDataSet<Destination>()); //-V521
		}

		AND_THEN("correctly read") {
			const bool* const read = machine.getStateData<Origin>();

			REQUIRE(read); //-V521

			if (read)
				REQUIRE(*read); //-V521
		}
	}

	WHEN("a bool is reset on Origin") {
		machine.resetStateData<Origin>();

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>()); //-V521
			REQUIRE(!machine.isStateDataSet<Origin>()); //-V521
			REQUIRE(!machine.isStateDataSet<Destination>()); //-V521
		}

		AND_THEN("correctly read") {
			REQUIRE(machine.getStateData<Origin>() == nullptr); //-V521
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
