#include "test_shared.hpp"

//------------------------------------------------------------------------------

struct Context {
	bool boolPayload;
	char charPayload;
};

using Payloads = hfsm::TransitionPayloads<char, bool>;

using M = hfsm::Machine<Context, Payloads>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Top),
				S(Origin),
				S(Destination)
			>;

#undef S

static_assert(FSM::stateId<Top>()			== 0, "");
static_assert(FSM::stateId<Origin>()		== 1, "");
static_assert(FSM::stateId<Destination>()	== 2, "");

//------------------------------------------------------------------------------

struct Top
	: FSM::State
{};

struct Origin
	: FSM::State
{};

struct Destination
	: FSM::State
{};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("State data test", "[machine]") {
	Context _;
	FSM::Instance machine(_);

	static_assert(FSM::Instance::DEEP_WIDTH		  ==  1, "");
	static_assert(FSM::Instance::STATE_COUNT	  ==  3, "");
	static_assert(FSM::Instance::COMPOSITE_COUNT  ==  1, "");
	static_assert(FSM::Instance::ORTHOGONAL_COUNT ==  0, "");
	static_assert(FSM::Instance::FORK_COUNT		  ==  1, "");
	static_assert(FSM::Instance::PRONG_COUNT	  ==  2, "");
	static_assert(FSM::Instance::WIDTH			  ==  2, "");

	WHEN("FSM is activated initially") {

		THEN("state data is not set on any states") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE(!machine.isStateDataSet<Origin>());
			REQUIRE(!machine.isStateDataSet<Destination>());
		}
	}

	WHEN("a bool is set on Origin") {
		_.boolPayload = true;
		machine.setStateData<Origin>(&_.boolPayload);

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE( machine.isStateDataSet<Origin>());
			REQUIRE(!machine.isStateDataSet<Destination>());
		}

		AND_THEN("correctly read") {
			bool* const payBool = machine.getStateData<Origin, bool>();

			REQUIRE(payBool != nullptr);

			if (payBool)
				REQUIRE(*payBool);
		}
	}

	WHEN("a bool is reset on Origin") {
		machine.resetStateData<Origin>();

		THEN("it is correctly set") {
			REQUIRE(!machine.isStateDataSet<Top>());
			REQUIRE(!machine.isStateDataSet<Origin>());
			REQUIRE(!machine.isStateDataSet<Destination>());
		}

		AND_THEN("correctly read") {
			REQUIRE(machine.getStateData<Origin, bool>() == nullptr);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
