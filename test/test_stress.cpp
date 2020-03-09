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

TEST_CASE("FSM.State Data", "[machine]") {
	FSM::Instance machine;

	WHEN("FSM is activated initially") {
		const Types states = {
			FSM::stateId<Apex>(),
			FSM::stateId<Shotgun>(),
			FSM::stateId<Top>(),
			FSM::stateId<Origin>(),
		};
		assertActive(machine, all, states);
		assertResumable(machine, all, {});
	}
}

////////////////////////////////////////////////////////////////////////////////
