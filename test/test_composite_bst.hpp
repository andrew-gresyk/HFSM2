#define HFSM_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_state_data {

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm2::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(S0),
				S(S1),
				S(S2),
				S(S3),
				S(S4),
				S(S5),
				S(S6),
				S(S7),
				S(S8),
				S(S9)
>;

#undef S

static_assert(FSM::regionId<Apex>()	==  0, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<S0>()	==  1, "");
static_assert(FSM::stateId<S1>()	==  2, "");
static_assert(FSM::stateId<S2>()	==  3, "");
static_assert(FSM::stateId<S3>()	==  4, "");
static_assert(FSM::stateId<S4>()	==  5, "");
static_assert(FSM::stateId<S5>()	==  6, "");
static_assert(FSM::stateId<S6>()	==  7, "");
static_assert(FSM::stateId<S7>()	==  8, "");
static_assert(FSM::stateId<S8>()	==  9, "");
static_assert(FSM::stateId<S9>()	== 10, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};
struct S0	: FSM::State {};
struct S1	: FSM::State {};
struct S2	: FSM::State {};
struct S3	: FSM::State {};
struct S4	: FSM::State {};
struct S5	: FSM::State {};
struct S6	: FSM::State {};
struct S7	: FSM::State {};
struct S8	: FSM::State {};
struct S9	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT == 11, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  1, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  0, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  0, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 10, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
