#define HFSM_ENABLE_STRUCTURE_REPORT
#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_debug {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Resumable<S(R),
						S(R_1),
						S(R_2)
					>,
					M::Composite<S(C),
						S(C_1),
						S(C_2)
					>,
					M::Utilitarian<S(U),
						S(U_1),
						S(U_2)
					>
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<O>()	==  1, "");
static_assert(FSM::regionId<R>()	==  2, "");
static_assert(FSM::regionId<C>()	==  3, "");
static_assert(FSM::regionId<U>()	==  4, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<I>()		==  1, "");
static_assert(FSM::stateId<O>()		==  2, "");
static_assert(FSM::stateId<R>()		==  3, "");
static_assert(FSM::stateId<R_1>()	==  4, "");
static_assert(FSM::stateId<R_2>()	==  5, "");
static_assert(FSM::stateId<C>()		==  6, "");
static_assert(FSM::stateId<C_1>()	==  7, "");
static_assert(FSM::stateId<C_2>()	==  8, "");
static_assert(FSM::stateId<U>()		==  9, "");
static_assert(FSM::stateId<U_1>()	== 10, "");
static_assert(FSM::stateId<U_2>()	== 11, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};
struct I	: FSM::State {};
struct O	: FSM::State {};
struct R	: FSM::State {};
struct R_1	: FSM::State {};
struct R_2	: FSM::State {};
struct C	: FSM::State {};
struct C_1	: FSM::State {};
struct C_2	: FSM::State {};
struct U	: FSM::State {};
struct U_1	: FSM::State {};
struct U_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT == 12, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  4, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT ==  8, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
