// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

namespace test_reverse_transitions {

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
	: public FSM::Injection
{
public:
	void preEnter(Context&)							{ _elapsed = 0.0f;			}
	void preReverseUpdate(Context& _)				{ _elapsed += _;			} //-V669

	float elapsed() const							{ return _elapsed;			}

private:
	float _elapsed = 0.0f;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public FSM::Injection
{
public:
	void preEnter(Context&) {
		++_entryCount;
		_currentUpdateCount = 0;
	}

	void preReverseUpdate(Context&) {
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
	void reverseUpdate(FullControl&)											{}
	//void reverseReact(const Action&, FullControl&)							{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct A_1
	: FSM::State
{
	void enter(PlanControl&)													{}

	void reverseUpdate(FullControl& control) {
		control.changeTo<A_2>();
	}

	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct A_2
	: FSM::StateT<Tracked>
{
	void enter(PlanControl&)													{}

	void reverseUpdate(FullControl& control) {
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
	void reverseUpdate(FullControl&)											{}
	void reverseReact(const Action&, FullControl&)								{}
	void exit(PlanControl&)														{}
};

struct A_2_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	//void reverseUpdate(FullControl&)											{}
	//void reverseReact(const Action&, FullControl&)							{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{
	void enter(PlanControl&)													{}
	void reverseUpdate(FullControl&)											{}
	void reverseReact(const Action&, FullControl&)								{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1
	: FSM::State
{
	void enter(PlanControl&)													{}
	void reverseUpdate(FullControl&)											{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1_1
	: FSM::State
{
	void enter(PlanControl&)													{}
	void reverseUpdate(FullControl&)											{}
	void exit(PlanControl&)														{}
};

//------------------------------------------------------------------------------

struct B_1_2
	: FSM::State
{
	//void enter(PlanControl&)													{}
	//void reverseUpdate(FullControl&)											{}
	//void exit(PlanControl&)													{}
};

//------------------------------------------------------------------------------

struct B_2
	: FSM::State
{
	void enter(PlanControl&)													{}
	void reverseUpdate(FullControl&)											{}
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
	void reverseUpdate(FullControl&)											{}
	void exit(PlanControl&)														{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: FSM::StateT<Tracked>
{
	void enter(PlanControl&)													{}

	void reverseUpdate(FullControl& control) {
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

TEST_CASE("FSM.Reverse Transitions") {
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

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			assertResumable(machine, all, {});

			REQUIRE(machine.previousTransitions().count() == 0);

			assertLastTransitions(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.reverseReact(Action{});
		{
			logger.assertSequence({
				{ FSM::stateId<A_1  >(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<A    >(), Event::Type::REVERSE_REACT },
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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<A_1  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<A_1  >(), Event::Type::CHANGE,	FSM::stateId<A_2  >() },

				{ FSM::stateId<A    >(), Event::Type::REVERSE_UPDATE },

				{ FSM::stateId<A_2  >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<A_1  >(), Event::Type::EXIT },

				{ FSM::stateId<A_2  >(), Event::Type::ENTER },
				{ FSM::stateId<A_2_1>(), Event::Type::ENTER },
			});

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
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(machine, all, {
				FSM::stateId<A_2  >(),
				FSM::stateId<A_2_1>(),
			});

			REQUIRE(machine.lastTransitionTo<A_2  >() == &previousTransitions[0]);
			REQUIRE(machine.lastTransitionTo<A_2_1>() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.reverseReact(Action{});
		{
			logger.assertSequence({
				{ FSM::stateId<A_2_1>(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<A_2  >(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<A    >(), Event::Type::REVERSE_REACT },
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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<A_2_1>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<A_2  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<A_2  >(), Event::Type::CHANGE,	FSM::stateId<B_2_2>() },

				{ FSM::stateId<A    >(), Event::Type::REVERSE_UPDATE },

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
															hfsm2::TransitionType::CHANGE});

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

		machine.reverseReact(Action{});
		{
			logger.assertSequence({
				{ FSM::stateId<B_1_1>(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<B_1  >(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<B_2_2>(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<B_2  >(), Event::Type::REVERSE_REACT },
				{ FSM::stateId<B    >(), Event::Type::REVERSE_REACT },
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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<B_1_1>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_1  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::RESUME,	FSM::stateId<A    >() },

				{ FSM::stateId<B_2  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B    >(), Event::Type::REVERSE_UPDATE },

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
															hfsm2::TransitionType::RESUME});

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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<A_2_1>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<A_2  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<A_2  >(), Event::Type::RESUME,	FSM::stateId<B    >() },

				{ FSM::stateId<A    >(), Event::Type::REVERSE_UPDATE },

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
															hfsm2::TransitionType::RESUME});

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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<B_1_1>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_1  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::CHANGE,	FSM::stateId<B    >() },

				{ FSM::stateId<B_2  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B    >(), Event::Type::REVERSE_UPDATE },

				{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_2_1>(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<B_2_1>(), Event::Type::CANCEL_PENDING },

				{ FSM::stateId<B_2_1>(), Event::Type::RESUME,	FSM::stateId<B_2_2>() },

				{ FSM::stateId<B_2_2>(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<B_2_2>(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<B_2_2>(), Event::Type::REENTER },
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

			const auto& previousTransitions = machine.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{FSM::stateId<B_2_1>(),
															FSM::stateId<B_2_2>(),
															hfsm2::TransitionType::RESUME});

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

		machine.reverseUpdate();
		{
			logger.assertSequence({
				{ FSM::stateId<B_1_1>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_1  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B_2_2>(), Event::Type::SCHEDULE,	FSM::stateId<A_2_2>() },
				{ FSM::stateId<B_2_2>(), Event::Type::RESUME,	FSM::stateId<A    >() },

				{ FSM::stateId<B_2  >(), Event::Type::REVERSE_UPDATE },
				{ FSM::stateId<B    >(), Event::Type::REVERSE_UPDATE },

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
															hfsm2::TransitionType::SCHEDULE});
			REQUIRE(previousTransitions[1] == M::Transition{FSM::stateId<B_2_2>(),
															FSM::stateId<A    >(),
															hfsm2::TransitionType::RESUME});

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