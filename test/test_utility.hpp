#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_utility {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Utilitarian<S(U),
						S(U_000),
						S(U_025),
						S(U_050),
						S(U_075),
						S(U_100)
					>,
					M::Composite<S(C),
						S(C_000),
						S(C_025),
						S(C_050),
						S(C_075),
						S(C_100)
					>
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<O>()	==  1, "");
static_assert(FSM::regionId<U>()	==  2, "");
static_assert(FSM::regionId<C>()	==  3, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<I>()		==  1, "");
static_assert(FSM::stateId<O>()		==  2, "");
static_assert(FSM::stateId<U>()		==  3, "");
static_assert(FSM::stateId<U_000>()	==  4, "");
static_assert(FSM::stateId<U_025>()	==  5, "");
static_assert(FSM::stateId<U_050>()	==  6, "");
static_assert(FSM::stateId<U_075>()	==  7, "");
static_assert(FSM::stateId<U_100>()	==  8, "");
static_assert(FSM::stateId<C>()		==  9, "");
static_assert(FSM::stateId<C_000>()	== 10, "");
static_assert(FSM::stateId<C_025>()	== 11, "");
static_assert(FSM::stateId<C_050>()	== 12, "");
static_assert(FSM::stateId<C_075>()	== 13, "");
static_assert(FSM::stateId<C_100>()	== 14, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};

struct I : FSM::State {};

struct O : FSM::State {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct U : FSM::State {};

struct U_000 : FSM::State { float utility(const Control&) { return 0.00f; } };
struct U_025 : FSM::State { float utility(const Control&) { return 0.25f; } };
struct U_050 : FSM::State { float utility(const Control&) { return 0.50f; } };
struct U_075 : FSM::State { float utility(const Control&) { return 0.75f; } };
struct U_100 : FSM::State { float utility(const Control&) { return 1.00f; } };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct C : FSM::State {};

struct C_000 : FSM::State { float utility(const Control&) { return 0.00f; } };
struct C_025 : FSM::State { float utility(const Control&) { return 0.25f; } };
struct C_050 : FSM::State { float utility(const Control&) { return 0.50f; } };
struct C_075 : FSM::State { float utility(const Control&) { return 0.75f; } };
struct C_100 : FSM::State { float utility(const Control&) { return 1.00f; } };

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT == 15, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  3, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 12, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
