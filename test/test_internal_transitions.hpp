#include "test_shared.hpp"

namespace test_internal_transitions {

////////////////////////////////////////////////////////////////////////////////

using Context = float;
using M = hfsm2::Machine<Context>;

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
	: public FSM::Bare
{
public:
	void preEnter(Context&)							{ _elapsed = 0.0f;			}
	void preUpdate(Context& _)						{ _elapsed += _;			}

	float elapsed() const							{ return _elapsed;			}

private:
	float _elapsed;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public FSM::Bare
{
public:
	void preEnter(Context&) {
		++_entryCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(Context&) {
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
	void enter(Control&)														{}
	void update(FullControl&)													{}
	//void react(const Action&, FullControl&)									{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct A_1
	: FSM::State
{
	void enter(Control&)														{}

	void update(FullControl& control) {
		control.changeTo<A_2>();
	}

	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct A_2
	: FSM::BaseT<Tracked>
{
	void enter(Control&)														{}

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

	void exit(Control&)															{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(Control&)															{}
};

struct A_2_2
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void react(const Action&, FullControl&)										{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct B_1
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct B_1_1
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct B_1_2
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void exit(Control&)															{}
};

//------------------------------------------------------------------------------

struct B_2
	: FSM::State
{
	void enter(Control&)														{}
	void update(FullControl&)													{}
	void exit(Control&)															{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.cancelPendingChanges();
		control.resume<B_2_2>();
	}

	void enter(Control&)														{}
	void update(FullControl&)													{}
	void exit(Control&)															{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: FSM::BaseT<Tracked>
{
	void entryGuard(GuardControl& control) {
		if (entryCount() == 2) {
			control.cancelPendingChanges();
			control.resume<A>();
		}
	}

	void enter(Control&)														{}

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

	void exit(Control&)															{}
};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH	 ==  2, "DEEP_WIDTH");
static_assert(FSM::Instance::STATE_COUNT == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  5, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 10, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
