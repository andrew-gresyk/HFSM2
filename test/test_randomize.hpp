#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_randomize {

//------------------------------------------------------------------------------

using M = hfsm2::MachineT<hfsm2::Config::RandomT<hfsm2::XoShiRo256Plus>>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Random<S(N),
						S(N_000),
						S(N_025),
						S(N_050),
						S(N_075),
						S(N_100)
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
static_assert(FSM::regionId<N>()	==  2, "");
static_assert(FSM::regionId<C>()	==  3, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<I>()		==  1, "");
static_assert(FSM::stateId<O>()		==  2, "");
static_assert(FSM::stateId<N>()		==  3, "");
static_assert(FSM::stateId<N_000>()	==  4, "");
static_assert(FSM::stateId<N_025>()	==  5, "");
static_assert(FSM::stateId<N_050>()	==  6, "");
static_assert(FSM::stateId<N_075>()	==  7, "");
static_assert(FSM::stateId<N_100>()	==  8, "");
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

struct N : FSM::State {};

struct N_000 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.00f; } };
struct N_025 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.25f; } };
struct N_050 : FSM::State { Rank rank(const Control&) {return 0; } Utility utility(const Control&) { return 0.50f; } };
struct N_075 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.75f; } };
struct N_100 : FSM::State { Rank rank(const Control&) {return 1; } Utility utility(const Control&) { return 1.00f; } };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct C : FSM::State {};

struct C_000 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.00f; } };
struct C_025 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.25f; } };
struct C_050 : FSM::State { Rank rank(const Control&) {return 0; } Utility utility(const Control&) { return 0.50f; } };
struct C_075 : FSM::State { Rank rank(const Control&) {return 2; } Utility utility(const Control&) { return 0.75f; } };
struct C_100 : FSM::State { Rank rank(const Control&) {return 1; } Utility utility(const Control&) { return 1.00f; } };

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 15, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  3, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 12, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}

#include <random>
