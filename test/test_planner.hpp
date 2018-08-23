#define HFSM_FORCE_DEBUG_LOG
#include "test_shared.hpp"

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

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
					S(Terminal)
				>,
				M::Composite<S(Unplanned),
					S(Work_1),
					S(Work_2)
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<Apex>()		==  0, "");
static_assert(FSM::stateId<Planned>()	==  1, "");
static_assert(FSM::stateId<Step1_BT>()	==  2, "");
static_assert(FSM::stateId<Step1_1>()	==  3, "");
static_assert(FSM::stateId<Step1_2>()	==  4, "");
static_assert(FSM::stateId<Step1_3>()	==  5, "");
static_assert(FSM::stateId<Hybrid>()	==  6, "");
static_assert(FSM::stateId<Step2L_P>()	==  7, "");
static_assert(FSM::stateId<Step2L_1>()	==  8, "");
static_assert(FSM::stateId<Step2L_2>()	==  9, "");
static_assert(FSM::stateId<Step2R_P>()	== 10, "");
static_assert(FSM::stateId<Step2R_1>()	== 11, "");
static_assert(FSM::stateId<Step2R_2>()	== 12, "");
static_assert(FSM::stateId<Terminal>()	== 13, "");
static_assert(FSM::stateId<Unplanned>()	== 14, "");
static_assert(FSM::stateId<Work_1>()	== 15, "");
static_assert(FSM::stateId<Work_2>()	== 16, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex
	: FSM::State
{};

//------------------------------------------------------------------------------

struct Planned
	: FSM::State
{
	void guard(FullControl& control) {
		control._();
	}

	void enter(PlanControl& control) {
		auto plan = control.plan();
		REQUIRE(!plan);

		plan.add<Step1_BT, Hybrid>();
		plan.add<Hybrid, Terminal>();
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
		Plan plan = control.plan();
		REQUIRE(!plan);

		plan.add<Step1_1, Step1_2>();
		plan.add<Step1_2, Step1_3>();
	}
};

struct Step1_1
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
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
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2L_P
	: FSM::State
{};

struct Step2L_1
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Step2L_2>();
	}
};

struct Step2L_2
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2R_P
	: FSM::State
{};

struct Step2R_1
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Step2R_2>();
	}
};

struct Step2R_2
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Terminal
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

static_assert(FSM::Instance::DEEP_WIDTH		  ==  2, "");
static_assert(FSM::Instance::STATE_COUNT	  == 17, "");
static_assert(FSM::Instance::COMPOSITE_COUNT  ==  6, "");
static_assert(FSM::Instance::ORTHOGONAL_COUNT ==  1, "");
static_assert(FSM::Instance::FORK_COUNT		  ==  7, "");
static_assert(FSM::Instance::PRONG_COUNT	  == 14, "");
static_assert(FSM::Instance::WIDTH			  ==  2, "");

////////////////////////////////////////////////////////////////////////////////
