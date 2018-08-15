#include "test_shared.hpp"

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(Planned),
					M::Composite<S(Step1_BT),
						S(Step1_1),
						S(Step1_2),
						S(Step1_3)
					>,
					M::Orthogonal<S(Forked2),
						M::Composite<S(Step2L_Planner),
							S(Step2L_1),
							S(Step2L_2)
						>,
						M::Composite<S(Step2R_Planner),
							S(Step2R_1),
							S(Step2R_2)
						>
					>
				>,
				M::Composite<S(Unplanned),
					S(Work_1),
					S(Work_2)
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<Planned>()		 ==  1, "");
static_assert(FSM::stateId<Step1_BT>()		 ==  2, "");
static_assert(FSM::stateId<Step1_1>()		 ==  3, "");
static_assert(FSM::stateId<Step1_2>()		 ==  4, "");
static_assert(FSM::stateId<Step1_3>()		 ==  5, "");
static_assert(FSM::stateId<Forked2>()		 ==  6, "");
static_assert(FSM::stateId<Step2L_Planner>() ==  7, "");
static_assert(FSM::stateId<Step2L_1>()		 ==  8, "");
static_assert(FSM::stateId<Step2L_2>()		 ==  9, "");
static_assert(FSM::stateId<Step2R_Planner>() == 10, "");
static_assert(FSM::stateId<Step2R_1>()		 == 11, "");
static_assert(FSM::stateId<Step2R_2>()		 == 12, "");
static_assert(FSM::stateId<Unplanned>()		 == 13, "");
static_assert(FSM::stateId<Work_1>()		 == 14, "");
static_assert(FSM::stateId<Work_2>()		 == 15, "");

////////////////////////////////////////////////////////////////////////////////

struct Planned
	: FSM::State
{
	void guard(FullControl& control) {
		control._();
	}

	void enter(PlanControl& control) {
		auto plan = control.plan();

		plan.clear();
		plan.add<Step1_BT, Forked2>();
	}

	void update(FullControl& control) {
		control._();
	}

	void exit(PlanControl& control) {
		control._();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_BT
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan<Step1_BT>();

		plan.clear();

		plan.add<Step1_1, Step1_2>();
		plan.add<Step1_2, Step1_3>();
	}
};

struct Step1_1
	: FSM::State
{};

struct Step1_2
	: FSM::State
{};

struct Step1_3
	: FSM::State
{};

//------------------------------------------------------------------------------

struct Forked2
	: FSM::State
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2L_Planner
	: FSM::State
{};

struct Step2L_1
	: FSM::State
{};

struct Step2L_2
	: FSM::State
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2R_Planner
	: FSM::State
{};

struct Step2R_1
	: FSM::State
{};

struct Step2R_2
	: FSM::State
{};

//------------------------------------------------------------------------------

struct Unplanned
	: FSM::State
{};

struct Work_1
	: FSM::State
{};

struct Work_2
	: FSM::State
{};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH		 ==  2, "");
static_assert(FSM::Instance::STATE_COUNT	 == 16, "");
static_assert(FSM::Instance::FORK_COUNT		 ==  7, "");
static_assert(FSM::Instance::PRONG_COUNT	 == 13, "");

//------------------------------------------------------------------------------

TEST_CASE("Planner test", "[machine]") {
	Context _;
	FSM::Instance machine(_);

	WHEN("FSM is activated initially") {

		THEN("state data is not set on any states") {
			//REQUIRE(!machine.isStateDataSet<Top>());
		}

		AND_THEN("correctly read") {
			//REQUIRE(machine.getStateData<Origin, bool>() == nullptr);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
