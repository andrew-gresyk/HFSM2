// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

using namespace test_tools;

namespace test_guards {

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

static_assert(FSM::regionId<Apex  >() ==  0, "");
static_assert(FSM::regionId<Step1 >() ==  1, "");
static_assert(FSM::regionId<Step2 >() ==  2, "");

static_assert(FSM::stateId<Apex   >() ==  0, "");
static_assert(FSM::stateId<Step1  >() ==  1, "");
static_assert(FSM::stateId<Step1_1>() ==  2, "");
static_assert(FSM::stateId<Step1_2>() ==  3, "");
static_assert(FSM::stateId<Step2  >() ==  4, "");
static_assert(FSM::stateId<Step2_1>() ==  5, "");
static_assert(FSM::stateId<Step2_2>() ==  6, "");
static_assert(FSM::stateId<Step3  >() ==  7, "");

////////////////////////////////////////////////////////////////////////////////

class EntryGuardTracked
	: public FSM::State
{
public:
	void entryGuard(GuardControl&)					{ ++_entryGuardCount;		}

	unsigned entryGuardCount() const				{ return _entryGuardCount;	}

private:
	unsigned _entryGuardCount = 0;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class ExitGuardTracked
	: public FSM::State
{
public:
	void exitGuard(GuardControl&)					{ ++_exitGuardCount;		}

	unsigned exitGuardCount() const					{ return _exitGuardCount;	}

private:
	unsigned _exitGuardCount = 0;
};

////////////////////////////////////////////////////////////////////////////////

struct Apex : FSM::State {};

//------------------------------------------------------------------------------

struct Step1
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();

		switch (exitGuardCount()) {
		case 1:
		case 2:
		case 3:
		case 4:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		case 5:
		case 6:
		case 7:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step3  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		default:
			assert(false);
		}

		if (exitGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_1
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();

		switch (exitGuardCount()) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		case 7:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step3  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		default:
			assert(false);
		}

		if (exitGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_2
	: FSM::StateT<ExitGuardTracked>
{
	void exitGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();

		switch (exitGuardCount()) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		case 7:
			REQUIRE(pendingTransitions.count() == 1);
			REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step3  >(),
														   hfsm2::TransitionType::CHANGE});
			break;

		default:
			assert(false);
		}

		if (exitGuardCount() == 2)
			control.cancelPendingTransitions();
	}
};

//------------------------------------------------------------------------------

struct Step2
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE(pendingTransitions.count() == 1);
		REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
													   hfsm2::TransitionType::CHANGE});

		if (entryGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_1
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE(pendingTransitions.count() == 1);
		REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
													   hfsm2::TransitionType::CHANGE});

		if (entryGuardCount() == 1)
			control.cancelPendingTransitions();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2_2
	: FSM::StateT<EntryGuardTracked>
{
	void entryGuard(GuardControl& control) {
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE(pendingTransitions.count() == 1);
		REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<Step2  >(),
													   hfsm2::TransitionType::CHANGE});

		if (entryGuardCount() == 2)
			control.cancelPendingTransitions();
	}
};

//------------------------------------------------------------------------------

struct Step3 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 8, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  == 3, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_COUNT	 == 1, "COMPO_COUNT");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 3, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_COUNT	 == 2, "ORTHO_COUNT");
static_assert(FSM::Instance::Info::ORTHO_UNITS   == 2, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<Apex   >(),
	FSM::stateId<Step1  >(),
	FSM::stateId<Step1_1>(),
	FSM::stateId<Step1_2>(),
	FSM::stateId<Step2  >(),
	FSM::stateId<Step2_1>(),
	FSM::stateId<Step2_2>(),
	FSM::stateId<Step3  >(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Entry Guard") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_1>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Apex   >(),	Event::Type::ENTER },
				{ FSM::stateId<Step1  >(),	Event::Type::ENTER },
				{ FSM::stateId<Step1_1>(),	Event::Type::ENTER },
				{ FSM::stateId<Step1_2>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// exit guards blocking

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_1>(),	Event::Type::CANCEL_PENDING },

				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::CANCEL_PENDING },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::CANCEL_PENDING },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// entry guards blocking

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Step2  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2  >(),	Event::Type::CANCEL_PENDING },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Step2  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2_1>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2_1>(),	Event::Type::CANCEL_PENDING },

				{ FSM::stateId<Step2_2>(),	Event::Type::ENTRY_GUARD },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<Step2>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step2  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Step2  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2_1>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2_2>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<Step2_2>(),	Event::Type::CANCEL_PENDING },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// successful transition to Step3

		machine.immediateChangeTo<Step3>();
		{
			logger.assertSequence({
				{							Event::Type::CHANGE,	FSM::stateId<Step3  >() },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<Step3  >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Step1_1>(),	Event::Type::EXIT },
				{ FSM::stateId<Step1_2>(),	Event::Type::EXIT },
				{ FSM::stateId<Step1  >(),	Event::Type::EXIT },

				{ FSM::stateId<Step3  >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex   >(),
				FSM::stateId<Step3  >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<Step1  >(),
				FSM::stateId<Step1_1>(),
				FSM::stateId<Step1_2>()
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<Step3  >(),	Event::Type::EXIT },
		{ FSM::stateId<Apex   >(),	Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
