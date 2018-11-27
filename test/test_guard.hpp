#define HFSM_VERBOSE_DEBUG_LOG
#include "test_shared.hpp"

namespace test_guard {

////////////////////////////////////////////////////////////////////////////////

using Context = float;
using M = hfsm2::Machine<Context>;

using Action = bool;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Orthogonal<S(Step1),
					S(Step1_1),
					S(Step1_2)
				>,
				M::Orthogonal<S(Step2),
					S(Step2_1),
					S(Step2_2)
				>,
				S(Step3)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>()		==  0, "");
static_assert(FSM::regionId<Step1>()	==  1, "");
static_assert(FSM::regionId<Step2>()	==  2, "");

static_assert(FSM::stateId<Apex>()		==  0, "");
static_assert(FSM::stateId<Step1>()		==  1, "");
static_assert(FSM::stateId<Step1_1>()	==  2, "");
static_assert(FSM::stateId<Step1_2>()	==  3, "");
static_assert(FSM::stateId<Step2>()		==  4, "");
static_assert(FSM::stateId<Step2_1>()	==  5, "");
static_assert(FSM::stateId<Step2_2>()	==  6, "");
static_assert(FSM::stateId<Step3>()		==  7, "");

////////////////////////////////////////////////////////////////////////////////

class EntryGuardTracked
	: public FSM::Bare
{
public:
	void preEntryGuard(Context&)					{ ++_entryGuardCount;		}

	auto entryGuardCount() const					{ return _entryGuardCount;	}

private:
	unsigned _entryGuardCount = 0;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class ExitGuardTracked
	: public FSM::Bare
{
public:
	void preExitGuard(Context&)						{ ++_exitGuardCount;		}

	auto exitGuardCount() const						{ return _exitGuardCount;	}

private:
	unsigned _exitGuardCount = 0;
};

//------------------------------------------------------------------------------

class UpdateTracked
	: public FSM::Bare
{
public:
	void preUpdate(Context&)						{ ++_updateCount;			}

	auto updateCount() const						{ return _updateCount;		}

private:
	unsigned _updateCount = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Apex : FSM::State {};

//------------------------------------------------------------------------------

struct Step1
	: FSM::BaseT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 1)
			control.cancelPendingChanges();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_1
	: FSM::BaseT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 1)
			control.cancelPendingChanges();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_2
	: FSM::BaseT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 2)
			control.cancelPendingChanges();
	}
};

//------------------------------------------------------------------------------

struct Step2
	: FSM::BaseT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 1)
			control.cancelPendingChanges();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_1
	: FSM::BaseT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 1)
			control.cancelPendingChanges();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_2
	: FSM::BaseT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 2)
			control.cancelPendingChanges();
	}
};

//------------------------------------------------------------------------------

struct Step3 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH	 == 1, "DEEP_WIDTH");
static_assert(FSM::Instance::STATE_COUNT == 8, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT == 1, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT == 2, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS == 2, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 3, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
