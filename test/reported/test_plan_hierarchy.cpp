// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// https://github.com/andrew-gresyk/HFSM2/commit/156af7c122c756ff2eb081f76c1bd3ba3dc142ad
// https://discord.com/channels/755015945269018695/759099438353350696/1336307420112224337
// "FYI - I found an issue in the last commit with returning to a region with an active plan, the failure or success status of the tasks isn't reset correctly, fix incoming"

#define HFSM2_ENABLE_PLANS
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "../tools.hpp"

using namespace test_tools;

namespace test_plan_hierarchy {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

struct TopFail	  {};
struct TopSucceed {};

struct BotFail	  {};
struct BotSucceed {};

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Composite<S(Top_1),
					S(Bot1_1),
					S(Bot1_2)
				>,
				M::Orthogonal<S(Fork),
					M::Composite<S(Top_2),
						S(Bot2_1),
						S(Bot2_2)
					>,
					M::Composite<S(Top_3),
						S(Bot3_1),
						S(Bot3_2)
					>
				>,
				S(Top_4)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex  >() ==  0, "");
static_assert(FSM::regionId<Top_1 >() ==  1, "");
static_assert(FSM::regionId<Fork  >() ==  2, "");
static_assert(FSM::regionId<Top_2 >() ==  3, "");
static_assert(FSM::regionId<Top_3 >() ==  4, "");

static_assert(FSM:: stateId<Apex  >() ==  0, "");
static_assert(FSM:: stateId<Top_1 >() ==  1, "");
static_assert(FSM:: stateId<Bot1_1>() ==  2, "");
static_assert(FSM:: stateId<Bot1_2>() ==  3, "");
static_assert(FSM:: stateId<Fork  >() ==  4, "");
static_assert(FSM:: stateId<Top_2 >() ==  5, "");
static_assert(FSM:: stateId<Bot2_1>() ==  6, "");
static_assert(FSM:: stateId<Bot2_2>() ==  7, "");
static_assert(FSM:: stateId<Top_3 >() ==  8, "");
static_assert(FSM:: stateId<Bot3_1>() ==  9, "");
static_assert(FSM:: stateId<Bot3_2>() == 10, "");
static_assert(FSM:: stateId<Top_4 >() == 11, "");

////////////////////////////////////////////////////////////////////////////////

struct TopReacting
	: FSM::State
{
	using FSM::State::react;

	void react(const TopFail&, EventControl& control) {
		control.fail();
	}

	void react(const TopSucceed&, EventControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct BotReacting
	: FSM::State
{
	using FSM::State::react;

	void react(const BotFail&, EventControl& control) {
		control.fail();
	}

	void react(const BotSucceed&, EventControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Apex   : FSM::State {};
struct Top_1  : FSM::StateT<TopReacting> {};
struct Bot1_1 : FSM::StateT<BotReacting> {};
struct Bot1_2 : FSM::StateT<BotReacting> {};
struct Fork   : FSM::State {};
struct Top_2  : FSM::StateT<TopReacting> {};
struct Bot2_1 : FSM::StateT<BotReacting> {};
struct Bot2_2 : FSM::StateT<BotReacting> {};
struct Top_3  : FSM::StateT<TopReacting> {};
struct Bot3_1 : FSM::StateT<BotReacting> {};
struct Bot3_2 : FSM::StateT<BotReacting> {};
struct Top_4  : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT  == 12, "");
static_assert(FSM::Instance::Info::REGION_COUNT ==  5, "");
static_assert(FSM::Instance::Info::COMPO_COUNT  ==  4, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS ==  9, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT  ==  1, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS  ==  1, "");

////////////////////////////////////////////////////////////////////////////////

void setup(FSM::Instance& machine) {
	auto plan = machine.plan<Apex>();
	plan.change<Top_1, Fork >();
	plan.change<Fork , Top_4>();

	machine.plan<Top_1>().change<Bot1_1, Bot1_2>();
	machine.plan<Top_2>().change<Bot2_1, Bot2_2>();
	machine.plan<Top_3>().change<Bot3_1, Bot3_2>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step0(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Top_1 >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Bot1_1>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Apex  >(), Event::Type::ENTER },
		{ FSM::stateId<Top_1 >(), Event::Type::ENTER },
		{ FSM::stateId<Bot1_1>(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_1>(),
	});

	assertResumable(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step1(FSM::Instance& machine, Logger& logger) {
	machine.react(BotFail{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot1_1>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::REACT },
		{ FSM::stateId<Bot1_1>(), Event::Type::REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::TASK_FAILURE, FSM::stateId<Bot1_1>() },

		{ FSM::stateId<Bot1_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Top_1 >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Top_1 >(), Event::Type::PLAN_FAILED },
		{ FSM::stateId<Top_1 >(), Event::Type::TASK_FAILURE, FSM::stateId<Top_1 >() },

		{ FSM::stateId<Apex  >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Apex  >(), Event::Type::PLAN_FAILED },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_1>(),
	});

	assertResumable(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.react(BotSucceed{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot1_1>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::REACT },
		{ FSM::stateId<Bot1_1>(), Event::Type::REACT },

		{ FSM::stateId<Top_1 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Bot1_1>() },

		{ FSM::stateId<Bot1_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Top_1 >(), Event::Type::CHANGE, FSM::stateId<Bot1_2>() },

		{ FSM::stateId<Bot1_1>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Bot1_2>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Bot1_1>(), Event::Type::EXIT },
		{ FSM::stateId<Bot1_2>(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<Bot1_1>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.react(TopSucceed{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot1_2>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::REACT },

		{ FSM::stateId<Top_1 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Top_1 >() },

		{ FSM::stateId<Bot1_2>(), Event::Type::REACT },

		{ FSM::stateId<Bot1_2>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_1 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::CHANGE, FSM::stateId<Fork  >() },

		{ FSM::stateId<Bot1_2>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Top_1 >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Fork  >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Top_2 >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Bot2_1>(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Top_3 >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Bot3_1>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Bot1_2>(), Event::Type::EXIT },
		{ FSM::stateId<Top_1 >(), Event::Type::EXIT },
		{ FSM::stateId<Fork  >(), Event::Type::ENTER },
		{ FSM::stateId<Top_2 >(), Event::Type::ENTER },
		{ FSM::stateId<Bot2_1>(), Event::Type::ENTER },
		{ FSM::stateId<Top_3 >(), Event::Type::ENTER },
		{ FSM::stateId<Bot3_1>(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Fork  >(),
		FSM::stateId<Top_2 >(),
		FSM::stateId<Bot2_1>(),
		FSM::stateId<Top_3 >(),
		FSM::stateId<Bot3_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_2>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.react(BotFail{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot2_1>(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Fork  >(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::REACT },
		{ FSM::stateId<Bot2_1>(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::TASK_FAILURE, FSM::stateId<Bot2_1>() },

		{ FSM::stateId<Top_3 >(), Event::Type::REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::TASK_FAILURE, FSM::stateId<Bot3_1>() },

		{ FSM::stateId<Bot2_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Top_2 >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Top_2 >(), Event::Type::PLAN_FAILED },
		{ FSM::stateId<Top_2 >(), Event::Type::TASK_FAILURE, FSM::stateId<Top_2 >() },

		{ FSM::stateId<Top_3 >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Top_3 >(), Event::Type::PLAN_FAILED },
		{ FSM::stateId<Top_3 >(), Event::Type::TASK_FAILURE, FSM::stateId<Top_3 >() },

		{ FSM::stateId<Apex  >(), Event::Type::PLAN_FAILURE },
		{ FSM::stateId<Apex  >(), Event::Type::PLAN_FAILED },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Fork  >(),
		FSM::stateId<Top_2 >(),
		FSM::stateId<Bot2_1>(),
		FSM::stateId<Top_3 >(),
		FSM::stateId<Bot3_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_2>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.react(BotSucceed{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot2_1>(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Fork  >(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::REACT },
		{ FSM::stateId<Bot2_1>(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Bot2_1>() },

		{ FSM::stateId<Top_3 >(), Event::Type::REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Bot3_1>() },

		{ FSM::stateId<Bot2_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Bot3_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Top_2 >(), Event::Type::CHANGE, FSM::stateId<Bot2_2>() },
		{ FSM::stateId<Top_3 >(), Event::Type::CHANGE, FSM::stateId<Bot3_2>() },

		{ FSM::stateId<Bot2_1>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Bot3_1>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Bot2_2>(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Bot3_2>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<Bot2_1>(), Event::Type::EXIT },
		{ FSM::stateId<Bot2_2>(), Event::Type::ENTER },
		{ FSM::stateId<Bot3_1>(), Event::Type::EXIT },
		{ FSM::stateId<Bot3_2>(), Event::Type::ENTER },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Fork  >(),
		FSM::stateId<Top_2 >(),
		FSM::stateId<Bot2_2>(),
		FSM::stateId<Top_3 >(),
		FSM::stateId<Bot3_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_2>(),
		FSM::stateId<Bot2_1>(),
		FSM::stateId<Bot3_1>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step6(FSM::Instance& machine, Logger& logger) {
	machine.react(BotSucceed{});

	logger.assertSequence({
		{ FSM::stateId<Apex  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot2_2>(), Event::Type::PRE_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::PRE_REACT },
		{ FSM::stateId<Bot3_2>(), Event::Type::PRE_REACT },

		{ FSM::stateId<Apex  >(), Event::Type::REACT },
		{ FSM::stateId<Fork  >(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::REACT },
		{ FSM::stateId<Bot2_2>(), Event::Type::REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Bot2_2>() },

		{ FSM::stateId<Top_3 >(), Event::Type::REACT },
		{ FSM::stateId<Bot3_2>(), Event::Type::REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Bot3_2>() },

		{ FSM::stateId<Bot2_2>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_2 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Bot3_2>(), Event::Type::POST_REACT },
		{ FSM::stateId<Top_3 >(), Event::Type::POST_REACT },
		{ FSM::stateId<Fork  >(), Event::Type::POST_REACT },
		{ FSM::stateId<Apex  >(), Event::Type::POST_REACT },

		{ FSM::stateId<Top_2 >(), Event::Type::PLAN_SUCCESS },
		{ FSM::stateId<Top_2 >(), Event::Type::PLAN_SUCCEEDED },
		{ FSM::stateId<Top_2 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Top_2 >() },

		{ FSM::stateId<Top_3 >(), Event::Type::PLAN_SUCCESS },
		{ FSM::stateId<Top_3 >(), Event::Type::PLAN_SUCCEEDED },
		{ FSM::stateId<Top_3 >(), Event::Type::TASK_SUCCESS, FSM::stateId<Top_3 >() },
	});

	assertActive(machine, {
		FSM::stateId<Apex  >(),
		FSM::stateId<Fork  >(),
		FSM::stateId<Top_2 >(),
		FSM::stateId<Bot2_2>(),
		FSM::stateId<Top_3 >(),
		FSM::stateId<Bot3_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<Top_1 >(),
		FSM::stateId<Bot1_2>(),
		FSM::stateId<Bot2_1>(),
		FSM::stateId<Bot3_1>(),
	});
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Plan Hierarchy") {
	Logger logger;

	{
		FSM::Instance machine{&logger};

		setup(machine);
		step0(machine, logger);
		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		step5(machine, logger);
		step6(machine, logger);
	}
}

////////////////////////////////////////////////////////////////////////////////

}
