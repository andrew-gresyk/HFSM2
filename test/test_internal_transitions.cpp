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

static_assert(FSM::regionId<A  >()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B  >()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A    >() ==  1, "");
static_assert(FSM::stateId<A_1  >() ==  2, "");
static_assert(FSM::stateId<A_2  >() ==  3, "");
static_assert(FSM::stateId<A_2_1>() ==  4, "");
static_assert(FSM::stateId<A_2_2>() ==  5, "");
static_assert(FSM::stateId<B    >() ==  6, "");
static_assert(FSM::stateId<B_1  >() ==  7, "");
static_assert(FSM::stateId<B_1_1>() ==  8, "");
static_assert(FSM::stateId<B_1_2>() ==  9, "");
static_assert(FSM::stateId<B_2  >() == 10, "");
static_assert(FSM::stateId<B_2_1>() == 11, "");
static_assert(FSM::stateId<B_2_2>() == 12, "");

//------------------------------------------------------------------------------

class Timed
	: public FSM::Injection
{
public:
	void preEnter(float&)							{ _elapsed = 0.0f;			}
	void preUpdate(float& _)						{ _elapsed += _;			} //-V669

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
	//void update(FullControl&)													{}
	//void react(const Action&, FullControl&)									{}
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
	//void enter(PlanControl&)													{}
	//void update(FullControl&)													{}
	//void exit(PlanControl&)													{}
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

const Types all = {
	FSM::stateId<A    >(),
	FSM::stateId<A_1  >(),
	FSM::stateId<A_2  >(),
	FSM::stateId<A_2_1>(),
	FSM::stateId<A_2_2>(),
	FSM::stateId<B    >(),
	FSM::stateId<B_1  >(),
	FSM::stateId<B_1_1>(),
	FSM::stateId<B_1_2>(),
	FSM::stateId<B_2  >(),
	FSM::stateId<B_2_1>(),
	FSM::stateId<B_2_2>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Internal Transition", "[machine]") {
	float _ = 0.0f;
	LoggerT<float> logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_, &logger};
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::CONSTRUCT },
				{ FSM::stateId<A_1  >(), Event::CONSTRUCT },

				{ FSM::stateId<A    >(), Event::ENTER },
				{ FSM::stateId<A_1  >(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},	 Event::REACT },
				{ FSM::stateId<A    >(), Event::REACT },
				{ FSM::stateId<A_1  >(), Event::REACT },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::UPDATE },
				{ FSM::stateId<A_1  >(), Event::UPDATE },

				{ FSM::stateId<A_1  >(), Event::CHANGE,			FSM::stateId<A_2  >() },

				{ FSM::stateId<A_2  >(), Event::ENTRY_GUARD },

				{ FSM::stateId<A_1  >(), Event::EXIT },

				{ FSM::stateId<A_1  >(), Event::DESTRUCT },
				{ FSM::stateId<A_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<A_2_1>(), Event::CONSTRUCT },

				{ FSM::stateId<A_2  >(), Event::ENTER },
				{ FSM::stateId<A_2_1>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A_1  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},	 Event::REACT },
				{ FSM::stateId<A    >(), Event::REACT },
				{ FSM::stateId<A_2  >(), Event::REACT },
				{ FSM::stateId<A_2_1>(), Event::REACT },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A_1  >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::UPDATE },
				{ FSM::stateId<A_2  >(), Event::UPDATE },

				{ FSM::stateId<A_2  >(), Event::CHANGE,			FSM::stateId<B_2_2>() },

				{ FSM::stateId<A_2_1>(), Event::UPDATE },

				{ FSM::stateId<A_2  >(), Event::EXIT_GUARD },
				{ FSM::stateId<B_2_2>(), Event::ENTRY_GUARD },

				{ FSM::stateId<A_2_1>(), Event::EXIT },
				{ FSM::stateId<A_2  >(), Event::EXIT },
				{ FSM::stateId<A    >(), Event::EXIT },

				{ FSM::stateId<A_2_1>(), Event::DESTRUCT },
				{ FSM::stateId<A_2  >(), Event::DESTRUCT },
				{ FSM::stateId<A    >(), Event::DESTRUCT },
				{ FSM::stateId<B    >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1_1>(), Event::CONSTRUCT },
				{ FSM::stateId<B_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_2_2>(), Event::CONSTRUCT },

				{ FSM::stateId<B    >(), Event::ENTER },
				{ FSM::stateId<B_1  >(), Event::ENTER },
				{ FSM::stateId<B_1_1>(), Event::ENTER },
				{ FSM::stateId<B_2  >(), Event::ENTER },
				{ FSM::stateId<B_2_2>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},	 Event::REACT },
				{ FSM::stateId<B    >(), Event::REACT },
				{ FSM::stateId<B_1  >(), Event::REACT },
				{ FSM::stateId<B_1_1>(), Event::REACT },
				{ FSM::stateId<B_2  >(), Event::REACT },
				{ FSM::stateId<B_2_2>(), Event::REACT },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<B    >(), Event::UPDATE },
				{ FSM::stateId<B_1  >(), Event::UPDATE },
				{ FSM::stateId<B_1_1>(), Event::UPDATE },
				{ FSM::stateId<B_2  >(), Event::UPDATE },
				{ FSM::stateId<B_2_2>(), Event::UPDATE },

				{ FSM::stateId<B_2_2>(), Event::RESUME,			FSM::stateId<A    >() },

				{ FSM::stateId<B_2_2>(), Event::EXIT_GUARD },
				{ FSM::stateId<A_2  >(), Event::ENTRY_GUARD },

				{ FSM::stateId<B_1_1>(), Event::EXIT },
				{ FSM::stateId<B_1  >(), Event::EXIT },
				{ FSM::stateId<B_2_2>(), Event::EXIT },
				{ FSM::stateId<B_2  >(), Event::EXIT },
				{ FSM::stateId<B    >(), Event::EXIT },

				{ FSM::stateId<B_1_1>(), Event::DESTRUCT },
				{ FSM::stateId<B_1  >(), Event::DESTRUCT },
				{ FSM::stateId<B_2_2>(), Event::DESTRUCT },
				{ FSM::stateId<B_2  >(), Event::DESTRUCT },
				{ FSM::stateId<B    >(), Event::DESTRUCT },
				{ FSM::stateId<A    >(), Event::CONSTRUCT },
				{ FSM::stateId<A_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<A_2_1>(), Event::CONSTRUCT },

				{ FSM::stateId<A    >(), Event::ENTER },
				{ FSM::stateId<A_2  >(), Event::ENTER },
				{ FSM::stateId<A_2_1>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::UPDATE },
				{ FSM::stateId<A_2  >(), Event::UPDATE },

				{ FSM::stateId<A_2  >(), Event::RESUME,			FSM::stateId<B    >() },

				{ FSM::stateId<A_2_1>(), Event::UPDATE },

				{ FSM::stateId<A_2  >(), Event::EXIT_GUARD },
				{ FSM::stateId<B_2_2>(), Event::ENTRY_GUARD },

				{ FSM::stateId<A_2_1>(), Event::EXIT },
				{ FSM::stateId<A_2  >(), Event::EXIT },
				{ FSM::stateId<A    >(), Event::EXIT },

				{ FSM::stateId<A_2_1>(), Event::DESTRUCT },
				{ FSM::stateId<A_2  >(), Event::DESTRUCT },
				{ FSM::stateId<A    >(), Event::DESTRUCT },
				{ FSM::stateId<B    >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1_1>(), Event::CONSTRUCT },
				{ FSM::stateId<B_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_2_2>(), Event::CONSTRUCT },

				{ FSM::stateId<B    >(), Event::ENTER },
				{ FSM::stateId<B_1  >(), Event::ENTER },
				{ FSM::stateId<B_1_1>(), Event::ENTER },
				{ FSM::stateId<B_2  >(), Event::ENTER },
				{ FSM::stateId<B_2_2>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<B    >(), Event::UPDATE },
				{ FSM::stateId<B_1  >(), Event::UPDATE },
				{ FSM::stateId<B_1_1>(), Event::UPDATE },
				{ FSM::stateId<B_2  >(), Event::UPDATE },
				{ FSM::stateId<B_2_2>(), Event::UPDATE },

				{ FSM::stateId<B_2_2>(), Event::CHANGE,			FSM::stateId<B    >() },

				{ FSM::stateId<B_2_2>(), Event::EXIT_GUARD },
				{ FSM::stateId<B_2_1>(), Event::ENTRY_GUARD },

				{ FSM::stateId<B_2_1>(), Event::CANCEL_PENDING },

				{ FSM::stateId<B_2_1>(), Event::RESUME,			FSM::stateId<B_2_2>() },

				{ FSM::stateId<B_2_2>(), Event::EXIT_GUARD },
				{ FSM::stateId<B_2_2>(), Event::ENTRY_GUARD },

				{ FSM::stateId<B_2_2>(), Event::REENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
			logger.assertSequence({
				{ FSM::stateId<B    >(), Event::UPDATE },
				{ FSM::stateId<B_1  >(), Event::UPDATE },
				{ FSM::stateId<B_1_1>(), Event::UPDATE },
				{ FSM::stateId<B_2  >(), Event::UPDATE },
				{ FSM::stateId<B_2_2>(), Event::UPDATE },

				{ FSM::stateId<B_2_2>(), Event::SCHEDULE,		FSM::stateId<A_2_2>() },
				{ FSM::stateId<B_2_2>(), Event::RESUME,			FSM::stateId<A    >() },

				{ FSM::stateId<B_2_2>(), Event::EXIT_GUARD },
				{ FSM::stateId<A_2  >(), Event::ENTRY_GUARD },

				{ FSM::stateId<B_1_1>(), Event::EXIT },
				{ FSM::stateId<B_1  >(), Event::EXIT },
				{ FSM::stateId<B_2_2>(), Event::EXIT },
				{ FSM::stateId<B_2  >(), Event::EXIT },
				{ FSM::stateId<B    >(), Event::EXIT },

				{ FSM::stateId<B_1_1>(), Event::DESTRUCT },
				{ FSM::stateId<B_1  >(), Event::DESTRUCT },
				{ FSM::stateId<B_2_2>(), Event::DESTRUCT },
				{ FSM::stateId<B_2  >(), Event::DESTRUCT },
				{ FSM::stateId<B    >(), Event::DESTRUCT },
				{ FSM::stateId<A    >(), Event::CONSTRUCT },
				{ FSM::stateId<A_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<A_2_2>(), Event::CONSTRUCT },

				{ FSM::stateId<A    >(), Event::ENTER },
				{ FSM::stateId<A_2  >(), Event::ENTER },
				{ FSM::stateId<A_2_2>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_2>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<A_2_2>(), Event::EXIT },
		{ FSM::stateId<A_2  >(), Event::EXIT },
		{ FSM::stateId<A    >(), Event::EXIT },

		{ FSM::stateId<A_2_2>(), Event::DESTRUCT },
		{ FSM::stateId<A_2  >(), Event::DESTRUCT },
		{ FSM::stateId<A    >(), Event::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
