#define HFSM_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_resumable_o {

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm2::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::ResumableRoot<S(Apex),
				M::Resumable<S(A),
					S(A_1),
					S(A_2)
				>,
				M::Orthogonal<S(B),
					S(B_1),
					S(B_2)
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	== 0, "");
static_assert(FSM::regionId<A>()	== 1, "");

static_assert(FSM::stateId<Apex>()	== 0, "");
static_assert(FSM::stateId<A>()		== 1, "");
static_assert(FSM::stateId<A_1>()	== 2, "");
static_assert(FSM::stateId<A_2>()	== 3, "");
static_assert(FSM::stateId<B>()		== 4, "");
static_assert(FSM::stateId<B_1>()	== 5, "");
static_assert(FSM::stateId<B_2>()	== 6, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};
struct A	: FSM::State {};
struct A_1	: FSM::State {};
struct A_2	: FSM::State {};
struct B	: FSM::State {};
struct B_1	: FSM::State {};
struct B_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT ==  7, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  2, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT ==  4, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
