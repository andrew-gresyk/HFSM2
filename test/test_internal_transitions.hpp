#include "shared.hpp"

namespace test_internal_transitions {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::MachineT<hfsm2::Config::ContextT<float>>;

using Action = bool;

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

static_assert(FSM::regionId<A>()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B>()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A>()		==  1, "");
static_assert(FSM::stateId<A_1>()	==  2, "");
static_assert(FSM::stateId<A_2>()	==  3, "");
static_assert(FSM::stateId<A_2_1>()	==  4, "");
static_assert(FSM::stateId<A_2_2>()	==  5, "");
static_assert(FSM::stateId<B>()		==  6, "");
static_assert(FSM::stateId<B_1>()	==  7, "");
static_assert(FSM::stateId<B_1_1>()	==  8, "");
static_assert(FSM::stateId<B_1_2>()	==  9, "");
static_assert(FSM::stateId<B_2>()	== 10, "");
static_assert(FSM::stateId<B_2_1>()	== 11, "");
static_assert(FSM::stateId<B_2_2>()	== 12, "");

//------------------------------------------------------------------------------

class Timed
	: public FSM::Injection
{
public:
	void preEnter(float&)							{ _elapsed = 0.0f;			}
	void preUpdate(float& _)						{ _elapsed += _;			}

	float elapsed() const							{ return _elapsed;			}

private:
	float _elapsed = 0.0f;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public FSM::Injection
{
public:
	void preEnter(float&) {
		++_entryCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(float&) {
		++_currentUpdateCount;
		++_totalUpdateCount;
	}

	unsigned entryCount() const					{ return _entryCount;			}
	unsigned currentUpdateCount() const			{ return _currentUpdateCount;	}
	unsigned totalUpdateCount() const			{ return _totalUpdateCount;		}

private:
	unsigned _entryCount = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{
	//void entryGuard(GuardControl&)											{}
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	//void react(const Action&, FullControl&)									{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct A_1
	: FSM::State
{
	void enter(PlanControl&)													{}

	void update(FullControl& control) {
		control.changeTo<A_2>();
	}

	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct A_2
	: FSM::StateT<Tracked>
{
	void enter(PlanControl&)													{}

	void update(FullControl& control) {
		switch (entryCount()) {
		case 1:
			control.changeTo<B_2_2>();
			break;

		case 2:
			control.resume<B>();
			break;
		}
	}

	void exit(PlanControl&)														{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(PlanControl&)														{}
};

struct A_2_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1_1
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.cancelPendingTransitions();
		control.resume<B_2_2>();
	}

	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void exit(PlanControl&)														{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: FSM::StateT<Tracked>
{
	void entryGuard(GuardControl& control) {
		if (entryCount() == 2) {
			control.cancelPendingTransitions();
			control.resume<A>();
		}
	}

	void enter(PlanControl&)													{}

	void update(FullControl& control) {
		switch (totalUpdateCount()) {
		case 1:
			control.resume<A>();
			break;

		case 2:
			control.changeTo<B>();
			break;

		case 3:
			control.schedule<A_2_2>();
			control.resume<A>();
			break;
		}
	}

	void exit(PlanControl&)														{}
};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}
