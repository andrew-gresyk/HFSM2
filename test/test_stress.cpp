#include "test_stress.hpp"

using namespace test_stress;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<Apex>(),
		FSM::stateId<S1>(),
		FSM::stateId<O2>(),
		FSM::stateId<O2_C1>(),
		FSM::stateId<O2_C1_S1>(),
		FSM::stateId<O2_C1_S2>(),
		FSM::stateId<O2_C1_O3>(),
		FSM::stateId<O2_C1_O3_C1>(),
		FSM::stateId<O2_C1_O3_C1_S1>(),
		FSM::stateId<O2_C1_O3_C1_S2>(),
		FSM::stateId<O2_C1_O3_C2>(),
		FSM::stateId<O2_C1_O3_C2_S1>(),
		FSM::stateId<O2_C1_O3_C2_S2>(),
		FSM::stateId<O2_C1_O3_C2_S3>(),
		FSM::stateId<O2_C2>(),
		FSM::stateId<O2_C2_S1>(),
		FSM::stateId<O2_C2_C2>(),
		FSM::stateId<O2_C2_C2_S1>(),
		FSM::stateId<O2_C2_C2_S2>(),
		FSM::stateId<O2_C2_C3>(),
		FSM::stateId<O2_C2_C3_S1>(),
		FSM::stateId<O2_C2_C3_S2>(),
		FSM::stateId<O2_C2_C4>(),
		FSM::stateId<O2_C2_C4_S1>(),
		FSM::stateId<O2_C2_C4_S2>(),
		FSM::stateId<O2_C2_S5>(),
		FSM::stateId<S3>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Stress", "[machine]") {
	FSM::Instance machine;

	WHEN("FSM is activated initially") {
		const Types states = {
			FSM::stateId<Apex>(),
			FSM::stateId<S1>(),
		};
		assertActive(machine, all, states);
		assertResumable(machine, all, {});
	}
}

////////////////////////////////////////////////////////////////////////////////
