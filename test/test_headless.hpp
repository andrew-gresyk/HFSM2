#include "test_shared.hpp"

namespace test_headless {

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm2::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::OrthogonalPeerRoot<
				M::OrthogonalPeers<
					S(Composite_1),
					S(Composite_2)
				>,
				M::OrthogonalPeers<
					S(Orthogonal_1),
					S(Orthogonal_2)
				>
			>;

#undef S

static_assert(FSM::stateId<Composite_1>()	== 2, "");
static_assert(FSM::stateId<Composite_2>()	== 3, "");
static_assert(FSM::stateId<Orthogonal_1>()	== 5, "");
static_assert(FSM::stateId<Orthogonal_2>()	== 6, "");

//------------------------------------------------------------------------------

struct Composite_1	: FSM::State {};
struct Composite_2	: FSM::State {};
struct Orthogonal_1	: FSM::State {};
struct Orthogonal_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH	 == 0, "DEEP_WIDTH");
static_assert(FSM::Instance::STATE_COUNT == 7, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT == 0, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT == 3, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS == 3, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 0, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
