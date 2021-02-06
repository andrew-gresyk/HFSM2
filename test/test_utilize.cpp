// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_utilize {

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

static_assert(FSM::regionId<Apex>() ==  0, "");
static_assert(FSM::regionId<O   >() ==  1, "");
static_assert(FSM::regionId<U   >() ==  2, "");
static_assert(FSM::regionId<C   >() ==  3, "");

static_assert(FSM::stateId<Apex >() ==  0, "");
static_assert(FSM::stateId<I    >() ==  1, "");
static_assert(FSM::stateId<O    >() ==  2, "");
static_assert(FSM::stateId<U    >() ==  3, "");
static_assert(FSM::stateId<U_000>() ==  4, "");
static_assert(FSM::stateId<U_025>() ==  5, "");
static_assert(FSM::stateId<U_050>() ==  6, "");
static_assert(FSM::stateId<U_075>() ==  7, "");
static_assert(FSM::stateId<U_100>() ==  8, "");
static_assert(FSM::stateId<C    >() ==  9, "");
static_assert(FSM::stateId<C_000>() == 10, "");
static_assert(FSM::stateId<C_025>() == 11, "");
static_assert(FSM::stateId<C_050>() == 12, "");
static_assert(FSM::stateId<C_075>() == 13, "");
static_assert(FSM::stateId<C_100>() == 14, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};

struct I : FSM::State {};

struct O : FSM::State {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct U : FSM::State {};

struct U_000 : FSM::State { Utility utility(const Control&) { return 0.00f; } };
struct U_025 : FSM::State { Utility utility(const Control&) { return 0.25f; } };
struct U_050 : FSM::State { Utility utility(const Control&) { return 0.50f; } };
struct U_075 : FSM::State { Utility utility(const Control&) { return 0.75f; } };
struct U_100 : FSM::State { Utility utility(const Control&) { return 1.00f; } };

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct C : FSM::State {};

struct C_000 : FSM::State { Utility utility(const Control&) { return 0.00f; } };
struct C_025 : FSM::State { Utility utility(const Control&) { return 0.25f; } };
struct C_050 : FSM::State { Utility utility(const Control&) { return 0.50f; } };
struct C_075 : FSM::State { Utility utility(const Control&) { return 0.75f; } };
struct C_100 : FSM::State { Utility utility(const Control&) { return 1.00f; } };

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 15, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  4, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  3, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 12, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<I    >(),
	FSM::stateId<O    >(),
	FSM::stateId<U    >(),
	FSM::stateId<U_000>(),
	FSM::stateId<U_025>(),
	FSM::stateId<U_050>(),
	FSM::stateId<U_075>(),
	FSM::stateId<U_100>(),
	FSM::stateId<C    >(),
	FSM::stateId<C_000>(),
	FSM::stateId<C_025>(),
	FSM::stateId<C_050>(),
	FSM::stateId<C_075>(),
	FSM::stateId<C_100>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Utilize") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		logger.assertSequence({
			{ FSM::stateId<Apex>(),	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<I   >(),	Event::Type::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::Type::CONSTRUCT },
			{ FSM::stateId<I   >(),	Event::Type::CONSTRUCT },

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
		machine.changeTo<O>();
		machine.update();

		logger.assertSequence({
			{						 Event::Type::CHANGE, FSM::stateId<O   >() },

			{ FSM::stateId<Apex >(), Event::Type::UPDATE },
			{ FSM::stateId<I    >(), Event::Type::UPDATE },

			{ FSM::stateId<U_000>(), Event::Type::UTILITY },
			{ FSM::stateId<U_025>(), Event::Type::UTILITY },
			{ FSM::stateId<U_050>(), Event::Type::UTILITY },
			{ FSM::stateId<U_075>(), Event::Type::UTILITY },
			{ FSM::stateId<U_100>(), Event::Type::UTILITY },

			{ FSM::stateId<U    >(), Event::Type::UTILITY_RESOLUTION, 4 },

			{ FSM::stateId<I    >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U_100>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_000>(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I    >(), Event::Type::EXIT },

			{ FSM::stateId<I    >(), Event::Type::DESTRUCT },
			{ FSM::stateId<O    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U_100>(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C_000>(), Event::Type::CONSTRUCT },

			{ FSM::stateId<O    >(), Event::Type::ENTER },
			{ FSM::stateId<U    >(), Event::Type::ENTER },
			{ FSM::stateId<U_100>(), Event::Type::ENTER },
			{ FSM::stateId<C    >(), Event::Type::ENTER },
			{ FSM::stateId<C_000>(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex >(),
			FSM::stateId<O    >(),
			FSM::stateId<U    >(),
			FSM::stateId<U_100>(),
			FSM::stateId<C    >(),
			FSM::stateId<C_000>(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}

	WHEN("Restarting O") {
		machine.restart<O>();
		machine.update();

		logger.assertSequence({
			{						 Event::Type::RESTART, FSM::stateId<O   >() },

			{ FSM::stateId<Apex >(), Event::Type::UPDATE },
			{ FSM::stateId<I    >(), Event::Type::UPDATE },

			{ FSM::stateId<I    >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U_000>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_000>(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I    >(), Event::Type::EXIT },

			{ FSM::stateId<I    >(), Event::Type::DESTRUCT },
			{ FSM::stateId<O    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U_000>(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C_000>(), Event::Type::CONSTRUCT },

			{ FSM::stateId<O    >(), Event::Type::ENTER },
			{ FSM::stateId<U    >(), Event::Type::ENTER },
			{ FSM::stateId<U_000>(), Event::Type::ENTER },
			{ FSM::stateId<C    >(), Event::Type::ENTER },
			{ FSM::stateId<C_000>(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex >(),
			FSM::stateId<O    >(),
			FSM::stateId<U    >(),
			FSM::stateId<U_000>(),
			FSM::stateId<C    >(),
			FSM::stateId<C_000>(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}

	WHEN("Utilizing O") {
		machine.utilize<O>();
		machine.update();

		logger.assertSequence({
			{						 Event::Type::UTILIZE, FSM::stateId<O   >() },

			{ FSM::stateId<Apex >(), Event::Type::UPDATE },
			{ FSM::stateId<I    >(), Event::Type::UPDATE },

			{ FSM::stateId<U_000>(), Event::Type::UTILITY },
			{ FSM::stateId<U_025>(), Event::Type::UTILITY },
			{ FSM::stateId<U_050>(), Event::Type::UTILITY },
			{ FSM::stateId<U_075>(), Event::Type::UTILITY },
			{ FSM::stateId<U_100>(), Event::Type::UTILITY },

			{ FSM::stateId<U    >(), Event::Type::UTILITY_RESOLUTION, 4 },

			{ FSM::stateId<C_000>(), Event::Type::UTILITY },
			{ FSM::stateId<C_025>(), Event::Type::UTILITY },
			{ FSM::stateId<C_050>(), Event::Type::UTILITY },
			{ FSM::stateId<C_075>(), Event::Type::UTILITY },
			{ FSM::stateId<C_100>(), Event::Type::UTILITY },

			{ FSM::stateId<C    >(), Event::Type::UTILITY_RESOLUTION, 4 },

			{ FSM::stateId<I    >(), Event::Type::EXIT_GUARD },
			{ FSM::stateId<O    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<U_100>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C    >(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<C_100>(), Event::Type::ENTRY_GUARD },

			{ FSM::stateId<I    >(), Event::Type::EXIT },

			{ FSM::stateId<I    >(), Event::Type::DESTRUCT },
			{ FSM::stateId<O    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<U_100>(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C    >(), Event::Type::CONSTRUCT },
			{ FSM::stateId<C_100>(), Event::Type::CONSTRUCT },

			{ FSM::stateId<O    >(), Event::Type::ENTER },
			{ FSM::stateId<U    >(), Event::Type::ENTER },
			{ FSM::stateId<U_100>(), Event::Type::ENTER },
			{ FSM::stateId<C    >(), Event::Type::ENTER },
			{ FSM::stateId<C_100>(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<Apex >(),
			FSM::stateId<O    >(),
			FSM::stateId<U    >(),
			FSM::stateId<U_100>(),
			FSM::stateId<C    >(),
			FSM::stateId<C_100>(),
		});

		assertResumable(machine, all, {
			FSM::stateId<I   >(),
		});
	}
}

////////////////////////////////////////////////////////////////////////////////

}
