#include "test_internal_transitions.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Internal transition test", "[historical]") {
	Context _;
	Logger logger;
	FSM::Instance machine(_, &logger);

	const Types all = {
		machine.stateId<A>(),
		machine.stateId<A_1>(),
		machine.stateId<A_2>(),
		machine.stateId<A_2_1>(),
		machine.stateId<A_2_2>(),
		machine.stateId<B>(),
		machine.stateId<B_1>(),
		machine.stateId<B_1_1>(),
		machine.stateId<B_1_2>(),
		machine.stateId<B_2>(),
		machine.stateId<B_2_1>(),
		machine.stateId<B_2_2>(),
	};

	static_assert(FSM::Instance::DEEP_WIDTH  ==  2, "");
	static_assert(FSM::Instance::STATE_COUNT == 13, "");
	static_assert(FSM::Instance::FORK_COUNT  ==  6, "");
	static_assert(FSM::Instance::PRONG_COUNT == 10, "");

	WHEN("FSM is activated initially") {

		THEN("initial states in every region are activated from root to leaf") {
			Events reference = {
				{ machine.stateId<A>(), Event::ENTER },
				{ machine.stateId<A_1>(), Event::ENTER },
			};
			logger.assertSequence(reference);
		}

		AND_THEN("only initial states in every region are active") {
			const Types states = {
				machine.stateId<A>(),
				machine.stateId<A_1>(),
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
