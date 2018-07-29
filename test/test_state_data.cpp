#include "test_internal_transitions.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Internal transition test", "[historical]") {
	Context _;
	FSM::Instance machine(_);

	WHEN("FSM is activated initially") {

		THEN("initial states in every region are activated from root to leaf") {
		}

		AND_THEN("only initial states in every region are active") {
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
