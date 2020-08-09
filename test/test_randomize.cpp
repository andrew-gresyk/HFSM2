#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_randomize {

//------------------------------------------------------------------------------

using Config = hfsm2::Config::RandomT<hfsm2::XoShiRo256Plus>;

using M = hfsm2::MachineT<Config>;

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

static_assert(FSM::regionId<Apex>() ==  0, "");
static_assert(FSM::regionId<O   >() ==  1, "");
static_assert(FSM::regionId<N   >() ==  2, "");
static_assert(FSM::regionId<C   >() ==  3, "");

static_assert(FSM::stateId<Apex >() ==  0, "");
static_assert(FSM::stateId<I    >() ==  1, "");
static_assert(FSM::stateId<O    >() ==  2, "");
static_assert(FSM::stateId<N    >() ==  3, "");
static_assert(FSM::stateId<N_000>() ==  4, "");
static_assert(FSM::stateId<N_025>() ==  5, "");
static_assert(FSM::stateId<N_050>() ==  6, "");
static_assert(FSM::stateId<N_075>() ==  7, "");
static_assert(FSM::stateId<N_100>() ==  8, "");
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
	FSM::stateId<N    >(),
	FSM::stateId<N_000>(),
	FSM::stateId<N_025>(),
	FSM::stateId<N_050>(),
	FSM::stateId<N_075>(),
	FSM::stateId<N_100>(),
	FSM::stateId<C    >(),
	FSM::stateId<C_000>(),
	FSM::stateId<C_025>(),
	FSM::stateId<C_050>(),
	FSM::stateId<C_075>(),
	FSM::stateId<C_100>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Randomize", "[machine]") {
	hfsm2::XoShiRo256Plus generator{0};
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{generator, &logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex >(), Event::ENTRY_GUARD },
				{ FSM::stateId<I    >(), Event::ENTRY_GUARD },

				{ FSM::stateId<Apex >(), Event::CONSTRUCT },
				{ FSM::stateId<I    >(), Event::CONSTRUCT },

				{ FSM::stateId<Apex >(), Event::ENTER },
				{ FSM::stateId<I    >(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex >(),
				FSM::stateId<I    >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Changing to O") {
			machine.changeTo<O>();
			machine.update();
			{
				logger.assertSequence({
					{						 Event::CHANGE,		FSM::stateId<O    >() },

					{ FSM::stateId<Apex >(), Event::UPDATE },
					{ FSM::stateId<I    >(), Event::UPDATE },

					{ FSM::stateId<N_000>(), Event::RANK },
					{ FSM::stateId<N_025>(), Event::RANK },
					{ FSM::stateId<N_050>(), Event::RANK },
					{ FSM::stateId<N_075>(), Event::RANK },
					{ FSM::stateId<N_100>(), Event::RANK },

					{ FSM::stateId<N_000>(), Event::UTILITY },
					{ FSM::stateId<N_025>(), Event::UTILITY },
					{ FSM::stateId<N_075>(), Event::UTILITY },

					{ FSM::stateId<N    >(), Event::RANDOM_RESOLUTION, 3 },

					{ FSM::stateId<I    >(), Event::EXIT_GUARD },
					{ FSM::stateId<O    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N_075>(), Event::ENTRY_GUARD },
					{ FSM::stateId<C    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

					{ FSM::stateId<I    >(), Event::EXIT },

					{ FSM::stateId<I    >(), Event::DESTRUCT },
					{ FSM::stateId<O    >(), Event::CONSTRUCT },
					{ FSM::stateId<N    >(), Event::CONSTRUCT },
					{ FSM::stateId<N_075>(), Event::CONSTRUCT },
					{ FSM::stateId<C    >(), Event::CONSTRUCT },
					{ FSM::stateId<C_000>(), Event::CONSTRUCT },

					{ FSM::stateId<O    >(), Event::ENTER },
					{ FSM::stateId<N    >(), Event::ENTER },
					{ FSM::stateId<N_075>(), Event::ENTER },
					{ FSM::stateId<C    >(), Event::ENTER },
					{ FSM::stateId<C_000>(), Event::ENTER },
				});

				assertActive(machine, all, {
					FSM::stateId<Apex >(),
					FSM::stateId<O    >(),
					FSM::stateId<N    >(),
					FSM::stateId<N_075>(),
					FSM::stateId<C    >(),
					FSM::stateId<C_000>(),
				});

				assertResumable(machine, all, {
					FSM::stateId<I    >(),
				});
			}
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Restarting O") {
			machine.restart<O>();
			machine.update();
			{
				logger.assertSequence({
					{						 Event::RESTART,	FSM::stateId<O    >() },

					{ FSM::stateId<Apex >(), Event::UPDATE },
					{ FSM::stateId<I    >(), Event::UPDATE },

					{ FSM::stateId<I    >(), Event::EXIT_GUARD },
					{ FSM::stateId<O    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N_000>(), Event::ENTRY_GUARD },
					{ FSM::stateId<C    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

					{ FSM::stateId<I    >(), Event::EXIT },

					{ FSM::stateId<I    >(), Event::DESTRUCT },
					{ FSM::stateId<O    >(), Event::CONSTRUCT },
					{ FSM::stateId<N    >(), Event::CONSTRUCT },
					{ FSM::stateId<N_000>(), Event::CONSTRUCT },
					{ FSM::stateId<C    >(), Event::CONSTRUCT },
					{ FSM::stateId<C_000>(), Event::CONSTRUCT },

					{ FSM::stateId<O    >(), Event::ENTER },
					{ FSM::stateId<N    >(), Event::ENTER },
					{ FSM::stateId<N_000>(), Event::ENTER },
					{ FSM::stateId<C    >(), Event::ENTER },
					{ FSM::stateId<C_000>(), Event::ENTER },
				});

				assertActive(machine, all, {
					FSM::stateId<Apex >(),
					FSM::stateId<O    >(),
					FSM::stateId<N    >(),
					FSM::stateId<N_000>(),
					FSM::stateId<C    >(),
					FSM::stateId<C_000>(),
				});

				assertResumable(machine, all, {
					FSM::stateId<I    >(),
				});
			}
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Randomizing O") {
			machine.randomize<O>();
			machine.update();
			{
				logger.assertSequence({
					{						 Event::RANDOMIZE,	FSM::stateId<O    >() },

					{ FSM::stateId<Apex >(), Event::UPDATE },
					{ FSM::stateId<I    >(), Event::UPDATE },

					{ FSM::stateId<N_000>(), Event::RANK },
					{ FSM::stateId<N_025>(), Event::RANK },
					{ FSM::stateId<N_050>(), Event::RANK },
					{ FSM::stateId<N_075>(), Event::RANK },
					{ FSM::stateId<N_100>(), Event::RANK },

					{ FSM::stateId<N_000>(), Event::UTILITY },
					{ FSM::stateId<N_025>(), Event::UTILITY },
					{ FSM::stateId<N_075>(), Event::UTILITY },

					{ FSM::stateId<N    >(), Event::RANDOM_RESOLUTION, 3 },

					{ FSM::stateId<C_000>(), Event::RANK },
					{ FSM::stateId<C_025>(), Event::RANK },
					{ FSM::stateId<C_050>(), Event::RANK },
					{ FSM::stateId<C_075>(), Event::RANK },
					{ FSM::stateId<C_100>(), Event::RANK },

					{ FSM::stateId<C_000>(), Event::UTILITY },
					{ FSM::stateId<C_025>(), Event::UTILITY },
					{ FSM::stateId<C_075>(), Event::UTILITY },

					{ FSM::stateId<C    >(), Event::RANDOM_RESOLUTION, 1 },

					{ FSM::stateId<I    >(), Event::EXIT_GUARD },
					{ FSM::stateId<O    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<N_075>(), Event::ENTRY_GUARD },
					{ FSM::stateId<C    >(), Event::ENTRY_GUARD },
					{ FSM::stateId<C_025>(), Event::ENTRY_GUARD },

					{ FSM::stateId<I    >(), Event::EXIT },

					{ FSM::stateId<I    >(), Event::DESTRUCT },
					{ FSM::stateId<O    >(), Event::CONSTRUCT },
					{ FSM::stateId<N    >(), Event::CONSTRUCT },
					{ FSM::stateId<N_075>(), Event::CONSTRUCT },
					{ FSM::stateId<C    >(), Event::CONSTRUCT },
					{ FSM::stateId<C_025>(), Event::CONSTRUCT },

					{ FSM::stateId<O    >(), Event::ENTER },
					{ FSM::stateId<N    >(), Event::ENTER },
					{ FSM::stateId<N_075>(), Event::ENTER },
					{ FSM::stateId<C    >(), Event::ENTER },
					{ FSM::stateId<C_025>(), Event::ENTER },
				});

				assertActive(machine, all, {
					FSM::stateId<Apex >(),
					FSM::stateId<O    >(),
					FSM::stateId<N    >(),
					FSM::stateId<N_075>(),
					FSM::stateId<C    >(),
					FSM::stateId<C_025>(),
				});

				assertResumable(machine, all, {
					FSM::stateId<I    >(),
				});
			}
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}
}

////////////////////////////////////////////////////////////////////////////////

}

#include <random>
