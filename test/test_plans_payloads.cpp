// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_PLANS
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_plans_payloads {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::PayloadT<int>;

using M = hfsm2::MachineT<Config>;

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Composite<S(Planned),
					M::Composite<S(Step1_BT),
						S(Step1_1),
						S(Step1_2),
						S(Step1_3)
					>,
					M::Orthogonal<S(Hybrid),
						M::Composite<S(Step2L_P),
							S(Step2L_1),
							S(Step2L_2)
						>,
						M::Composite<S(Step2R_P),
							S(Step2R_1),
							S(Step2R_2)
						>
					>,
					M::Orthogonal<S(Terminal),
						S(Terminal_L),
						S(Terminal_R)
					>
				>,
				M::Composite<S(Unplanned),
					S(Work_1),
					S(Work_2)
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex     >() ==  0, "");
static_assert(FSM::regionId<Planned  >() ==  1, "");
static_assert(FSM::regionId<Step1_BT >() ==  2, "");
static_assert(FSM::regionId<Hybrid   >() ==  3, "");
static_assert(FSM::regionId<Step2L_P >() ==  4, "");
static_assert(FSM::regionId<Step2R_P >() ==  5, "");
static_assert(FSM::regionId<Terminal >() ==  6, "");
static_assert(FSM::regionId<Unplanned>() ==  7, "");

static_assert(FSM::stateId<Apex      >() ==  0, "");
static_assert(FSM::stateId<Planned   >() ==  1, "");
static_assert(FSM::stateId<Step1_BT  >() ==  2, "");
static_assert(FSM::stateId<Step1_1   >() ==  3, "");
static_assert(FSM::stateId<Step1_2   >() ==  4, "");
static_assert(FSM::stateId<Step1_3   >() ==  5, "");
static_assert(FSM::stateId<Hybrid    >() ==  6, "");
static_assert(FSM::stateId<Step2L_P  >() ==  7, "");
static_assert(FSM::stateId<Step2L_1  >() ==  8, "");
static_assert(FSM::stateId<Step2L_2  >() ==  9, "");
static_assert(FSM::stateId<Step2R_P  >() == 10, "");
static_assert(FSM::stateId<Step2R_1  >() == 11, "");
static_assert(FSM::stateId<Step2R_2  >() == 12, "");
static_assert(FSM::stateId<Terminal  >() == 13, "");
static_assert(FSM::stateId<Terminal_L>() == 14, "");
static_assert(FSM::stateId<Terminal_R>() == 15, "");
static_assert(FSM::stateId<Unplanned >() == 16, "");
static_assert(FSM::stateId<Work_1    >() == 17, "");
static_assert(FSM::stateId<Work_2    >() == 18, "");

////////////////////////////////////////////////////////////////////////////////
// TODO: make use of planFailed()
// TODO: build a plan with re-entry

struct Apex : FSM::State {};

//------------------------------------------------------------------------------

struct Planned
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan<Planned>();
		REQUIRE(!plan);

		REQUIRE(plan.changeWith<Step1_BT, Hybrid>(1));
		REQUIRE(plan.restartWith<Hybrid, Terminal>(2));
	}

	void planFailed(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_BT
	: FSM::State
{
	void enter(PlanControl& control) {
		Plan plan = control.plan();
		REQUIRE(!plan);

		REQUIRE(plan.resumeWith<Step1_2, Step1_3>(3));
	}
};

struct Step1_1
	: FSM::State
{
	void update(FullControl& control) {
		control.changeWith<Step1_2  >(4);
	}
};

struct Step1_2
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 4);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 4);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step1_3
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 3);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 3);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Hybrid
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 1);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 1);

		auto plan = control.plan();
		REQUIRE(!plan);

		REQUIRE(plan.utilizeWith<Step2L_1, Step2L_2>(4));
		REQUIRE(plan.randomizeWith<Step2R_1, Step2R_2>(5));
	}

	void planFailed(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2L_P : FSM::State {};

struct Step2L_1
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 1);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 1);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step2L_2
	: FSM::State
{
	Utility utility(const Control&) {
		return 2.0f;
	}

	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 2);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 4);
		REQUIRE( pendingTransitions[1].payload());
		REQUIRE(*pendingTransitions[1].payload() == 5);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 2);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 4);
		REQUIRE( currentTransitions[1].payload());
		REQUIRE(*currentTransitions[1].payload() == 5);
	}

	void update(FullControl& control) {
		control.fail();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2R_P : FSM::State {};

struct Step2R_1
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 1);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 1);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step2R_2
	: FSM::State
{
	Rank rank(const Control&) {
		return 1;
	}

	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 2);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 4);
		REQUIRE( pendingTransitions[1].payload());
		REQUIRE(*pendingTransitions[1].payload() == 5);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 2);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 4);
		REQUIRE( currentTransitions[1].payload());
		REQUIRE(*currentTransitions[1].payload() == 5);
	}

	void update(FullControl& control) {
		control.fail();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Terminal
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 2);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 2);
	}

	void planSucceeded(FullControl& control) {
		control.fail();
	}
};

struct Terminal_L
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 2);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 2);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

struct Terminal_R
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE( pendingTransitions.count() == 1);
		REQUIRE( pendingTransitions[0].payload());
		REQUIRE(*pendingTransitions[0].payload() == 2);

		REQUIRE(control.currentTransitions().count() == 0);
	}

	void enter(PlanControl& control) {
		const auto& currentTransitions = control.currentTransitions();
		REQUIRE( currentTransitions.count() == 1);
		REQUIRE( currentTransitions[0].payload());
		REQUIRE(*currentTransitions[0].payload() == 2);
	}

	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Unplanned  : FSM::State {};
struct Work_1	  : FSM::State {};
struct Work_2	  : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 19, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  8, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_COUNT	 ==  6, "COMPO_COUNT");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 14, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_COUNT	 ==  2, "ORTHO_COUNT");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  2, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<Planned  >(),
	FSM::stateId<Step1_BT >(),
	FSM::stateId<Step1_1  >(),
	FSM::stateId<Step1_2  >(),
	FSM::stateId<Step1_3  >(),
	FSM::stateId<Hybrid   >(),
	FSM::stateId<Step2L_P >(),
	FSM::stateId<Step2L_1 >(),
	FSM::stateId<Step2L_2 >(),
	FSM::stateId<Step2R_P >(),
	FSM::stateId<Step2R_1 >(),
	FSM::stateId<Step2R_2 >(),
	FSM::stateId<Unplanned>(),
	FSM::stateId<Terminal >(),
	FSM::stateId<Work_1   >(),
	FSM::stateId<Work_2   >(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Plan (payloads)") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex      >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Planned   >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_BT  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_1   >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Apex      >(), Event::Type::ENTER },
				{ FSM::stateId<Planned   >(), Event::Type::ENTER },
				{ FSM::stateId<Step1_BT  >(), Event::Type::ENTER },
				{ FSM::stateId<Step1_1   >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Step1_BT  >(),
				FSM::stateId<Step1_1   >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_BT  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId<Step1_1   >(), Event::Type::PRE_UPDATE },

				{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
				{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
				{ FSM::stateId<Step1_BT  >(), Event::Type::UPDATE },
				{ FSM::stateId<Step1_1   >(), Event::Type::UPDATE },

				{ FSM::stateId<Step1_1   >(), Event::Type::CHANGE, FSM::stateId<Step1_2  >() },

				{ FSM::stateId<Step1_1   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId<Step1_BT  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

				{ FSM::stateId<Step1_1   >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId<Step1_2   >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Step1_1   >(), Event::Type::EXIT  },
				{ FSM::stateId<Step1_2   >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Step1_BT  >(),
				FSM::stateId<Step1_2   >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1_1   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId <Apex      >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step1_2   >(), Event::Type::PRE_UPDATE },

				{ FSM::stateId <Apex      >(), Event::Type::UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step1_2   >(), Event::Type::UPDATE },

				{ FSM::regionId<Step1_BT  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step1_2  >() },

				{ FSM::stateId <Step1_2   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Apex      >(), Event::Type::POST_UPDATE },

				{ FSM::stateId <Step1_BT  >(), Event::Type::CHANGE,			FSM::stateId<Step1_3  >() },

				{ FSM::stateId <Step1_2   >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step1_3   >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId <Step1_2   >(), Event::Type::EXIT },

				{ FSM::stateId <Step1_3   >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Step1_BT  >(),
				FSM::stateId<Step1_3   >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1_2   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId <Apex      >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step1_3   >(), Event::Type::PRE_UPDATE },

				{ FSM::stateId <Apex      >(), Event::Type::UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step1_3   >(), Event::Type::UPDATE },

				{ FSM::regionId<Step1_BT  >(), Event::Type::TASK_SUCCESS,		FSM::stateId<Step1_3   >() },

				{ FSM::stateId <Step1_3   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step1_BT  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Apex      >(), Event::Type::POST_UPDATE },

				{ FSM::stateId <Step1_BT  >(), Event::Type::PLAN_SUCCEEDED },
				{ FSM::regionId<Step1_BT  >(), Event::Type::TASK_SUCCESS,		FSM::stateId<Step1_BT  >() },
				{ FSM::regionId<Step1_BT  >(), Event::Type::PLAN_SUCCESS },
				{ FSM::stateId <Planned   >(), Event::Type::CHANGE,				FSM::stateId<Hybrid    >() },

				{ FSM::stateId <Step1_BT  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step1_3   >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Hybrid    >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Step2L_P  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Step2L_1  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Step2R_P  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Step2R_1  >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId <Step1_3   >(), Event::Type::EXIT   },
				{ FSM::stateId <Step1_BT  >(), Event::Type::EXIT   },

				{ FSM::stateId <Hybrid    >(), Event::Type::ENTER  },
				{ FSM::stateId <Step2L_P  >(), Event::Type::ENTER  },
				{ FSM::stateId <Step2L_1  >(), Event::Type::ENTER  },
				{ FSM::stateId <Step2R_P  >(), Event::Type::ENTER  },
				{ FSM::stateId <Step2R_1  >(), Event::Type::ENTER  },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Hybrid    >(),
				FSM::stateId<Step2L_P  >(),
				FSM::stateId<Step2L_1  >(),
				FSM::stateId<Step2R_P  >(),
				FSM::stateId<Step2R_1  >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1_BT  >(),
				FSM::stateId<Step1_3   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId <Apex      >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2L_1  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2R_P  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2R_1  >(), Event::Type::PRE_UPDATE },

				{ FSM::stateId <Apex      >(), Event::Type::UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2L_1  >(), Event::Type::UPDATE },

				{ FSM::regionId<Step2L_P  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step2L_1  >() },

				{ FSM::stateId <Step2R_P  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2R_1  >(), Event::Type::UPDATE },

				{ FSM::regionId<Step2R_P  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step2R_1  >() },

				{ FSM::stateId <Step2L_1  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2R_1  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2R_P  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Apex      >(), Event::Type::POST_UPDATE },

				{ FSM::stateId <Hybrid    >(), Event::Type::CHANGE,			FSM::stateId<Step2L_2  >() },
				{ FSM::stateId <Hybrid    >(), Event::Type::CHANGE,			FSM::stateId<Step2R_2  >() },

				{ FSM::stateId <Step2L_1  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2R_1  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2L_2  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Step2R_2  >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId <Step2L_1  >(), Event::Type::EXIT  },
				{ FSM::stateId <Step2L_2  >(), Event::Type::ENTER },

				{ FSM::stateId <Step2R_1  >(), Event::Type::EXIT  },
				{ FSM::stateId <Step2R_2  >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Hybrid    >(),
				FSM::stateId<Step2L_P  >(),
				FSM::stateId<Step2L_2  >(),
				FSM::stateId<Step2R_P  >(),
				FSM::stateId<Step2R_2  >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1_BT  >(),
				FSM::stateId<Step1_3   >(),
				FSM::stateId<Step2L_1  >(),
				FSM::stateId<Step2R_1  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId <Apex      >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2L_2  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2R_P  >(), Event::Type::PRE_UPDATE },
				{ FSM::stateId <Step2R_2  >(), Event::Type::PRE_UPDATE },

				{ FSM::stateId <Apex      >(), Event::Type::UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2L_2  >(), Event::Type::UPDATE },

				{ FSM::regionId<Step2L_P  >(), Event::Type::TASK_FAILURE,	  FSM::stateId<Step2L_2 >() },

				{ FSM::stateId <Step2R_P  >(), Event::Type::UPDATE },
				{ FSM::stateId <Step2R_2  >(), Event::Type::UPDATE },

				{ FSM::regionId<Step2R_P  >(), Event::Type::TASK_FAILURE,	  FSM::stateId<Step2R_2 >() },

				{ FSM::stateId <Step2L_2  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2L_P  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2R_2  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Step2R_P  >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Hybrid    >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Planned   >(), Event::Type::POST_UPDATE },
				{ FSM::stateId <Apex      >(), Event::Type::POST_UPDATE },

				{ FSM::stateId <Hybrid    >(), Event::Type::PLAN_FAILED },
				{ FSM::regionId<Hybrid    >(), Event::Type::TASK_SUCCESS,	  FSM::stateId<Hybrid   >() },
				{ FSM::regionId<Hybrid    >(), Event::Type::PLAN_SUCCESS },

				{ FSM::stateId <Planned   >(), Event::Type::CHANGE,			  FSM::stateId<Terminal >() },

				{ FSM::stateId <Hybrid    >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2L_P  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2L_2  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2R_P  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Step2R_2  >(), Event::Type::EXIT_GUARD  },
				{ FSM::stateId <Terminal  >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Terminal_L>(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId <Terminal_R>(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId <Step2L_2  >(), Event::Type::EXIT },
				{ FSM::stateId <Step2L_P  >(), Event::Type::EXIT },
				{ FSM::stateId <Step2R_2  >(), Event::Type::EXIT },
				{ FSM::stateId <Step2R_P  >(), Event::Type::EXIT },
				{ FSM::stateId <Hybrid    >(), Event::Type::EXIT },

				{ FSM::stateId <Terminal  >(), Event::Type::ENTER },
				{ FSM::stateId <Terminal_L>(), Event::Type::ENTER },
				{ FSM::stateId <Terminal_R>(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex      >(),
				FSM::stateId<Planned   >(),
				FSM::stateId<Terminal  >(),
				FSM::stateId<Terminal_L>(),
				FSM::stateId<Terminal_R>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1_3   >(),
				FSM::stateId<Hybrid    >(),
				FSM::stateId<Step2L_P  >(),
				FSM::stateId<Step2L_2  >(),
				FSM::stateId<Step2R_P  >(),
				FSM::stateId<Step2R_2  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId <Terminal_L>(), Event::Type::EXIT },
		{ FSM::stateId <Terminal_R>(), Event::Type::EXIT },
		{ FSM::stateId <Terminal  >(), Event::Type::EXIT },
		{ FSM::stateId <Planned   >(), Event::Type::EXIT },
		{ FSM::stateId <Apex      >(), Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
