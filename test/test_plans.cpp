// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_PLANS
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

using namespace test_tools;

namespace test_plans {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

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
					M::Orthogonal<S(Fork),
						M::Composite<S(Step3L_P),
							S(Step3L_1),
							S(Step3L_2)
						>,
						M::Composite<S(Step3R_P),
							S(Step3R_1),
							S(Step3R_2)
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
static_assert(FSM::regionId<Fork     >() ==  6, "");
static_assert(FSM::regionId<Step3L_P >() ==  7, "");
static_assert(FSM::regionId<Step3R_P >() ==  8, "");
static_assert(FSM::regionId<Terminal >() ==  9, "");
static_assert(FSM::regionId<Unplanned>() == 10, "");

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
static_assert(FSM::stateId<Fork      >() == 13, "");
static_assert(FSM::stateId<Step3L_P  >() == 14, "");
static_assert(FSM::stateId<Step3L_1  >() == 15, "");
static_assert(FSM::stateId<Step3L_2  >() == 16, "");
static_assert(FSM::stateId<Step3R_P  >() == 17, "");
static_assert(FSM::stateId<Step3R_1  >() == 18, "");
static_assert(FSM::stateId<Step3R_2  >() == 19, "");
static_assert(FSM::stateId<Terminal  >() == 20, "");
static_assert(FSM::stateId<Terminal_L>() == 21, "");
static_assert(FSM::stateId<Terminal_R>() == 22, "");
static_assert(FSM::stateId<Unplanned >() == 23, "");
static_assert(FSM::stateId<Work_1    >() == 24, "");
static_assert(FSM::stateId<Work_2    >() == 25, "");

////////////////////////////////////////////////////////////////////////////////
// TODO: make use of planFailed()
// TODO: build a plan with re-entry

struct Apex	  : FSM::State {};

//------------------------------------------------------------------------------

struct Planned
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan<Planned>();
		REQUIRE(!plan);

		REQUIRE(plan.change<Step1_BT, Hybrid>());
		REQUIRE(plan.restart<Hybrid, Step3L_2>());
		REQUIRE(plan.restart<Hybrid, Step3R_2>());
		REQUIRE(plan.change<Fork, Terminal>());
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

		REQUIRE(plan.change <Step1_2, Step1_2>());
		REQUIRE(plan.restart<Step1_2, Step1_3>());
	}
};

struct Step1_1
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Step1_2  >();
	}
};

struct Step1_2
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step1_3
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Hybrid
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan();
		REQUIRE(!plan);

		REQUIRE(plan.utilize<Step2L_1, Step2L_2>());
		REQUIRE(plan.randomize<Step2R_1, Step2R_2>());
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

	void update(FullControl& control) {
		control.fail();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2R_P : FSM::State {};

struct Step2R_1
	: FSM::State
{
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

	void update(FullControl& control) {
		control.fail();
	}
};

//------------------------------------------------------------------------------

struct Fork
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step3L_P : FSM::State {};

struct Step3L_1
	: FSM::State
{};

struct Step3L_2
	: FSM::State
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step3R_P : FSM::State {};

struct Step3R_1
	: FSM::State
{};

struct Step3R_2
	: FSM::State
{};

//------------------------------------------------------------------------------

struct Terminal
	: FSM::State
{
	void planSucceeded(FullControl& control) {
		control.fail();
	}
};

struct Terminal_L
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Terminal_R
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Unplanned  : FSM::State {};
struct Work_1	  : FSM::State {};
struct Work_2	  : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT  == 26, "");
static_assert(FSM::Instance::Info::REGION_COUNT == 11, "");
static_assert(FSM::Instance::Info::COMPO_COUNT	==  8, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS == 19, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT	==  3, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS  ==  3, "");

////////////////////////////////////////////////////////////////////////////////

void step0(FSM::Instance& machine, Logger& logger) {
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

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_1   >(),
	});

	assertResumable(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step1(FSM::Instance& machine, Logger& logger) {
	machine.update();

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

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_2   >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_1   >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_2   >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_2   >(), Event::Type::UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step1_2  >() },

		{ FSM::stateId<Step1_2   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::CHANGE,			FSM::stateId<Step1_2  >() },

		{ FSM::stateId<Step1_2   >(), Event::Type::EXIT_GUARD  },
		{ FSM::stateId<Step1_2   >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Step1_2   >(), Event::Type::REENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_2   >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_1   >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_2   >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_2   >(), Event::Type::UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step1_2  >() },

		{ FSM::stateId<Step1_2   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::CHANGE,			FSM::stateId<Step1_3  >() },

		{ FSM::stateId<Step1_2   >(), Event::Type::EXIT_GUARD  },
		{ FSM::stateId<Step1_3   >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Step1_2   >(), Event::Type::EXIT },

		{ FSM::stateId<Step1_3   >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_3   >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_2   >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step1_3   >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step1_3   >(), Event::Type::UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::TASK_SUCCESS,		FSM::stateId<Step1_3   >() },

		{ FSM::stateId<Step1_3   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step1_BT  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Step1_BT  >(), Event::Type::PLAN_SUCCESS },
		{ FSM::stateId<Step1_BT  >(), Event::Type::PLAN_SUCCEEDED },
		{ FSM::stateId<Step1_BT  >(), Event::Type::TASK_SUCCESS,		FSM::stateId<Step1_BT  >() },
		{ FSM::stateId<Planned   >(), Event::Type::CHANGE,				FSM::stateId<Hybrid    >() },

		{ FSM::stateId<Step1_3   >(), Event::Type::EXIT_GUARD  },
		{ FSM::stateId<Step1_BT  >(), Event::Type::EXIT_GUARD  },

		{ FSM::stateId<Hybrid    >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Step2L_P  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Step2L_1  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Step2R_P  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Step2R_1  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Step1_3   >(), Event::Type::EXIT   },
		{ FSM::stateId<Step1_BT  >(), Event::Type::EXIT   },

		{ FSM::stateId<Hybrid    >(), Event::Type::ENTER  },
		{ FSM::stateId<Step2L_P  >(), Event::Type::ENTER  },
		{ FSM::stateId<Step2L_1  >(), Event::Type::ENTER  },
		{ FSM::stateId<Step2R_P  >(), Event::Type::ENTER  },
		{ FSM::stateId<Step2R_1  >(), Event::Type::ENTER  },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Hybrid    >(),
		FSM::stateId<Step2L_P  >(),
		FSM::stateId<Step2L_1  >(),
		FSM::stateId<Step2R_P  >(),
		FSM::stateId<Step2R_1  >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_3   >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2L_1  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2R_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2R_1  >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2L_1  >(), Event::Type::UPDATE },

		{ FSM::stateId<Step2L_P  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step2L_1  >() },

		{ FSM::stateId<Step2R_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2R_1  >(), Event::Type::UPDATE },

		{ FSM::stateId<Step2R_P  >(), Event::Type::TASK_SUCCESS,	FSM::stateId<Step2R_1  >() },

		{ FSM::stateId<Step2L_1  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2R_1  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2R_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Hybrid    >(), Event::Type::CHANGE,			FSM::stateId<Step2L_2  >() },
		{ FSM::stateId<Hybrid    >(), Event::Type::CHANGE,			FSM::stateId<Step2R_2  >() },

		{ FSM::stateId<Step2L_1  >(), Event::Type::EXIT_GUARD  },
		{ FSM::stateId<Step2R_1  >(), Event::Type::EXIT_GUARD  },
		{ FSM::stateId<Step2L_2  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Step2R_2  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Step2L_1  >(), Event::Type::EXIT  },
		{ FSM::stateId<Step2L_2  >(), Event::Type::ENTER },

		{ FSM::stateId<Step2R_1  >(), Event::Type::EXIT  },
		{ FSM::stateId<Step2R_2  >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Hybrid    >(),
		FSM::stateId<Step2L_P  >(),
		FSM::stateId<Step2L_2  >(),
		FSM::stateId<Step2R_P  >(),
		FSM::stateId<Step2R_2  >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_BT  >(),
		FSM::stateId<Step1_3   >(),
		FSM::stateId<Step2L_1  >(),
		FSM::stateId<Step2R_1  >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step6(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2L_2  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2R_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step2R_2  >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2L_2  >(), Event::Type::UPDATE },

		{ FSM::stateId<Step2L_P  >(), Event::Type::TASK_FAILURE,	  FSM::stateId<Step2L_2	>() },

		{ FSM::stateId<Step2R_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step2R_2  >(), Event::Type::UPDATE },

		{ FSM::stateId<Step2R_P  >(), Event::Type::TASK_FAILURE,	  FSM::stateId<Step2R_2	>() },

		{ FSM::stateId<Step2L_2  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2L_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2R_2  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step2R_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Hybrid    >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Hybrid    >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Hybrid    >(), Event::Type::PLAN_FAILED },
		{ FSM::stateId<Hybrid    >(), Event::Type::TASK_SUCCESS,	  FSM::stateId<Hybrid	>() },
		{ FSM::stateId<Planned   >(), Event::Type::CHANGE,			  FSM::stateId<Step3L_2	>() },
		{ FSM::stateId<Planned   >(), Event::Type::CHANGE,			  FSM::stateId<Step3R_2	>() },

		{ FSM::stateId<Step2L_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step2L_P  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step2R_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step2R_P  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Hybrid    >(), Event::Type::EXIT_GUARD },

		{ FSM::stateId<Fork      >(), Event::Type::ENTRY_GUARD	},
		{ FSM::stateId<Step3L_P  >(), Event::Type::ENTRY_GUARD	},
		{ FSM::stateId<Step3L_2  >(), Event::Type::ENTRY_GUARD	},
		{ FSM::stateId<Step3R_P  >(), Event::Type::ENTRY_GUARD	},
		{ FSM::stateId<Step3R_2  >(), Event::Type::ENTRY_GUARD	},

		{ FSM::stateId<Step2L_2  >(), Event::Type::EXIT },
		{ FSM::stateId<Step2L_P  >(), Event::Type::EXIT },
		{ FSM::stateId<Step2R_2  >(), Event::Type::EXIT },
		{ FSM::stateId<Step2R_P  >(), Event::Type::EXIT },
		{ FSM::stateId<Hybrid    >(), Event::Type::EXIT },

		{ FSM::stateId<Fork      >(), Event::Type::ENTER },
		{ FSM::stateId<Step3L_P  >(), Event::Type::ENTER },
		{ FSM::stateId<Step3L_2  >(), Event::Type::ENTER },
		{ FSM::stateId<Step3R_P  >(), Event::Type::ENTER },
		{ FSM::stateId<Step3R_2  >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Fork      >(),
		FSM::stateId<Step3L_P  >(),
		FSM::stateId<Step3L_2  >(),
		FSM::stateId<Step3R_P  >(),
		FSM::stateId<Step3R_2  >(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_3   >(),
		FSM::stateId<Hybrid    >(),
		FSM::stateId<Step2L_P  >(),
		FSM::stateId<Step2L_2  >(),
		FSM::stateId<Step2R_P  >(),
		FSM::stateId<Step2R_2  >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step7(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<Apex      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Fork      >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step3L_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step3L_2  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step3R_P  >(), Event::Type::PRE_UPDATE },
		{ FSM::stateId<Step3R_2  >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<Apex      >(), Event::Type::UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::UPDATE },
		{ FSM::stateId<Fork      >(), Event::Type::UPDATE },

		{ FSM::stateId<Fork      >(), Event::Type::TASK_SUCCESS,	  FSM::stateId<Fork      >() },

		{ FSM::stateId<Step3L_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step3L_2  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step3R_P  >(), Event::Type::UPDATE },
		{ FSM::stateId<Step3R_2  >(), Event::Type::UPDATE },

		{ FSM::stateId<Step3L_2  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step3L_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step3R_2  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Step3R_P  >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Fork      >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Planned   >(), Event::Type::POST_UPDATE },
		{ FSM::stateId<Apex      >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<Planned   >(), Event::Type::CHANGE,			  FSM::stateId<Terminal >() },

		{ FSM::stateId<Step3L_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step3L_P  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step3R_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Step3R_P  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Fork      >(), Event::Type::EXIT_GUARD },

		{ FSM::stateId<Terminal  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Terminal_L>(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Terminal_R>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Step3L_2  >(), Event::Type::EXIT },
		{ FSM::stateId<Step3L_P  >(), Event::Type::EXIT },
		{ FSM::stateId<Step3R_2  >(), Event::Type::EXIT },
		{ FSM::stateId<Step3R_P  >(), Event::Type::EXIT },
		{ FSM::stateId<Fork      >(), Event::Type::EXIT },

		{ FSM::stateId<Terminal  >(), Event::Type::ENTER },
		{ FSM::stateId<Terminal_L>(), Event::Type::ENTER },
		{ FSM::stateId<Terminal_R>(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex      >(),
		FSM::stateId<Planned   >(),
		FSM::stateId<Terminal  >(),
		FSM::stateId<Terminal_L>(),
		FSM::stateId<Terminal_R>(),
	});

	assertResumable(machine, {
		FSM::stateId<Step1_3   >(),
		FSM::stateId<Step2L_2  >(),
		FSM::stateId<Step2R_2  >(),
		FSM::stateId<Fork	   >(),
		FSM::stateId<Step3L_P  >(),
		FSM::stateId<Step3L_2  >(),
		FSM::stateId<Step3R_P  >(),
		FSM::stateId<Step3R_2  >(),
	});
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Plans") {
	Logger logger;

	{
		FSM::Instance machine{&logger};

		step0(machine, logger);
		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		step5(machine, logger);
		step6(machine, logger);
		step7(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<Terminal_L>(), Event::Type::EXIT },
		{ FSM::stateId<Terminal_R>(), Event::Type::EXIT },
		{ FSM::stateId<Terminal  >(), Event::Type::EXIT },
		{ FSM::stateId<Planned   >(), Event::Type::EXIT },
		{ FSM::stateId<Apex      >(), Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
