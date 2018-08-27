#include "test_shared.hpp"

////////////////////////////////////////////////////////////////////////////////

using Context = float;
using M = hfsm::Machine<Context>;

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

// PeerRoot is 0
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
	//void guard(TransitionControl&)				{}
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	//void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct A_2;

struct A_1
	: FSM::State
{
	void enter(Control&)							{}

	void update(TransitionControl& control) {
		control.changeTo<A_2>();
	}

	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B;
struct B_2_2;

struct A_2
	: FSM::BaseT<Tracked>
{
	void enter(Control&)							{}

	void update(TransitionControl& control) {
		switch (entryCount()) {
		case 1:
			control.changeTo<B_2_2>();
			break;

		case 2:
			control.resume<B>();
			break;
		}
	}

	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

struct A_2_2
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1_1
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1_2
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_2
	: FSM::State
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: FSM::State
{
	void guard(TransitionControl& control) {
		control.resume<B_2_2>();
	}

	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: FSM::BaseT<Tracked>
{
	void guard(TransitionControl& control) {
		if (entryCount() == 2)
			control.resume<A>();
	}

	void enter(Control&)							{}

	void update(TransitionControl& control) {
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

	void exit(Control&)								{}
};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::DEEP_WIDTH	 ==  2, "");
static_assert(FSM::Instance::STATE_COUNT == 13, "");
static_assert(FSM::Instance::COMPO_COUNT ==  5, "");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "");
static_assert(FSM::Instance::PRONG_COUNT == 10, "");

////////////////////////////////////////////////////////////////////////////////
