#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_ortho_units {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Orthogonal<S(O1),
					S(O1_01),
					S(O1_02),
					S(O1_03),
					S(O1_04),
					S(O1_05)
				>,
				M::Orthogonal<S(O2),
					S(O2_01),
					S(O2_02),
					S(O2_03),
					S(O2_04),
					S(O2_05)
				>,
				M::Orthogonal<S(O3),
					S(O3_01),
					S(O3_02),
					S(O3_03),
					S(O3_04),
					S(O3_05)
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<O1>()	==  1, "");
static_assert(FSM::regionId<O2>()	==  2, "");
static_assert(FSM::regionId<O3>()	==  3, "");

static_assert(FSM::stateId<Apex>()	==  0, "");
static_assert(FSM::stateId<O1>()	==  1, "");
static_assert(FSM::stateId<O1_01>()	==  2, "");
static_assert(FSM::stateId<O1_02>()	==  3, "");
static_assert(FSM::stateId<O1_03>()	==  4, "");
static_assert(FSM::stateId<O1_04>()	==  5, "");
static_assert(FSM::stateId<O1_05>()	==  6, "");

static_assert(FSM::stateId<O2>()	==  7, "");
static_assert(FSM::stateId<O2_01>()	==  8, "");
static_assert(FSM::stateId<O2_02>()	==  9, "");
static_assert(FSM::stateId<O2_03>()	== 10, "");
static_assert(FSM::stateId<O2_04>()	== 11, "");
static_assert(FSM::stateId<O2_05>()	== 12, "");

static_assert(FSM::stateId<O3>()	== 13, "");
static_assert(FSM::stateId<O3_01>()	== 14, "");
static_assert(FSM::stateId<O3_02>()	== 15, "");
static_assert(FSM::stateId<O3_03>()	== 16, "");
static_assert(FSM::stateId<O3_04>()	== 17, "");
static_assert(FSM::stateId<O3_05>()	== 18, "");

//------------------------------------------------------------------------------

struct Apex	 : FSM::State {};

struct O1	 : FSM::State {};
struct O1_01 : FSM::State {};
struct O1_02 : FSM::State {};
struct O1_03 : FSM::State {};
struct O1_04 : FSM::State {};
struct O1_05 : FSM::State {};

struct O2	 : FSM::State {};
struct O2_01 : FSM::State {};
struct O2_02 : FSM::State {};
struct O2_03 : FSM::State {};
struct O2_04 : FSM::State {};
struct O2_05 : FSM::State {};

struct O3	 : FSM::State {};
struct O3_01 : FSM::State {};
struct O3_02 : FSM::State {};
struct O3_03 : FSM::State {};
struct O3_04 : FSM::State {};
struct O3_05 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 19, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  1, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  ==  3, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  3, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  3, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<O1>(),
		FSM::stateId<O1_01>(),
		FSM::stateId<O1_02>(),
		FSM::stateId<O1_03>(),
		FSM::stateId<O1_04>(),
		FSM::stateId<O1_05>(),

		FSM::stateId<O2>(),
		FSM::stateId<O2_01>(),
		FSM::stateId<O2_02>(),
		FSM::stateId<O2_03>(),
		FSM::stateId<O2_04>(),
		FSM::stateId<O2_05>(),

		FSM::stateId<O3>(),
		FSM::stateId<O3_01>(),
		FSM::stateId<O3_02>(),
		FSM::stateId<O3_03>(),
		FSM::stateId<O3_04>(),
		FSM::stateId<O3_05>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.OrthoUnits", "[machine]") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<O1>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<O1_01>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_02>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_03>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_04>(), Event::ENTRY_GUARD },
			{ FSM::stateId<O1_05>(), Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	 Event::ENTER },
			{ FSM::stateId<O1>(),	 Event::ENTER },
			{ FSM::stateId<O1_01>(), Event::ENTER },
			{ FSM::stateId<O1_02>(), Event::ENTER },
			{ FSM::stateId<O1_03>(), Event::ENTER },
			{ FSM::stateId<O1_04>(), Event::ENTER },
			{ FSM::stateId<O1_05>(), Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<O1>(),
			FSM::stateId<O1_01>(),
			FSM::stateId<O1_02>(),
			FSM::stateId<O1_03>(),
			FSM::stateId<O1_04>(),
			FSM::stateId<O1_05>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}
}

////////////////////////////////////////////////////////////////////////////////

}
