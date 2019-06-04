#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_utility_regions {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Utilitarian<S(F),
					M::Composite<S(C),
						S(C_1),
						S(C_2),
						S(C_3)
					>,
					M::Resumable<S(R),
						S(R_1),
						S(R_2),
						S(R_3)
					>,
					M::Utilitarian<S(U),
						S(U_1),
						S(U_2),
						S(U_3)
					>,
					M::Orthogonal<S(O),
						M::Composite<S(OC),
							S(OC_1),
							S(OC_2),
							S(OC_3)
						>,
						M::Resumable<S(OR),
							S(OR_1),
							S(OR_2),
							S(OR_3)
						>,
						M::Utilitarian<S(OU),
							S(OU_1),
							S(OU_2),
							S(OU_3)
						>
					>
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<F>()	==  1, "");
static_assert(FSM::regionId<C>()	==  2, "");
static_assert(FSM::regionId<R>()	==  3, "");
static_assert(FSM::regionId<U>()	==  4, "");
static_assert(FSM::regionId<O>()	==  5, "");
static_assert(FSM::regionId<OC>()	==  6, "");
static_assert(FSM::regionId<OR>()	==  7, "");
static_assert(FSM::regionId<OU>()	==  8, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<I>()		==  1, "");
static_assert(FSM::stateId<F>()		==  2, "");
static_assert(FSM::stateId<C>()		==  3, "");
static_assert(FSM::stateId<C_1>()	==  4, "");
static_assert(FSM::stateId<C_2>()	==  5, "");
static_assert(FSM::stateId<C_3>()	==  6, "");
static_assert(FSM::stateId<R>()		==  7, "");
static_assert(FSM::stateId<R_1>()	==  8, "");
static_assert(FSM::stateId<R_2>()	==  9, "");
static_assert(FSM::stateId<R_3>()	== 10, "");
static_assert(FSM::stateId<U>()		== 11, "");
static_assert(FSM::stateId<U_1>()	== 12, "");
static_assert(FSM::stateId<U_2>()	== 13, "");
static_assert(FSM::stateId<U_3>()	== 14, "");
static_assert(FSM::stateId<O>()		== 15, "");
static_assert(FSM::stateId<OC>()	== 16, "");
static_assert(FSM::stateId<OC_1>()	== 17, "");
static_assert(FSM::stateId<OC_2>()	== 18, "");
static_assert(FSM::stateId<OC_3>()	== 19, "");
static_assert(FSM::stateId<OR>()	== 20, "");
static_assert(FSM::stateId<OR_1>()	== 21, "");
static_assert(FSM::stateId<OR_2>()	== 22, "");
static_assert(FSM::stateId<OR_3>()	== 23, "");
static_assert(FSM::stateId<OU>()	== 24, "");
static_assert(FSM::stateId<OU_1>()	== 25, "");
static_assert(FSM::stateId<OU_2>()	== 26, "");
static_assert(FSM::stateId<OU_3>()	== 27, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};

struct I    : FSM::State {};
struct F    : FSM::State {};

struct C    : FSM::State {};
struct C_1  : FSM::State {};
struct C_2  : FSM::State {};
struct C_3  : FSM::State {};

struct R    : FSM::State {};
struct R_1  : FSM::State {};
struct R_2  : FSM::State {};
struct R_3  : FSM::State {};

struct U    : FSM::State {};
struct U_1  : FSM::State {};
struct U_2  : FSM::State {};
struct U_3  : FSM::State {};

struct O    : FSM::State {};

struct OC   : FSM::State {};
struct OC_1 : FSM::State {};
struct OC_2 : FSM::State {};
struct OC_3 : FSM::State {};

struct OR   : FSM::State {};
struct OR_1 : FSM::State {};
struct OR_2 : FSM::State {};
struct OR_3 : FSM::State {};

struct OU   : FSM::State {};
struct OU_1 : FSM::State {};
struct OU_2 : FSM::State {};
struct OU_3 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT == 28, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  8, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 24, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
