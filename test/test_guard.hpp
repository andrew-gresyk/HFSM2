#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_guard {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

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
	: public FSM::Injection
{
public:
	void preEntryGuard(Context&)					{ ++_entryGuardCount;		}

	unsigned entryGuardCount() const				{ return _entryGuardCount;	}

private:
	unsigned _entryGuardCount = 0;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class ExitGuardTracked
	: public FSM::Injection
{
public:
	void preExitGuard(Context&)						{ ++_exitGuardCount;		}

	unsigned exitGuardCount() const					{ return _exitGuardCount;	}

private:
	unsigned _exitGuardCount = 0;
};

//------------------------------------------------------------------------------

class UpdateTracked
	: public FSM::Injection
{
public:
	void preUpdate(Context&)						{ ++_updateCount;			}

	unsigned updateCount() const					{ return _updateCount;		}

private:
	unsigned _updateCount = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Apex : FSM::State {};

//------------------------------------------------------------------------------

struct Step1
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_1
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_2
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		if (exitGuardCount() == 2)
			control.cancelPendingTransitions();
	}
};

//------------------------------------------------------------------------------

struct Step2
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_1
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_2
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		if (entryGuardCount() == 2)
			control.cancelPendingTransitions();
	}
};

//------------------------------------------------------------------------------

struct Step3 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 8, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 1, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 3, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 2, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   == 2, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}
