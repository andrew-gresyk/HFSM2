#define HFSM_VERBOSE_DEBUG_LOG
#include "test_shared.hpp"

namespace test_self_transitions {

////////////////////////////////////////////////////////////////////////////////

using Context = float;
using M = hfsm2::Machine<Context>;

using Action = bool;

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

static_assert(FSM::regionId<A>()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B>()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A>()		==  1, "");
static_assert(FSM::stateId<A_1>()	==  2, "");
static_assert(FSM::stateId<A_2>()	==  3, "");
static_assert(FSM::stateId<A_2_1>()	==  4, "");
static_assert(FSM::stateId<A_2_2>()	==  5, "");
static_assert(FSM::stateId<B>()		==  6, "");
static_assert(FSM::stateId<B_1>()	==  7, "");
static_assert(FSM::stateId<B_1_1>()	==  8, "");
static_assert(FSM::stateId<B_1_2>()	==  9, "");
static_assert(FSM::stateId<B_2>()	== 10, "");
static_assert(FSM::stateId<B_2_1>()	== 11, "");
static_assert(FSM::stateId<B_2_2>()	== 12, "");

////////////////////////////////////////////////////////////////////////////////

struct A	 : FSM::State {};
struct A_1	 : FSM::State {};
struct A_2	 : FSM::State {};
struct A_2_1 : FSM::State {};
struct A_2_2 : FSM::State {};
struct B	 : FSM::State {};
struct B_1	 : FSM::State {};
struct B_1_1 : FSM::State {};
struct B_1_2 : FSM::State {};
struct B_2	 : FSM::State {};
struct B_2_1 : FSM::State {};
struct B_2_2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH	 ==  2, "DEEP_WIDTH");
static_assert(FSM::Instance::STATE_COUNT == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  5, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 10, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
