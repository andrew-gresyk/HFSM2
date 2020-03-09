#include "shared.hpp"

namespace test_stress {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////
// Stress test
// based on https://github.com/andrew-gresyk/HFSM2/issues/13 by https://github.com/DonMathi

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(S1),
				M::Orthogonal<S(O2),
					M::Composite<S(O2_C1),
						S(O2_C1_S1),
						S(O2_C1_S2),
						M::Orthogonal<S(O2_C1_O3),
							M::Composite<S(O2_C1_O3_C1),
								S(O2_C1_O3_C1_S1),
								S(O2_C1_O3_C1_S2)
							>,
							M::Composite<S(O2_C1_O3_C2),
								S(O2_C1_O3_C2_S1),
								S(O2_C1_O3_C2_S2),
								S(O2_C1_O3_C2_S3)
							>
						>
					>,
					M::Composite<S(O2_C2),
						S(O2_C2_S1),
						M::Composite<S(O2_C2_C2),
							S(O2_C2_C2_S1),
							S(O2_C2_C2_S2)
						>,
						M::Composite<S(O2_C2_C3),
							S(O2_C2_C3_S1),
							S(O2_C2_C3_S2)
						>,
						M::Composite<S(O2_C2_C4),
							S(O2_C2_C4_S1),
							S(O2_C2_C4_S2)
						>,
						S(O2_C2_S5)
					>
				>,
				S(S3)
			>;

#undef S

static_assert(FSM::regionId<Apex>()			 ==  0, "");
static_assert(FSM::regionId<O2>()			 ==  1, "");
static_assert(FSM::regionId<O2_C1>()		 ==  2, "");
static_assert(FSM::regionId<O2_C1_O3>()		 ==  3, "");
static_assert(FSM::regionId<O2_C1_O3_C1>()	 ==  4, "");
static_assert(FSM::regionId<O2_C1_O3_C2>()	 ==  5, "");
static_assert(FSM::regionId<O2_C2>()		 ==  6, "");
static_assert(FSM::regionId<O2_C2_C2>()		 ==  7, "");
static_assert(FSM::regionId<O2_C2_C3>()		 ==  8, "");
static_assert(FSM::regionId<O2_C2_C4>()		 ==  9, "");

static_assert(FSM::stateId<Apex>()			 ==  0, "");
static_assert(FSM::stateId<S1>()			 ==  1, "");
static_assert(FSM::stateId<O2>()			 ==  2, "");
static_assert(FSM::stateId<O2_C1>()			 ==  3, "");
static_assert(FSM::stateId<O2_C1_S1>()		 ==  4, "");
static_assert(FSM::stateId<O2_C1_S2>()		 ==  5, "");
static_assert(FSM::stateId<O2_C1_O3>()		 ==  6, "");
static_assert(FSM::stateId<O2_C1_O3_C1>()	 ==  7, "");
static_assert(FSM::stateId<O2_C1_O3_C1_S1>() ==  8, "");
static_assert(FSM::stateId<O2_C1_O3_C1_S2>() ==  9, "");
static_assert(FSM::stateId<O2_C1_O3_C2>()	 == 10, "");
static_assert(FSM::stateId<O2_C1_O3_C2_S1>() == 11, "");
static_assert(FSM::stateId<O2_C1_O3_C2_S2>() == 12, "");
static_assert(FSM::stateId<O2_C1_O3_C2_S3>() == 13, "");
static_assert(FSM::stateId<O2_C2>()			 == 14, "");
static_assert(FSM::stateId<O2_C2_S1>()		 == 15, "");
static_assert(FSM::stateId<O2_C2_C2>()		 == 16, "");
static_assert(FSM::stateId<O2_C2_C2_S1>()	 == 17, "");
static_assert(FSM::stateId<O2_C2_C2_S2>()	 == 18, "");
static_assert(FSM::stateId<O2_C2_C3>()		 == 19, "");
static_assert(FSM::stateId<O2_C2_C3_S1>()	 == 20, "");
static_assert(FSM::stateId<O2_C2_C3_S2>()	 == 21, "");
static_assert(FSM::stateId<O2_C2_C4>()		 == 22, "");
static_assert(FSM::stateId<O2_C2_C4_S1>()	 == 23, "");
static_assert(FSM::stateId<O2_C2_C4_S2>()	 == 24, "");
static_assert(FSM::stateId<O2_C2_S5>()		 == 25, "");
static_assert(FSM::stateId<S3>()			 == 26, "");

//------------------------------------------------------------------------------

struct Apex				: FSM::State {};

struct S1				: FSM::State {};
struct O2				: FSM::State {};

struct O2_C1			: FSM::State {};
struct O2_C1_S1			: FSM::State {};
struct O2_C1_S2			: FSM::State {};

struct O2_C1_O3			: FSM::State {};

struct O2_C1_O3_C1		: FSM::State {};
struct O2_C1_O3_C1_S1	: FSM::State {};
struct O2_C1_O3_C1_S2	: FSM::State {};

struct O2_C1_O3_C2		: FSM::State {};
struct O2_C1_O3_C2_S1	: FSM::State {};
struct O2_C1_O3_C2_S2	: FSM::State {};
struct O2_C1_O3_C2_S3	: FSM::State {};

struct O2_C2			: FSM::State {};
struct O2_C2_S1			: FSM::State {};

struct O2_C2_C2			: FSM::State {};
struct O2_C2_C2_S1		: FSM::State {};
struct O2_C2_C2_S2		: FSM::State {};

struct O2_C2_C3			: FSM::State {};
struct O2_C2_C3_S1		: FSM::State {};
struct O2_C2_C3_S2		: FSM::State {};

struct O2_C2_C4			: FSM::State {};
struct O2_C2_C4_S1		: FSM::State {};
struct O2_C2_C4_S2		: FSM::State {};

struct O2_C2_S5			: FSM::State {};
struct S3				: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 27, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  8, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 22, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  2, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  2, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}
