#include "test_resumable_o.hpp"

using namespace test_resumable_o;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<A>(),
		FSM::stateId<A_1>(),
		FSM::stateId<A_2>(),
		FSM::stateId<B>(),
		FSM::stateId<B_1>(),
		FSM::stateId<B_2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Resumable test.O", "[machine]") {
	Context _;
	Logger logger;

	FSM::Instance machine(_, &logger);

	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	Event::ENTER },
			{ FSM::stateId<A>(),	Event::ENTER },
			{ FSM::stateId<A_1>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<A>(),
			FSM::stateId<A_1>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.changeTo<A_2>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<A_2>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<A>(),	 Event::UPDATE },
			{ FSM::stateId<A_1>(),	 Event::UPDATE },

			{ FSM::stateId<A_1>(),	 Event::EXIT_GUARD },

			{ FSM::stateId<A_2>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<A_1>(),	 Event::EXIT },

			{ FSM::stateId<A_2>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<A>(),
			FSM::stateId<A_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<A_1>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.changeTo<B>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<B>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<A>(),	 Event::UPDATE },
			{ FSM::stateId<A_2>(),	 Event::UPDATE },

			{ FSM::stateId<A>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<A_2>(),	 Event::EXIT_GUARD },

			{ FSM::stateId<B>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<B_1>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<B_2>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<A_2>(),	 Event::EXIT },
			{ FSM::stateId<A>(),	 Event::EXIT },

			{ FSM::stateId<B>(),	 Event::ENTER },
			{ FSM::stateId<B_1>(),	 Event::ENTER },
			{ FSM::stateId<B_2>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<B>(),
			FSM::stateId<B_1>(),
			FSM::stateId<B_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<A>(),
			FSM::stateId<A_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.changeTo<A>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<A>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<B>(),	 Event::UPDATE },
			{ FSM::stateId<B_1>(),	 Event::UPDATE },
			{ FSM::stateId<B_2>(),	 Event::UPDATE },

			{ FSM::stateId<B>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<B_1>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<B_2>(),	 Event::EXIT_GUARD },

			{ FSM::stateId<A>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<A_2>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<B_1>(),	 Event::EXIT },
			{ FSM::stateId<B_2>(),	 Event::EXIT },
			{ FSM::stateId<B>(),	 Event::EXIT },

			{ FSM::stateId<A>(),	 Event::ENTER },
			{ FSM::stateId<A_2>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<A>(),
			FSM::stateId<A_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<B>(),
			FSM::stateId<B_1>(),
			FSM::stateId<B_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.changeTo<Apex>();
	machine.update();
	{
		const Events reference = {
			{						 Event::CHANGE, FSM::stateId<Apex>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<A>(),	 Event::UPDATE },
			{ FSM::stateId<A_2>(),	 Event::UPDATE },

			{ FSM::stateId<A>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<A_2>(),	 Event::EXIT_GUARD },

			{ FSM::stateId<B>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<B_1>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<B_2>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<A_2>(),	 Event::EXIT },
			{ FSM::stateId<A>(),	 Event::EXIT },

			{ FSM::stateId<B>(),	 Event::ENTER },
			{ FSM::stateId<B_1>(),	 Event::ENTER },
			{ FSM::stateId<B_2>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<B>(),
			FSM::stateId<B_1>(),
			FSM::stateId<B_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<A>(),
			FSM::stateId<A_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.restart<Apex>();
	machine.update();
	{
		const Events reference = {
			{						 Event::RESTART, FSM::stateId<Apex>() },

			{ 0u,					 Event::UPDATE },
			{ FSM::stateId<B>(),	 Event::UPDATE },
			{ FSM::stateId<B_1>(),	 Event::UPDATE },
			{ FSM::stateId<B_2>(),	 Event::UPDATE },

			{ FSM::stateId<B>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<B_1>(),	 Event::EXIT_GUARD },
			{ FSM::stateId<B_2>(),	 Event::EXIT_GUARD },

			{ FSM::stateId<A>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId<A_1>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId<B_1>(),	 Event::EXIT },
			{ FSM::stateId<B_2>(),	 Event::EXIT },
			{ FSM::stateId<B>(),	 Event::EXIT },

			{ FSM::stateId<A>(),	 Event::ENTER },
			{ FSM::stateId<A_1>(),	 Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<A>(),
			FSM::stateId<A_1>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<A_2>(),
			FSM::stateId<B>(),
			FSM::stateId<B_1>(),
			FSM::stateId<B_2>(),
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////
