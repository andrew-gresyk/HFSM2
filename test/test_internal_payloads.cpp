// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

namespace test_internal_payloads {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::ContextT<float>
					::PayloadT<int>;

using M = hfsm2::MachineT<Config>;

struct RegionStateIndex {
	hfsm2::StateID stateId;
	hfsm2::Short stateIndex;
};

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
	void preUpdate(FullControl& control)			{ _elapsed += control._();	} //-V669

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

	void react(const RegionStateIndex& event,
			   FullControl& control)
	{
		if (control.stateId() == event.stateId)
			REQUIRE(control.activeSubState() == event.stateIndex);
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
	//void react(const RegionStateIndex&, FullControl&)							{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct A_1
	: FSM::State
{
	void enter(PlanControl&)													{}

	void update(FullControl& control) {
		control.changeWith<A_2>(1);
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
			control.changeWith<B_2_2>(2);
			break;

		case 2:
			control.resumeWith<B>(3);
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
	void react(const RegionStateIndex&, FullControl&)							{}
	void exit(PlanControl&)														{}
};

struct A_2_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	//void update(FullControl&)													{}
	//void react(const RegionStateIndex&, FullControl&)							{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(PlanControl&)													{}
	void update(FullControl&)													{}
	void react(const RegionStateIndex&, FullControl&)							{}
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
													   hfsm2::TransitionType::CHANGE,
													   6});

		REQUIRE(control.lastTransition());
		REQUIRE(*control.lastTransition() == M::Transition{FSM::stateId<A_2  >(),
														   FSM::stateId<B    >(),
														   hfsm2::TransitionType::RESUME,
														   6});

		control.cancelPendingTransitions();
		control.resumeWith<B_2_2>(4);
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
			control.resumeWith<A>(5);
			break;

		case 2:
			control.changeWith<B>(6);
			break;

		case 3:
			control.scheduleWith<A_2_2>(7);
			control.resumeWith<A>(8);
			break;
		}
	}

	void exit(PlanControl&)														{}
};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

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

TEST_CASE("FSM.Internal Payloads") {
	float _ = 0.0f;
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_, &logger};
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::Type::ENTER },
				{ FSM::stateId<A_1  >(), Event::Type::ENTER },
			});

			REQUIRE(machine.activeSubState<A  >() == 0);
			REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_SHORT);

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(RegionStateIndex{FSM::stateId<A_2>(), hfsm2::INVALID_SHORT});
		{
			logger.assertSequence({
				{ FSM::stateId<A    >(), Event::Type::PRE_REACT },
				{ FSM::stateId<A_1  >(), Event::Type::PRE_REACT },

				{ FSM::stateId<A    >(), Event::Type::REACT },
				{ FSM::stateId<A_1  >(), Event::Type::REACT },

				{ FSM::stateId<A_1  >(), Event::Type::POST_REACT },
				{ FSM::stateId<A    >(), Event::Type::POST_REACT },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_SHORT);

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A_1  >(),
			});

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_1  >(),
															FSM::stateId<A_2  >(),
															hfsm2::TransitionType::CHANGE,
															1});

			assertLastTransitions(machine, all, {
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[0]);
			REQUIRE(machine.lastTransitionTo<A_2_1>() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(RegionStateIndex{FSM::stateId<A_2>(), 0});
		{
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

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<A_1  >(),
			});

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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

			REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == 0);
			REQUIRE(machine.activeSubState<B_2>() == 1);

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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_2  >(),
															FSM::stateId<B_2_2>(),
															hfsm2::TransitionType::CHANGE,
															2});

			assertLastTransitions(machine, all, {
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

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(RegionStateIndex{FSM::stateId<A_2>(), hfsm2::INVALID_SHORT});
		{
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

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_SHORT);

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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_2>(),
															FSM::stateId<A    >(),
															hfsm2::TransitionType::RESUME,
															3});

			assertLastTransitions(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			REQUIRE(machine.lastTransitionTo<A    >() == &previousTransitions[0]);
			REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[0]);
			REQUIRE(machine.lastTransitionTo<A_2_1>() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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

			REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == 0);
			REQUIRE(machine.activeSubState<B_2>() == 1);

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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<A_2  >(),
															FSM::stateId<B    >(),
															hfsm2::TransitionType::RESUME,
															4});

			assertLastTransitions(machine, all, {
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

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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

			REQUIRE(machine.activeSubState<A  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<A_2>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == 0);
			REQUIRE(machine.activeSubState<B_2>() == 1);

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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_1>(),
															FSM::stateId<B_2_2>(),
															hfsm2::TransitionType::RESUME,
															5});

			// No states were activated
			assertLastTransitions(machine, all, {});

			/* Transition out of an entryGuard()
			assertLastTransitions(machine, all, {
				FSM::stateId<B_2_2>(),
			});

			REQUIRE(machine.lastTransitionTo<B_2_2>() == &previousTransitions[0]);
			*/
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		{
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
			REQUIRE(machine.activeSubState<B  >() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_1>() == hfsm2::INVALID_SHORT);
			REQUIRE(machine.activeSubState<B_2>() == hfsm2::INVALID_SHORT);

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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 2);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_2>(),
															FSM::stateId<A_2_2>(),
															hfsm2::TransitionType::SCHEDULE,
															6});
			REQUIRE(previousTransitions[1] == M::Transition{FSM::stateId<B_2_2>(),
															FSM::stateId<A    >(),
															hfsm2::TransitionType::RESUME,
															7});

			assertLastTransitions(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_2>(),
			});

			REQUIRE(machine.lastTransitionTo<A    >() == &previousTransitions[1]);
			REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[1]);
			REQUIRE(machine.lastTransitionTo<A_2_2>() == &previousTransitions[1]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<A_2_2>(), Event::Type::EXIT },
		{ FSM::stateId<A_2  >(), Event::Type::EXIT },
		{ FSM::stateId<A    >(), Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
