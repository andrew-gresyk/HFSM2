#include "test_shared.hpp"

//------------------------------------------------------------------------------

using Context = float;
using M = hfsm::Machine<Context>;

using Action = bool;

//------------------------------------------------------------------------------

class Timed
	: public M::Bare
{
public:
	void preEnter(Context&)							{ _elapsed = 0.0f;			}
	void preUpdate(Context& _)						{ _elapsed += _;			}

	auto elapsed() const							{ return _elapsed;			}

private:
	float _elapsed;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public M::Bare
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
	: M::Base
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
	: M::Base
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
	: M::BaseT<Tracked>
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
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

struct A_2_2
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void react(const Action&, TransitionControl&)	{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1_1
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_1_2
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

//------------------------------------------------------------------------------

struct B_2
	: M::Base
{
	void enter(Control&)							{}
	void update(TransitionControl&)					{}
	void exit(Control&)								{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: M::Base
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
	: M::BaseT<Tracked>
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

using FSM = M::PeerRoot<
				M::Composite<A,
					A_1,
					M::Composite<A_2,
						A_2_1,
						A_2_2
					>
				>,
				M::Orthogonal<B,
					M::Composite<B_1,
						B_1_1,
						B_1_2
					>,
					M::Composite<B_2,
						B_2_1,
						B_2_2
					>
				>
			>;

static_assert(FSM::DEEP_WIDTH  ==  2, "");
static_assert(FSM::STATE_COUNT == 13, "");
static_assert(FSM::FORK_COUNT  ==  6, "");
static_assert(FSM::PRONG_COUNT == 10, "");

////////////////////////////////////////////////////////////////////////////////
