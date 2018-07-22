#include "test_internal_transitions.hpp"

//------------------------------------------------------------------------------

namespace {

const Types all = {
	typeid(A),
	typeid(A_1),
	typeid(A_2),
	typeid(A_2_1),
	typeid(A_2_2),
	typeid(B),
	typeid(B_1),
	typeid(B_1_1),
	typeid(B_1_2),
	typeid(B_2),
	typeid(B_2_1),
	typeid(B_2_2),
};

}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Internal transition test", "[historical]") {
	Context _;
	Logger logger;
	FSM machine(_, &logger);

	WHEN("FSM is activated initially") {

		THEN("initial states in every region are activated from root to leaf") {
			Events reference = {
				{ typeid(A), Event::ENTER },
				{ typeid(A_1), Event::ENTER },
			};
			logger.assertSequence(reference);
		}

		AND_THEN("only initial states in every region are active") {
			const Types states = {
				typeid(A),
				typeid(A_1),
			};
			assertActive(machine, all, states);
		}

		AND_THEN("no states are resumable") {
			assertResumable(machine, all, {});
		}

		AND_THEN("no states are scheduled") {
			assertScheduled(machine, all, {});
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
