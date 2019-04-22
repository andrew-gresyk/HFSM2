#include "test_delayed_teardown.hpp"

using namespace test_delayed_teardown;

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

TEST_CASE("Delayed teardown", "[machine]") {
	Context _;
	Logger logger;
	FSM::Instance machine(_, &logger);

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
