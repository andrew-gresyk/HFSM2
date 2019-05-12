#include "test_resumable.hpp"

using namespace test_resumable;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<I>(),
		FSM::stateId<O>(),
		FSM::stateId<OR>(),
		FSM::stateId<OR_1>(),
		FSM::stateId<OR_2>(),
		FSM::stateId<OC>(),
		FSM::stateId<OC_1>(),
		FSM::stateId<OC_2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Resumable", "[machine]") {
	Logger logger;

	FSM::Instance machine(&logger);
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<I>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<Apex>(),	Event::ENTER },
			{ FSM::stateId<I>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<I>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.changeTo<OR_2>();
	machine.changeTo<OC_2>();
	machine.update();
	{
		const Events reference = {
			{						Event::CHANGE, FSM::stateId<OR_2>() },
			{						Event::CHANGE, FSM::stateId<OC_2>() },

			{ FSM::stateId<Apex>(),	Event::UPDATE },
			{ FSM::stateId<I>(),	Event::UPDATE },

			{ FSM::stateId<I>(),	Event::EXIT_GUARD },

			{ FSM::stateId<O>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<OR>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<OC>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<OC_2>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<I>(),	Event::EXIT },

			{ FSM::stateId<O>(),	Event::ENTER },
			{ FSM::stateId<OR>(),	Event::ENTER },
			{ FSM::stateId<OR_2>(),	Event::ENTER },
			{ FSM::stateId<OC>(),	Event::ENTER },
			{ FSM::stateId<OC_2>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<O>(),
			FSM::stateId<OR>(),
			FSM::stateId<OR_2>(),
			FSM::stateId<OC>(),
			FSM::stateId<OC_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<I>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.resume<Apex>();
	machine.update();
	{
		const Events reference = {
			{						Event::RESUME, FSM::stateId<Apex>() },

			{ FSM::stateId<Apex>(),	Event::UPDATE },
			{ FSM::stateId<O>(),	Event::UPDATE },
			{ FSM::stateId<OR>(),	Event::UPDATE },
			{ FSM::stateId<OR_2>(),	Event::UPDATE },
			{ FSM::stateId<OC>(),	Event::UPDATE },
			{ FSM::stateId<OC_2>(),	Event::UPDATE },

			{ FSM::stateId<O>(),	Event::EXIT_GUARD },
			{ FSM::stateId<OR>(),	Event::EXIT_GUARD },
			{ FSM::stateId<OR_2>(),	Event::EXIT_GUARD },
			{ FSM::stateId<OC>(),	Event::EXIT_GUARD },
			{ FSM::stateId<OC_2>(),	Event::EXIT_GUARD },

			{ FSM::stateId<I>(),	Event::ENTRY_GUARD },

			{ FSM::stateId<OR_2>(),	Event::EXIT },
			{ FSM::stateId<OR>(),	Event::EXIT },
			{ FSM::stateId<OC_2>(),	Event::EXIT },
			{ FSM::stateId<OC>(),	Event::EXIT },
			{ FSM::stateId<O>(),	Event::EXIT },

			{ FSM::stateId<I>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<I>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<O>(),
			FSM::stateId<OR>(),
			FSM::stateId<OR_2>(),
			FSM::stateId<OC>(),
			FSM::stateId<OC_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	WHEN("Changing to Apex") {
		machine.changeTo<Apex>();
		machine.update();
		{
			const Events reference = {
				{						Event::CHANGE, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I>(),	Event::UPDATE },

				{ FSM::stateId<I>(),	Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	Event::EXIT },

				{ FSM::stateId<O>(),	Event::ENTER },
				{ FSM::stateId<OR>(),	Event::ENTER },
				{ FSM::stateId<OR_2>(),	Event::ENTER },
				{ FSM::stateId<OC>(),	Event::ENTER },
				{ FSM::stateId<OC_1>(),	Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<O>(),
				FSM::stateId<OR>(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC>(),
				FSM::stateId<OC_1>(),
			};
			assertActive(machine, all, active);

			const Types resumable = {
				FSM::stateId<I>(),
				FSM::stateId<OC_2>(),
			};
			assertResumable(machine, all, resumable);
		}
	}

	WHEN("Restarting O") {
		machine.restart<O>();
		machine.update();

		{
			const Events reference = {
				{						Event::RESTART, FSM::stateId<O>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I>(),	Event::UPDATE },

				{ FSM::stateId<I>(),	Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_1>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	Event::EXIT },

				{ FSM::stateId<O>(),	Event::ENTER },
				{ FSM::stateId<OR>(),	Event::ENTER },
				{ FSM::stateId<OR_1>(),	Event::ENTER },
				{ FSM::stateId<OC>(),	Event::ENTER },
				{ FSM::stateId<OC_1>(),	Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<O>(),
				FSM::stateId<OR>(),
				FSM::stateId<OR_1>(),
				FSM::stateId<OC>(),
				FSM::stateId<OC_1>(),
			};
			assertActive(machine, all, active);

			const Types resumable = {
				FSM::stateId<I>(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC_2>(),
			};
			assertResumable(machine, all, resumable);
		}
	}

	WHEN("Resuming Apex") {
		machine.resume<Apex>();
		machine.update();

		{
			const Events reference = {
				{						Event::RESUME, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I>(),	Event::UPDATE },

				{ FSM::stateId<I>(),	Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_2>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	Event::EXIT },

				{ FSM::stateId<O>(),	Event::ENTER },
				{ FSM::stateId<OR>(),	Event::ENTER },
				{ FSM::stateId<OR_2>(),	Event::ENTER },
				{ FSM::stateId<OC>(),	Event::ENTER },
				{ FSM::stateId<OC_2>(),	Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<O>(),
				FSM::stateId<OR>(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC>(),
				FSM::stateId<OC_2>(),
			};
			assertActive(machine, all, active);

			const Types resumable = {
				FSM::stateId<I>(),
			};
			assertResumable(machine, all, resumable);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
