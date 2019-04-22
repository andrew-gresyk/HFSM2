#define HFSM_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_resumable {

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm2::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::ResumableRoot<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Resumable<S(OR),
						S(OR_1),
						S(OR_2)
					>,
					M::Composite<S(OC),
						S(OC_1),
						S(OC_2)
					>
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	== 0, "");
static_assert(FSM::regionId<O>()	== 1, "");
static_assert(FSM::regionId<OR>()	== 2, "");
static_assert(FSM::regionId<OC>()	== 3, "");

static_assert(FSM::stateId<Apex>()	== 0, "");
static_assert(FSM::stateId<I>()		== 1, "");
static_assert(FSM::stateId<O>()		== 2, "");
static_assert(FSM::stateId<OR>()	== 3, "");
static_assert(FSM::stateId<OR_1>()	== 4, "");
static_assert(FSM::stateId<OR_2>()	== 5, "");
static_assert(FSM::stateId<OC>()	== 6, "");
static_assert(FSM::stateId<OC_1>()	== 7, "");
static_assert(FSM::stateId<OC_2>()	== 8, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};
struct I	: FSM::State {};
struct O	: FSM::State {};
struct OR	: FSM::State {};
struct OR_1	: FSM::State {};
struct OR_2	: FSM::State {};
struct OC	: FSM::State {};
struct OC_1	: FSM::State {};
struct OC_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT == 9, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT == 3, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT == 1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS == 1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 6, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
