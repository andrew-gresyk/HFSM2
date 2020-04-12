#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_composite_bst {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

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

static_assert(FSM::Instance::STATE_COUNT   == 11, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  1, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  0, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  0, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<S0>(),
		FSM::stateId<S1>(),
		FSM::stateId<S2>(),
		FSM::stateId<S3>(),
		FSM::stateId<S4>(),
		FSM::stateId<S5>(),
		FSM::stateId<S6>(),
		FSM::stateId<S7>(),
		FSM::stateId<S8>(),
		FSM::stateId<S9>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Composite BST", "[machine]") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<S0>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::ENTER },
			{ FSM::stateId<S0>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<S0>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.changeTo<S1>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<S1>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<S0>(),	 Event::UPDATE },

			{ FSM::stateId<S0>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<S1>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<S0>(),	 Event::EXIT },
			{ FSM::stateId<S1>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<S1>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<S0>(),
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////

}
