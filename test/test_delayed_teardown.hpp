#define HFSM_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_delayed_teardown {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

using Action = bool;

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

static_assert(FSM::regionId<Apex>()		==  0, "");
static_assert(FSM::regionId<Step1>()	==  1, "");

static_assert(FSM::stateId<Apex>()		==  0, "");
static_assert(FSM::stateId<Step1>()		==  1, "");
static_assert(FSM::stateId<Step1_1>()	==  2, "");
static_assert(FSM::stateId<Step1_2>()	==  3, "");
static_assert(FSM::stateId<Setup>()		==  4, "");
static_assert(FSM::stateId<Work>()		==  5, "");
static_assert(FSM::stateId<Teardown>()	==  6, "");
static_assert(FSM::stateId<Step2>()		==  7, "");

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
			control.cancelPendingChanges();

			// TODO: query destination from the in-flight transition
			control.plan().add<Teardown, Step2>();

			control.changeTo<Teardown>();
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

static_assert(FSM::Instance::STATE_COUNT == 8, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT == 2, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT == 1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS == 1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 5, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
