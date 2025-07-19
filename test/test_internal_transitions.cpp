// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

using namespace test_tools;

namespace test_internal_transitions {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::ContextT<float>;

using M = hfsm2::MachineT<Config>;

using Action = bool;

using Logger = LoggerT<Config>;

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

////////////////////////////////////////////////////////////////////////////////

class Timed
	: public FSM::State
{
public:
	void enter(PlanControl&)						{ _elapsed = 0.0f;			}
	void preUpdate(FullControl& control)			{ _elapsed += control._();	}

	float elapsed() const							{ return _elapsed;			}

private:
	float _elapsed = 0.0f;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public FSM::State
{
public:
	void enter(PlanControl&) {
		++_entryCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(FullControl&) {
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
	//void react(const Action&, EventControl&)									{}
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
	void react(const Action&, EventControl&)									{}
	void exit(PlanControl&)														{}
};

struct A_2_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	//void update(FullControl&)													{}
	//void react(const Action&, EventControl&)									{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void react(const Action&, EventControl&)									{}
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
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE(pendingTransitions.count() == 1);
		REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<B_2_2>(),
													   FSM::stateId<B    >(),
													   hfsm2::TransitionType::CHANGE});

		REQUIRE(control.lastTransition());
		REQUIRE(*control.lastTransition() == M::Transition{FSM::stateId<A_2  >(),
														   FSM::stateId<B    >(),
														   hfsm2::TransitionType::RESUME});

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

static_assert(FSM::Instance::Info::STATE_COUNT  == 13, "");
static_assert(FSM::Instance::Info::REGION_COUNT ==  6, "");
static_assert(FSM::Instance::Info::COMPO_COUNT	==  5, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS == 10, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT	==  1, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS  ==  1, "");

////////////////////////////////////////////////////////////////////////////////

void step0(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ FSM::stateId<A    >(), Event::Type::ENTER },
		{ FSM::stateId<A_1  >(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == 0);
	REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_PRONG);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_1  >(),
	});

	assertResumable(machine, {});

	REQUIRE(machine.previousTransitions().count() == 0);

	assertLastTransitions(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step1(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ FSM::stateId<A    >(), Event::Type::PRE_REACT },
		{ FSM::stateId<A_1  >(), Event::Type::PRE_REACT },

		{ FSM::stateId<A    >(), Event::Type::REACT },
		{ FSM::stateId<A_1  >(), Event::Type::REACT },

		{ FSM::stateId<A_1  >(), Event::Type::POST_REACT },
		{ FSM::stateId<A    >(), Event::Type::POST_REACT },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_1  >(),
	});

	assertResumable(machine, {});

	REQUIRE(machine.previousTransitions().count() == 0);

	assertLastTransitions(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<A    >(), Event::Type::UPDATE },
		{ FSM::stateId<A_1  >(), Event::Type::UPDATE },
		{ FSM::stateId<A_1  >(), Event::Type::CHANGE,	FSM::stateId<A_2  >() },

		{ FSM::stateId<A_2  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A_1  >(), Event::Type::EXIT },

		{ FSM::stateId<A_2  >(), Event::Type::ENTER },
		{ FSM::stateId<A_2_1>(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == 1);
	REQUIRE(machine.activeSubState<A_2>() == 0);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_PRONG);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<A_1  >(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 1);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_1  >(),
													FSM::stateId<A_2  >(),
													hfsm2::TransitionType::CHANGE});

	assertLastTransitions(machine, {
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<A_2_1>() == &previousTransitions[0]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ FSM::stateId<A    >(), Event::Type::PRE_REACT },
		{ FSM::stateId<A_2  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<A_2_1>(), Event::Type::PRE_REACT },

		{ FSM::stateId<A    >(), Event::Type::REACT },
		{ FSM::stateId<A_2  >(), Event::Type::REACT },
		{ FSM::stateId<A_2_1>(), Event::Type::REACT },

		{ FSM::stateId<A_2_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<A_2  >(), Event::Type::POST_REACT },
		{ FSM::stateId<A    >(), Event::Type::POST_REACT },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<A_1  >(),
	});

	REQUIRE(machine.previousTransitions().count() == 0);

	assertLastTransitions(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<A_2  >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<A    >(), Event::Type::UPDATE },
		{ FSM::stateId<A_2  >(), Event::Type::UPDATE },
		{ FSM::stateId<A_2  >(), Event::Type::CHANGE,	FSM::stateId<B_2_2>() },

		{ FSM::stateId<A_2_1>(), Event::Type::UPDATE },

		{ FSM::stateId<A_2  >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<A_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<B_2_2>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A_2_1>(), Event::Type::EXIT },
		{ FSM::stateId<A_2  >(), Event::Type::EXIT },
		{ FSM::stateId<A    >(), Event::Type::EXIT },

		{ FSM::stateId<B    >(), Event::Type::ENTER },
		{ FSM::stateId<B_1  >(), Event::Type::ENTER },
		{ FSM::stateId<B_1_1>(), Event::Type::ENTER },
		{ FSM::stateId<B_2  >(), Event::Type::ENTER },
		{ FSM::stateId<B_2_2>(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == 0);
	REQUIRE(machine.activeSubState<B_2>() == 1);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 1);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_2  >(),
													FSM::stateId<B_2_2>(),
													hfsm2::TransitionType::CHANGE});

	assertLastTransitions(machine, {
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	REQUIRE(machine.lastTransitionTo<B    >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_1  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_1_1>() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_2  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_2_2>() == &previousTransitions[0]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step5(FSM::Instance& machine, Logger& logger) {
	machine.react(Action{});

	logger.assertSequence({
		{ FSM::stateId<B    >(), Event::Type::PRE_REACT },
		{ FSM::stateId<B_1  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<B_1_1>(), Event::Type::PRE_REACT },
		{ FSM::stateId<B_2  >(), Event::Type::PRE_REACT },
		{ FSM::stateId<B_2_2>(), Event::Type::PRE_REACT },

		{ FSM::stateId<B    >(), Event::Type::REACT },
		{ FSM::stateId<B_1  >(), Event::Type::REACT },
		{ FSM::stateId<B_1_1>(), Event::Type::REACT },
		{ FSM::stateId<B_2  >(), Event::Type::REACT },
		{ FSM::stateId<B_2_2>(), Event::Type::REACT },

		{ FSM::stateId<B_1_1>(), Event::Type::POST_REACT },
		{ FSM::stateId<B_1  >(), Event::Type::POST_REACT },
		{ FSM::stateId<B_2_2>(), Event::Type::POST_REACT },
		{ FSM::stateId<B_2  >(), Event::Type::POST_REACT },
		{ FSM::stateId<B    >(), Event::Type::POST_REACT },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	REQUIRE(machine.previousTransitions().count() == 0);

	assertLastTransitions(machine, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step6(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<B_2_2>(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<B    >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1_1>(), Event::Type::UPDATE },
		{ FSM::stateId<B_2  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_2_2>(), Event::Type::UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::RESUME,	FSM::stateId<A    >() },

		{ FSM::stateId<B_2_2>(), Event::Type::POST_UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<A_2  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B_1_1>(), Event::Type::EXIT },
		{ FSM::stateId<B_1  >(), Event::Type::EXIT },
		{ FSM::stateId<B_2_2>(), Event::Type::EXIT },
		{ FSM::stateId<B_2  >(), Event::Type::EXIT },
		{ FSM::stateId<B    >(), Event::Type::EXIT },

		{ FSM::stateId<A    >(), Event::Type::ENTER },
		{ FSM::stateId<A_2  >(), Event::Type::ENTER },
		{ FSM::stateId<A_2_1>(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == 1);
	REQUIRE(machine.activeSubState<A_2>() == 0);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_PRONG);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	assertResumable(machine, {
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 1);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_2>(),
													FSM::stateId<A    >(),
													hfsm2::TransitionType::RESUME});

	assertLastTransitions(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	REQUIRE(machine.lastTransitionTo<A    >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<A_2_1>() == &previousTransitions[0]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step7(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<A_2  >(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<A    >(), Event::Type::UPDATE },
		{ FSM::stateId<A_2  >(), Event::Type::UPDATE },

		{ FSM::stateId<A_2  >(), Event::Type::RESUME,	FSM::stateId<B    >() },

		{ FSM::stateId<A_2_1>(), Event::Type::UPDATE },

		{ FSM::stateId<A_2  >(), Event::Type::POST_UPDATE },

		{ FSM::stateId<A_2  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<B_2_2>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<A_2_1>(), Event::Type::EXIT },
		{ FSM::stateId<A_2  >(), Event::Type::EXIT },
		{ FSM::stateId<A    >(), Event::Type::EXIT },

		{ FSM::stateId<B    >(), Event::Type::ENTER },
		{ FSM::stateId<B_1  >(), Event::Type::ENTER },
		{ FSM::stateId<B_1_1>(), Event::Type::ENTER },
		{ FSM::stateId<B_2  >(), Event::Type::ENTER },
		{ FSM::stateId<B_2_2>(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == 0);
	REQUIRE(machine.activeSubState<B_2>() == 1);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 1);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_2  >(),
													FSM::stateId<B    >(),
													hfsm2::TransitionType::RESUME});

	assertLastTransitions(machine, {
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	REQUIRE(machine.lastTransitionTo<B    >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_1  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_1_1>() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_2  >() == &previousTransitions[0]);
	REQUIRE(machine.lastTransitionTo<B_2_2>() == &previousTransitions[0]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step8(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<B_2_2>(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<B    >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1_1>(), Event::Type::UPDATE },
		{ FSM::stateId<B_2  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_2_2>(), Event::Type::UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::CHANGE,	FSM::stateId<B    >() },

		{ FSM::stateId<B_2_2>(), Event::Type::POST_UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<B_2_1>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B_2_1>(), Event::Type::CANCEL_PENDING },

		{ FSM::stateId<B_2_1>(), Event::Type::RESUME,	FSM::stateId<B_2_2>() },

		{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<B_2_2>(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B_2_2>(), Event::Type::REENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == 0);
	REQUIRE(machine.activeSubState<B_2>() == 1);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_1>(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 1);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_1>(),
													FSM::stateId<B_2_2>(),
													hfsm2::TransitionType::RESUME});

	// No states were activated
	assertLastTransitions(machine, {});

	/* Transition out of an entryGuard()
	assertLastTransitions(machine, {
		FSM::stateId<B_2_2>(),
	});

	REQUIRE(machine.lastTransitionTo<B_2_2>() == &previousTransitions[0]);
	*/
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step9(FSM::Instance& machine, Logger& logger) {
	machine.update();

	logger.assertSequence({
		{ FSM::stateId<B_2_2>(), Event::Type::PRE_UPDATE },

		{ FSM::stateId<B    >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_1_1>(), Event::Type::UPDATE },
		{ FSM::stateId<B_2  >(), Event::Type::UPDATE },
		{ FSM::stateId<B_2_2>(), Event::Type::UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::SCHEDULE,	FSM::stateId<A_2_2>() },
		{ FSM::stateId<B_2_2>(), Event::Type::RESUME,	FSM::stateId<A    >() },

		{ FSM::stateId<B_2_2>(), Event::Type::POST_UPDATE },

		{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<A_2  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<B_1_1>(), Event::Type::EXIT },
		{ FSM::stateId<B_1  >(), Event::Type::EXIT },
		{ FSM::stateId<B_2_2>(), Event::Type::EXIT },
		{ FSM::stateId<B_2  >(), Event::Type::EXIT },
		{ FSM::stateId<B    >(), Event::Type::EXIT },

		{ FSM::stateId<A    >(), Event::Type::ENTER },
		{ FSM::stateId<A_2  >(), Event::Type::ENTER },
		{ FSM::stateId<A_2_2>(), Event::Type::ENTER },
	});

	REQUIRE(machine.activeSubState<A  >() == 1);
	REQUIRE(machine.activeSubState<A_2>() == 1);
	REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_PRONG);
	REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_PRONG);

	assertActive(machine, {
		hfsm2::ROOT_ID,
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_2>(),
	});

	assertResumable(machine, {
		FSM::stateId<B    >(),
		FSM::stateId<B_1  >(),
		FSM::stateId<B_1_1>(),
		FSM::stateId<B_2  >(),
		FSM::stateId<B_2_2>(),
	});

	const auto& previousTransitions = machine.previousTransitions();
	REQUIRE(previousTransitions.count() == 2);
	REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_2>(),
													FSM::stateId<A_2_2>(),
													hfsm2::TransitionType::SCHEDULE});
	REQUIRE(previousTransitions[1] == M::Transition{FSM::stateId<B_2_2>(),
													FSM::stateId<A    >(),
													hfsm2::TransitionType::RESUME});

	assertLastTransitions(machine, {
		FSM::stateId<A    >(),
		FSM::stateId<A_2  >(),
		FSM::stateId<A_2_2>(),
	});

	REQUIRE(machine.lastTransitionTo<A    >() == &previousTransitions[1]);
	REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[1]);
	REQUIRE(machine.lastTransitionTo<A_2_2>() == &previousTransitions[1]);
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Internal Transition") {
	float context = 0.0f;
	Logger logger;

	{
		FSM::Instance machine{context, &logger};
		step0(machine, logger);
		step1(machine, logger);
		step2(machine, logger);
		step3(machine, logger);
		step4(machine, logger);
		step5(machine, logger);
		step6(machine, logger);
		step7(machine, logger);
		step8(machine, logger);
		step9(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<A_2_2>(), Event::Type::EXIT },
		{ FSM::stateId<A_2  >(), Event::Type::EXIT },
		{ FSM::stateId<A    >(), Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
