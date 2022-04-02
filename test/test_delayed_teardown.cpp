// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_PLANS
#define HFSM2_ENABLE_TRANSITION_HISTORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_delayed_teardown {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Orthogonal<S(Step1),
					S(Step1_1),
					M::Composite<S(Step1_2),
						S(Setup),
						S(Work),
						S(Teardown)
					>
				>,
				S(Step2)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex >()	==  0, "");
static_assert(FSM::regionId<Step1>()	==  1, "");

static_assert(FSM::stateId<Apex    >()	==  0, "");
static_assert(FSM::stateId<Step1   >()	==  1, "");
static_assert(FSM::stateId<Step1_1 >()	==  2, "");
static_assert(FSM::stateId<Step1_2 >()	==  3, "");
static_assert(FSM::stateId<Setup   >()	==  4, "");
static_assert(FSM::stateId<Work    >()	==  5, "");
static_assert(FSM::stateId<Teardown>()	==  6, "");
static_assert(FSM::stateId<Step2   >()	==  7, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	   : FSM::State {};
struct Step1   : FSM::State {};
struct Step1_1 : FSM::State {};
struct Step1_2 : FSM::State {};

//------------------------------------------------------------------------------

struct Setup
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Work>();
	}
};

struct Work
	: FSM::State
{
	void exitGuard(GuardControl& control) {
		if (!control.isPendingEnter<Teardown>()) {
			REQUIRE(control.currentTransitions().count() == 0);

			const auto& pendingTransitions = control.pendingTransitions();
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2   >(),
														   hfsm2::TransitionType::CHANGE});

			const auto& requests = control.requests();
			REQUIRE(requests.count() == 0);

			for (const auto& transition : control.pendingTransitions()) {
				// workaround for the new doctest
				const bool success = control.plan().change<Teardown>(transition.destination);

				REQUIRE(success);
			}

			control.cancelPendingTransitions();
			control.changeTo<Teardown>();

			REQUIRE(requests.count() == 1);
			REQUIRE(requests[0] == M::Transition{FSM::stateId<Work    >(),
												 FSM::stateId<Teardown>(),
												 hfsm2::TransitionType::CHANGE});
		} else {
			REQUIRE(control.currentTransitions().count() == 0);

			const auto& pendingTransitions = control.pendingTransitions();
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Work    >(),
														   FSM::stateId<Teardown>(),
														   hfsm2::TransitionType::CHANGE});

			REQUIRE(control.requests().count() == 0);
		}
	}
};

struct Teardown
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Step2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 8, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  == 3, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS == 2, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 5, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS == 1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   == 1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<Step1   >(),
	FSM::stateId<Step1_1 >(),
	FSM::stateId<Step1_2 >(),
	FSM::stateId<Setup   >(),
	FSM::stateId<Work    >(),
	FSM::stateId<Teardown>(),
	FSM::stateId<Step2   >(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Delayed Teardown") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex    >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_1 >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_2 >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Setup   >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Apex    >(),	Event::Type::ENTER },
				{ FSM::stateId<Step1   >(),	Event::Type::ENTER },
				{ FSM::stateId<Step1_1 >(),	Event::Type::ENTER },
				{ FSM::stateId<Step1_2 >(),	Event::Type::ENTER },
				{ FSM::stateId<Setup   >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex    >(),
				FSM::stateId<Step1   >(),
				FSM::stateId<Step1_1 >(),
				FSM::stateId<Step1_2 >(),
				FSM::stateId<Setup   >(),
			});

			assertResumable(machine, all, {});

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<Apex    >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Setup   >(),	Event::Type::PRE_UPDATE },

				{ FSM::stateId<Apex    >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Setup   >(),	Event::Type::UPDATE },

				{ FSM::stateId<Setup   >(),	Event::Type::CHANGE,		FSM::stateId<Work   >() },

				{ FSM::stateId<Step1_1 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Setup   >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Apex    >(),	Event::Type::POST_UPDATE },

				{ FSM::stateId<Setup   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Work    >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Setup   >(),	Event::Type::EXIT },
				{ FSM::stateId<Work    >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex    >(),
				FSM::stateId<Step1   >(),
				FSM::stateId<Step1_1 >(),
				FSM::stateId<Step1_2 >(),
				FSM::stateId<Work    >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Setup   >()
			});

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<Setup   >(),
															FSM::stateId<Work    >(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(machine, all, {
				FSM::stateId<Work    >(),
			});

			REQUIRE(machine.lastTransitionTo<Work    >() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<Step2   >();
		machine.update();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,		FSM::stateId<Step2>() },

				{ FSM::stateId<Apex    >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Work    >(),	Event::Type::PRE_UPDATE },

				{ FSM::stateId<Apex    >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Work    >(),	Event::Type::UPDATE },

				{ FSM::stateId<Step1_1 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Work    >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Apex    >(),	Event::Type::POST_UPDATE },

				{ FSM::stateId<Step1   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_1 >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2 >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Work    >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Work    >(),	Event::Type::CANCEL_PENDING },

				{ FSM::stateId<Work    >(),	Event::Type::CHANGE,		FSM::stateId<Teardown>() },

				{ FSM::stateId<Work    >(),	Event::Type::EXIT_GUARD  },
				{ FSM::stateId<Teardown>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Work    >(),	Event::Type::EXIT  },
				{ FSM::stateId<Teardown>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex    >(),
				FSM::stateId<Step1   >(),
				FSM::stateId<Step1_1 >(),
				FSM::stateId<Step1_2 >(),
				FSM::stateId<Teardown>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Work    >()
			});

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<Work    >(),
															FSM::stateId<Teardown>(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(machine, all, {
				FSM::stateId<Teardown>(),
			});

			REQUIRE(machine.lastTransitionTo<Teardown>() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<Apex    >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::PRE_UPDATE },
				{ FSM::stateId<Teardown>(),	Event::Type::PRE_UPDATE },

				{ FSM::stateId<Apex    >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_1 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::UPDATE },
				{ FSM::stateId<Teardown>(),	Event::Type::UPDATE },

				{ FSM::regionId<Step1_2>(),	Event::Type::TASK_SUCCESS,	FSM::stateId<Teardown>() },

				{ FSM::stateId<Step1_1 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Teardown>(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1_2 >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1   >(),	Event::Type::POST_UPDATE },
				{ FSM::stateId<Apex    >(),	Event::Type::POST_UPDATE },

				{ FSM::stateId<Step1_2 >(),	Event::Type::CHANGE,		FSM::stateId<Step2   >() },

				{ FSM::stateId<Step1   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_1 >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2 >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Teardown>(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Step2   >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Step1_1 >(),	Event::Type::EXIT },
				{ FSM::stateId<Teardown>(),	Event::Type::EXIT },
				{ FSM::stateId<Step1_2 >(),	Event::Type::EXIT },
				{ FSM::stateId<Step1   >(),	Event::Type::EXIT },

				{ FSM::stateId<Step2   >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex    >(),
				FSM::stateId<Step2   >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1   >(),
				FSM::stateId<Step1_1 >(),
				FSM::stateId<Step1_2 >(),
				FSM::stateId<Teardown>()
			});

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<Step1_2 >(),
															FSM::stateId<Step2   >(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(machine, all, {
				FSM::stateId<Step2   >(),
			});

			REQUIRE(machine.lastTransitionTo<Step2   >() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<Step2   >(),	Event::Type::EXIT },
		{ FSM::stateId<Apex    >(),	Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
