#include "test_shared.hpp"

//------------------------------------------------------------------------------

struct Context {
	bool boolPayload;
	char charPayload;
};

using Payloads = hfsm::TransitionPayloads<char, bool>;

using M = hfsm::Machine<Context, Payloads>;

////////////////////////////////////////////////////////////////////////////////

struct Top
	: FSM::Base
{
	struct Origin
		: FSM::Base
	{};

	struct Destination
		: FSM::Base
	{};
};

//------------------------------------------------------------------------------

using FSM = M::Root<Top,
				Top::Origin,
				Top::Destination
			>;

static_assert(FSM::stateId<Top>()				== 0, "");
static_assert(FSM::stateId<Top::Origin>()		== 1, "");
static_assert(FSM::stateId<Top::Destination>()	== 2, "");

//------------------------------------------------------------------------------

TEST_CASE("Internal transition test", "[historical]") {
	Context _;
	FSM::Instance machine(_);

	static_assert(FSM::Instance::DEEP_WIDTH  == 1, "");
	static_assert(FSM::Instance::STATE_COUNT == 3, "");
	static_assert(FSM::Instance::FORK_COUNT  == 1, "");
	static_assert(FSM::Instance::PRONG_COUNT == 2, "");

	WHEN("FSM is activated initially") {

		THEN("state data is not set on any states") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE(!machine.isStateDataSet<Top::Origin>());
			REQUIRE(!machine.isStateDataSet<Top::Destination>());
		}
	}

	WHEN("a bool is set on Origin") {
		_.boolPayload = true;
		machine.setStateData<Top::Origin>(&_.boolPayload);

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE( machine.isStateDataSet<Top::Origin>());
			REQUIRE(!machine.isStateDataSet<Top::Destination>());
		}

		AND_THEN("correctly read") {
			bool* const payBool = machine.getStateData<Top::Origin, bool>();

			REQUIRE(payBool != nullptr);

			if (payBool)
				REQUIRE(*payBool);
		}
	}

	WHEN("a bool is reset on Origin") {
		machine.resetStateData<Top::Origin>();

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE(!machine.isStateDataSet<Top::Origin>());
			REQUIRE(!machine.isStateDataSet<Top::Destination>());
		}

		AND_THEN("correctly read") {
			REQUIRE(machine.getStateData<Top::Origin, bool>() == nullptr);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
