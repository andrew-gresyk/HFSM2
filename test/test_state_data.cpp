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

TEST_CASE("State data test", "[machine]") {
	Context _;
	FSM::Instance machine(_);

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
		_.boolPayload = true;
		machine.setStateData<Origin>(&_.boolPayload);

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>()); //-V521
			REQUIRE( machine.isStateDataSet<Origin>()); //-V521
			REQUIRE(!machine.isStateDataSet<Destination>()); //-V521
		}

		AND_THEN("correctly read") {
			bool* const payBool = machine.getStateData<Origin, bool>();

			REQUIRE(payBool != nullptr); //-V521

			if (payBool)
				REQUIRE(*payBool); //-V521
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
			REQUIRE(machine.getStateData<Origin, bool>() == nullptr); //-V521
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
