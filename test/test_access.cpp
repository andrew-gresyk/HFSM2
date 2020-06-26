#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_access {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(A),
					S(A_1),
					M::Composite<S(A_2),
						S(A_2_1),
						S(A_2_2)
					>
				>,
				M::Orthogonal<S(B),
					M::Composite<S(B_1),
						S(B_1_1),
						S(B_1_2)
					>,
					M::Composite<S(B_2),
						S(B_2_1),
						S(B_2_2)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<A  >()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B  >()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A    >() ==  1, "");
static_assert(FSM::stateId<A_1  >() ==  2, "");
static_assert(FSM::stateId<A_2  >() ==  3, "");
static_assert(FSM::stateId<A_2_1>() ==  4, "");
static_assert(FSM::stateId<A_2_2>() ==  5, "");
static_assert(FSM::stateId<B    >() ==  6, "");
static_assert(FSM::stateId<B_1  >() ==  7, "");
static_assert(FSM::stateId<B_1_1>() ==  8, "");
static_assert(FSM::stateId<B_1_2>() ==  9, "");
static_assert(FSM::stateId<B_2  >() == 10, "");
static_assert(FSM::stateId<B_2_1>() == 11, "");
static_assert(FSM::stateId<B_2_2>() == 12, "");

////////////////////////////////////////////////////////////////////////////////

struct A	 : FSM::State { const hfsm2::StateID id = FSM::stateId<A    >(); };
struct A_1	 : FSM::State { const hfsm2::StateID id = FSM::stateId<A_1  >(); };
struct A_2	 : FSM::State { const hfsm2::StateID id = FSM::stateId<A_2  >(); };
struct A_2_1 : FSM::State { const hfsm2::StateID id = FSM::stateId<A_2_1>(); };
struct A_2_2 : FSM::State { const hfsm2::StateID id = FSM::stateId<A_2_2>(); };
struct B	 : FSM::State { const hfsm2::StateID id = FSM::stateId<B    >(); };
struct B_1	 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_1  >(); };
struct B_1_1 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_1_1>(); };
struct B_1_2 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_1_2>(); };
struct B_2	 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_2  >(); };
struct B_2_1 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_2_1>(); };
struct B_2_2 : FSM::State { const hfsm2::StateID id = FSM::stateId<B_2_2>(); };

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

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

}
