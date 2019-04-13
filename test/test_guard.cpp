#include "test_guard.hpp"

using namespace test_guard;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<Step1>(),
		FSM::stateId<Step1_1>(),
		FSM::stateId<Step1_2>(),
		FSM::stateId<Step2>(),
		FSM::stateId<Step2_1>(),
		FSM::stateId<Step2_2>(),
		FSM::stateId<Step3>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Guard test", "[machine]") {
	Context _;
	Logger logger;
	FSM::Instance machine(_, &logger);

	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::ENTER },
			{ FSM::stateId<Step1>(),	Event::ENTER },
			{ FSM::stateId<Step1_1>(),	Event::ENTER },
			{ FSM::stateId<Step1_2>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	// exit guards blocking

	machine.changeTo<Step2>();
	machine.update();
	{
		const Events reference = {
			{							Event::CHANGE, FSM::stateId<Step2>() },

			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1>(),	Event::CANCELLED_PENDING },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
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

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::CANCELLED_PENDING },

			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
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

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::CANCELLED_PENDING },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	// entry guards blocking

	machine.changeTo<Step2>();
	machine.update();
	{
		const Events reference = {
			{							Event::CHANGE, FSM::stateId<Step2>() },

			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },

			{ FSM::stateId<Step2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2>(),	Event::CANCELLED_PENDING },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
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

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },

			{ FSM::stateId<Step2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2_1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2_1>(),	Event::CANCELLED_PENDING },

			{ FSM::stateId<Step2_2>(),	Event::ENTRY_GUARD },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
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

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },

			{ FSM::stateId<Step2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2_1>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2_2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step2_2>(),	Event::CANCELLED_PENDING },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	// successful transition to Step3

	machine.changeTo<Step3>();
	machine.update();
	{
		const Events reference = {
			{							Event::CHANGE, FSM::stateId<Step3>() },

			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Step1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },

			{ FSM::stateId<Step1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD },
			{ FSM::stateId<Step1_2>(),	Event::EXIT_GUARD },

			{ FSM::stateId<Step3>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Step1_1>(),	Event::EXIT },
			{ FSM::stateId<Step1_2>(),	Event::EXIT },
			{ FSM::stateId<Step1>(),	Event::EXIT },

			{ FSM::stateId<Step3>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Step3>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1>(),
			FSM::stateId<Step1_1>(),
			FSM::stateId<Step1_2>()
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////
