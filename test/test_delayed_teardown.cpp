#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_delayed_teardown {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Orthogonal<S(Step1),
					S(Step1_1),
					M::Composite<S(Step1_2),
						S(Setup),
						S(Work),
						S(Teardown)
					>
				>,
				S(Step2)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>()		==  0, "");
static_assert(FSM::regionId<Step1>()	==  1, "");

static_assert(FSM::stateId<Apex>()		==  0, "");
static_assert(FSM::stateId<Step1>()		==  1, "");
static_assert(FSM::stateId<Step1_1>()	==  2, "");
static_assert(FSM::stateId<Step1_2>()	==  3, "");
static_assert(FSM::stateId<Setup>()		==  4, "");
static_assert(FSM::stateId<Work>()		==  5, "");
static_assert(FSM::stateId<Teardown>()	==  6, "");
static_assert(FSM::stateId<Step2>()		==  7, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	   : FSM::State {};
struct Step1   : FSM::State {};
struct Step1_1 : FSM::State {};
struct Step1_2 : FSM::State {};

//------------------------------------------------------------------------------

struct Setup
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Work>();
	}
};

struct Work
	: FSM::State
{
	void exitGuard(GuardControl& control) {
		if (!control.isPendingEnter<Teardown>()) {
			for (const auto& transition : control.pendingTransitions())
				REQUIRE(control.plan().change<Teardown>(transition.stateId));

			control.cancelPendingTransitions();
			control.changeTo<Teardown>();
		}
	}
};

struct Teardown
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Step2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 8, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 2, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 5, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   == 1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<Step1>(),
		FSM::stateId<Step1_1>(),
		FSM::stateId<Step1_2>(),
		FSM::stateId<Setup>(),
		FSM::stateId<Work>(),
		FSM::stateId<Teardown>(),
		FSM::stateId<Step2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Exit Guard", "[machine]") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::ENTRY_GUARD },
			{ FSM::stateId<Step1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Setup>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),		Event::ENTER },
			{ FSM::stateId<Step1>(),	Event::ENTER },
			{ FSM::stateId<Step1_1>(),	Event::ENTER },
			{ FSM::stateId<Step1_2>(),	Event::ENTER },
			{ FSM::stateId<Setup>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
			FSM::stateId<Setup>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },
			{ FSM::stateId<Setup>(),	Event::UPDATE },

			{ FSM::stateId<Setup>(),	Event::CHANGE, FSM::stateId<Work>() },

			{ FSM::stateId<Setup>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Work>(),		Event::ENTRY_GUARD },

			{ FSM::stateId<Setup>(),	Event::EXIT },
			{ FSM::stateId<Work>(),		Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
			FSM::stateId<Work>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Setup>()
		};
		assertResumable(machine, all, resumable);
	}

	machine.changeTo<Step2>();
	machine.update();
	{
		const Events reference = {
			{							Event::CHANGE, FSM::stateId<Step2>() },

			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },
			{ FSM::stateId<Work>(),		Event::UPDATE },

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Work>(),		Event::EXIT_GUARD },

			{ FSM::stateId<Work>(),		Event::CANCELLED_PENDING },

			{ FSM::stateId<Work>(),		Event::CHANGE, FSM::stateId<Teardown>() },

			{ FSM::stateId<Work>(),		Event::EXIT_GUARD  },
			{ FSM::stateId<Teardown>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Work>(),		Event::EXIT  },
			{ FSM::stateId<Teardown>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
			FSM::stateId<Teardown>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Work>()
		};
		assertResumable(machine, all, resumable);
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },
			{ FSM::stateId<Teardown>(),	Event::UPDATE },

			{ FSM::regionId<Step1_2>(),	Event::TASK_SUCCESS, FSM::stateId<Teardown>() },
			{ FSM::stateId<Step1_2>(),	Event::CHANGE,		 FSM::stateId<Step2>() },

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Teardown>(),	Event::EXIT_GUARD },

			{ FSM::stateId<Step2>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Step1_1>(),	Event::EXIT },
			{ FSM::stateId<Teardown>(),	Event::EXIT },
			{ FSM::stateId<Step1_2>(),	Event::EXIT },
			{ FSM::stateId<Step1>(),	Event::EXIT },

			{ FSM::stateId<Step2>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
			FSM::stateId<Teardown>()
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////

}
