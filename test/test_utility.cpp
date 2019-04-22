#include "test_utility.hpp"

using namespace test_utility;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<I>(),
		FSM::stateId<O>(),
		FSM::stateId<U>(),
		FSM::stateId<U_000>(),
		FSM::stateId<U_025>(),
		FSM::stateId<U_050>(),
		FSM::stateId<U_075>(),
		FSM::stateId<U_100>(),
		FSM::stateId<C>(),
		FSM::stateId<C_000>(),
		FSM::stateId<C_025>(),
		FSM::stateId<C_050>(),
		FSM::stateId<C_075>(),
		FSM::stateId<C_100>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Utility", "[machine]") {
	Context _;
	Logger logger;

	FSM::Instance machine(_, &logger);

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

	WHEN("Changing to O") {
		machine.changeTo<O>();
		machine.update();
		{
			const Events reference = {
				{						 Event::CHANGE, FSM::stateId<O>() },

				{ FSM::stateId<Apex>(),	 Event::UPDATE },
				{ FSM::stateId<I>(),	 Event::UPDATE },

				{ FSM::stateId<U_000>(), Event::UTILITY },
				{ FSM::stateId<U_025>(), Event::UTILITY },
				{ FSM::stateId<U_050>(), Event::UTILITY },
				{ FSM::stateId<U_075>(), Event::UTILITY },
				{ FSM::stateId<U_100>(), Event::UTILITY },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U_100>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<U>(),	 Event::ENTER },
				{ FSM::stateId<U_100>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_000>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<U>(),
				FSM::stateId<U_100>(),
				FSM::stateId<C>(),
				FSM::stateId<C_000>(),
			};
			assertActive(machine, all, active);

			const Types resumable = {
				FSM::stateId<I>(),
			};
			assertResumable(machine, all, resumable);
		}
	}

	WHEN("Restarting O") {
		machine.restart<O>();
		machine.update();
		{
			const Events reference = {
				{						 Event::RESTART, FSM::stateId<O>() },

				{ FSM::stateId<Apex>(),	 Event::UPDATE },
				{ FSM::stateId<I>(),	 Event::UPDATE },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U_000>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<U>(),	 Event::ENTER },
				{ FSM::stateId<U_000>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_000>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<U>(),
				FSM::stateId<U_000>(),
				FSM::stateId<C>(),
				FSM::stateId<C_000>(),
			};
			assertActive(machine, all, active);

			const Types resumable = {
				FSM::stateId<I>(),
			};
			assertResumable(machine, all, resumable);
		}
	}

	WHEN("Utilizing O") {
		machine.utilize<O>();
		machine.update();
		{
			const Events reference = {
				{						 Event::UTILIZE, FSM::stateId<O>() },

				{ FSM::stateId<Apex>(),	 Event::UPDATE },
				{ FSM::stateId<I>(),	 Event::UPDATE },

				{ FSM::stateId<U_000>(), Event::UTILITY },
				{ FSM::stateId<U_025>(), Event::UTILITY },
				{ FSM::stateId<U_050>(), Event::UTILITY },
				{ FSM::stateId<U_075>(), Event::UTILITY },
				{ FSM::stateId<U_100>(), Event::UTILITY },

				{ FSM::stateId<C_000>(), Event::UTILITY },
				{ FSM::stateId<C_025>(), Event::UTILITY },
				{ FSM::stateId<C_050>(), Event::UTILITY },
				{ FSM::stateId<C_075>(), Event::UTILITY },
				{ FSM::stateId<C_100>(), Event::UTILITY },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<U_100>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_100>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<U>(),	 Event::ENTER },
				{ FSM::stateId<U_100>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_100>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<U>(),
				FSM::stateId<U_100>(),
				FSM::stateId<C>(),
				FSM::stateId<C_100>(),
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
