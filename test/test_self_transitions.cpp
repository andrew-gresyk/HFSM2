// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_self_transitions {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(A),
					S(A_1),
					M::Composite<S(A_2),
						S(A_2_1),
						S(A_2_2)
					>
				>,
				M::Orthogonal<S(B),
					M::Composite<S(B_1),
						S(B_1_1),
						S(B_1_2)
					>,
					M::Composite<S(B_2),
						S(B_2_1),
						S(B_2_2)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<A   >() ==  1, "");
static_assert(FSM::regionId<A_2 >() ==  2, "");
static_assert(FSM::regionId<B   >() ==  3, "");
static_assert(FSM::regionId<B_1 >() ==  4, "");
static_assert(FSM::regionId<B_2 >() ==  5, "");

static_assert(FSM::stateId<A    >() ==  1, "");
static_assert(FSM::stateId<A_1  >() ==  2, "");
static_assert(FSM::stateId<A_2  >() ==  3, "");
static_assert(FSM::stateId<A_2_1>() ==  4, "");
static_assert(FSM::stateId<A_2_2>() ==  5, "");
static_assert(FSM::stateId<B    >() ==  6, "");
static_assert(FSM::stateId<B_1  >() ==  7, "");
static_assert(FSM::stateId<B_1_1>() ==  8, "");
static_assert(FSM::stateId<B_1_2>() ==  9, "");
static_assert(FSM::stateId<B_2  >() == 10, "");
static_assert(FSM::stateId<B_2_1>() == 11, "");
static_assert(FSM::stateId<B_2_2>() == 12, "");

////////////////////////////////////////////////////////////////////////////////

struct A	 : FSM::State {};
struct A_1	 : FSM::State {};
struct A_2	 : FSM::State {};
struct A_2_1 : FSM::State {};
struct A_2_2 : FSM::State {};
struct B	 : FSM::State {};
struct B_1	 : FSM::State {};
struct B_1_1 : FSM::State {};
struct B_1_2 : FSM::State {};
struct B_2	 : FSM::State {};
struct B_2_1 : FSM::State {};
struct B_2_2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<A    >(),
	FSM::stateId<A_1  >(),
	FSM::stateId<A_2  >(),
	FSM::stateId<A_2_1>(),
	FSM::stateId<A_2_2>(),
	FSM::stateId<B    >(),
	FSM::stateId<B_1  >(),
	FSM::stateId<B_1_1>(),
	FSM::stateId<B_1_2>(),
	FSM::stateId<B_2  >(),
	FSM::stateId<B_2_1>(),
	FSM::stateId<B_2_2>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Self Transition") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ hfsm2::StateID{0}, 	 Event::Type::ENTRY_GUARD },
				{ FSM::stateId<A    >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<A_1  >(), Event::Type::ENTRY_GUARD },

				{ hfsm2::StateID{0}, 	 Event::Type::ENTER },
				{ FSM::stateId<A    >(), Event::Type::ENTER },
				{ FSM::stateId<A_1  >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<A    >();
		machine.update();
		{
			logger.assertSequence({
				{						 Event::Type::CHANGE,		FSM::stateId<A    >() },

				{ hfsm2::StateID{0}, 	 Event::Type::UPDATE },
				{ FSM::stateId<A    >(), Event::Type::UPDATE },
				{ FSM::stateId<A_1  >(), Event::Type::UPDATE },

				{ FSM::stateId<A    >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<A_1  >(), Event::Type::EXIT_GUARD },

				{ FSM::stateId<A    >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<A_1  >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<A    >(), Event::Type::REENTER },
				{ FSM::stateId<A_1  >(), Event::Type::REENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<B    >();
		machine.update();
		{
			logger.assertSequence({
				{						 Event::Type::CHANGE, FSM::stateId<B    >() },

				{ hfsm2::StateID{0}, 	 Event::Type::UPDATE },
				{ FSM::stateId<A    >(), Event::Type::UPDATE },
				{ FSM::stateId<A_1  >(), Event::Type::UPDATE },

				{ FSM::stateId<A    >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<A_1  >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B    >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_1  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_1_1>(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_2  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_2_1>(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<A_1  >(), Event::Type::EXIT },
				{ FSM::stateId<A    >(), Event::Type::EXIT },

				{ FSM::stateId<B    >(), Event::Type::ENTER },
				{ FSM::stateId<B_1  >(), Event::Type::ENTER },
				{ FSM::stateId<B_1_1>(), Event::Type::ENTER },
				{ FSM::stateId<B_2  >(), Event::Type::ENTER },
				{ FSM::stateId<B_2_1>(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<B    >();
		machine.update();
		{
			logger.assertSequence({
				{						 Event::Type::CHANGE, FSM::stateId<B    >() },

				{ hfsm2::StateID{0}, 	 Event::Type::UPDATE },
				{ FSM::stateId<B    >(), Event::Type::UPDATE },
				{ FSM::stateId<B_1  >(), Event::Type::UPDATE },
				{ FSM::stateId<B_1_1>(), Event::Type::UPDATE },
				{ FSM::stateId<B_2  >(), Event::Type::UPDATE },
				{ FSM::stateId<B_2_1>(), Event::Type::UPDATE },

				{ FSM::stateId<B    >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_1  >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_1_1>(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_2  >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_2_1>(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B    >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_1  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_1_1>(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_2  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<B_2_1>(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<B    >(), Event::Type::REENTER },
				{ FSM::stateId<B_1  >(), Event::Type::REENTER },
				{ FSM::stateId<B_1_1>(), Event::Type::REENTER },
				{ FSM::stateId<B_2  >(), Event::Type::REENTER },
				{ FSM::stateId<B_2_1>(), Event::Type::REENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	const Events reference = {
		{ FSM::stateId<B_1_1>(), Event::Type::EXIT },
		{ FSM::stateId<B_1  >(), Event::Type::EXIT },
		{ FSM::stateId<B_2_1>(), Event::Type::EXIT },
		{ FSM::stateId<B_2  >(), Event::Type::EXIT },
		{ FSM::stateId<B    >(), Event::Type::EXIT },
		{ hfsm2::StateID{0}, 	 Event::Type::EXIT },
	};
	logger.assertSequence(reference);
}

////////////////////////////////////////////////////////////////////////////////

}
