// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_select {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Selectable<S(S),
						S(S_0),
						S(S_1),
						S(S_2),
						S(S_3),
						S(S_4)
					>,
					M::Composite<S(C),
						S(C_0),
						S(C_1),
						S(C_2),
						S(C_3),
						S(C_4)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>() ==  0, "");
static_assert(FSM::regionId<O   >() ==  1, "");
static_assert(FSM::regionId<S   >() ==  2, "");
static_assert(FSM::regionId<C   >() ==  3, "");

static_assert(FSM::stateId<Apex>() ==  0, "");
static_assert(FSM::stateId<I   >() ==  1, "");
static_assert(FSM::stateId<O   >() ==  2, "");
static_assert(FSM::stateId<S   >() ==  3, "");
static_assert(FSM::stateId<S_0 >() ==  4, "");
static_assert(FSM::stateId<S_1 >() ==  5, "");
static_assert(FSM::stateId<S_2 >() ==  6, "");
static_assert(FSM::stateId<S_3 >() ==  7, "");
static_assert(FSM::stateId<S_4 >() ==  8, "");
static_assert(FSM::stateId<C   >() ==  9, "");
static_assert(FSM::stateId<C_0 >() == 10, "");
static_assert(FSM::stateId<C_1 >() == 11, "");
static_assert(FSM::stateId<C_2 >() == 12, "");
static_assert(FSM::stateId<C_3 >() == 13, "");
static_assert(FSM::stateId<C_4 >() == 14, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	: FSM::State {};

struct I : FSM::State {};

struct O : FSM::State {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct S : FSM::State { Short select(const Control&) { return 1; }; };

struct S_0 : FSM::State {};
struct S_1 : FSM::State {};
struct S_2 : FSM::State {};
struct S_3 : FSM::State {};
struct S_4 : FSM::State {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct C : FSM::State { Short select(const Control&) { return 3; }; };

struct C_0 : FSM::State {};
struct C_1 : FSM::State {};
struct C_2 : FSM::State {};
struct C_3 : FSM::State {};
struct C_4 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 15, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  4, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  3, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 12, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<I   >(),
	FSM::stateId<O   >(),
	FSM::stateId<S   >(),
	FSM::stateId<S_0 >(),
	FSM::stateId<S_1 >(),
	FSM::stateId<S_2 >(),
	FSM::stateId<S_3 >(),
	FSM::stateId<S_4 >(),
	FSM::stateId<C   >(),
	FSM::stateId<C_0 >(),
	FSM::stateId<C_1 >(),
	FSM::stateId<C_2 >(),
	FSM::stateId<C_3 >(),
	FSM::stateId<C_4 >(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Select") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		logger.assertSequence({
			{ FSM::stateId<Apex>(),	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<I   >(),	Event::Type::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::Type::ENTER },
			{ FSM::stateId<I   >(),	Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex>(),
			FSM::stateId<I   >(),
		});

		assertResumable(machine, all, {});
	}

	WHEN("Changing to O") {
		machine.immediateChangeTo<O>();

		logger.assertSequence({
			{						Event::Type::CHANGE, FSM::stateId<O   >() },

			{ FSM::stateId<S   >(), Event::Type::REPORT_SELECT },
			{ FSM::stateId<S   >(), Event::Type::SELECT_RESOLUTION, 1 },

			{ FSM::stateId<I   >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S_1 >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_0 >(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I   >(), Event::Type::EXIT },

			{ FSM::stateId<O   >(), Event::Type::ENTER },
			{ FSM::stateId<S   >(), Event::Type::ENTER },
			{ FSM::stateId<S_1 >(), Event::Type::ENTER },
			{ FSM::stateId<C   >(), Event::Type::ENTER },
			{ FSM::stateId<C_0 >(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex>(),
			FSM::stateId<O   >(),
			FSM::stateId<S   >(),
			FSM::stateId<S_1 >(),
			FSM::stateId<C   >(),
			FSM::stateId<C_0 >(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}

	WHEN("Restarting O") {
		machine.immediateRestart<O>();

		logger.assertSequence({
			{						Event::Type::RESTART, FSM::stateId<O   >() },

			{ FSM::stateId<I   >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S_0 >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_0 >(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I   >(), Event::Type::EXIT },

			{ FSM::stateId<O   >(), Event::Type::ENTER },
			{ FSM::stateId<S   >(), Event::Type::ENTER },
			{ FSM::stateId<S_0 >(), Event::Type::ENTER },
			{ FSM::stateId<C   >(), Event::Type::ENTER },
			{ FSM::stateId<C_0 >(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex>(),
			FSM::stateId<O   >(),
			FSM::stateId<S   >(),
			FSM::stateId<S_0 >(),
			FSM::stateId<C   >(),
			FSM::stateId<C_0 >(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}

	WHEN("Selecting O") {
		machine.immediateSelect<O>();

		logger.assertSequence({
			{						Event::Type::SELECT, FSM::stateId<O   >() },

			{ FSM::stateId<S   >(), Event::Type::REPORT_SELECT },
			{ FSM::stateId<S   >(), Event::Type::SELECT_RESOLUTION, 1 },

			{ FSM::stateId<C   >(), Event::Type::REPORT_SELECT },
			{ FSM::stateId<C   >(), Event::Type::SELECT_RESOLUTION, 3 },

			{ FSM::stateId<I   >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<S_1 >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C   >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_3 >(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I   >(), Event::Type::EXIT },

			{ FSM::stateId<O   >(), Event::Type::ENTER },
			{ FSM::stateId<S   >(), Event::Type::ENTER },
			{ FSM::stateId<S_1 >(), Event::Type::ENTER },
			{ FSM::stateId<C   >(), Event::Type::ENTER },
			{ FSM::stateId<C_3 >(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex>(),
			FSM::stateId<O   >(),
			FSM::stateId<S   >(),
			FSM::stateId<S_1 >(),
			FSM::stateId<C   >(),
			FSM::stateId<C_3 >(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}
}

////////////////////////////////////////////////////////////////////////////////

}
