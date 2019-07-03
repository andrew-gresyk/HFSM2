#include "test_randomize.hpp"

using namespace test_randomize;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<I>(),
		FSM::stateId<O>(),
		FSM::stateId<N>(),
		FSM::stateId<N_000>(),
		FSM::stateId<N_025>(),
		FSM::stateId<N_050>(),
		FSM::stateId<N_075>(),
		FSM::stateId<N_100>(),
		FSM::stateId<C>(),
		FSM::stateId<C_000>(),
		FSM::stateId<C_025>(),
		FSM::stateId<C_050>(),
		FSM::stateId<C_075>(),
		FSM::stateId<C_100>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Randomize", "[machine]") {
	hfsm2::XoShiRo256Plus generator{0};
	Logger logger;

	FSM::Instance machine{generator, &logger};
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

				{ FSM::stateId<N_000>(), Event::RANK },
				{ FSM::stateId<N_025>(), Event::RANK },
				{ FSM::stateId<N_050>(), Event::RANK },
				{ FSM::stateId<N_075>(), Event::RANK },
				{ FSM::stateId<N_100>(), Event::RANK },

				{ FSM::stateId<N_000>(), Event::UTILITY },
				{ FSM::stateId<N_025>(), Event::UTILITY },
				{ FSM::stateId<N_075>(), Event::UTILITY },

				{ FSM::stateId<N>(),	 Event::RANDOM_RESOLUTION, 3 },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<N>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<N_075>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<N>(),	 Event::ENTER },
				{ FSM::stateId<N_075>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_000>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<N>(),
				FSM::stateId<N_075>(),
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
				{ FSM::stateId<N>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<N_000>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_000>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<N>(),	 Event::ENTER },
				{ FSM::stateId<N_000>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_000>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<N>(),
				FSM::stateId<N_000>(),
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

	WHEN("Randomizing O") {
		machine.randomize<O>();
		machine.update();
		{
			const Events reference = {
				{						 Event::RANDOMIZE, FSM::stateId<O>() },

				{ FSM::stateId<Apex>(),	 Event::UPDATE },
				{ FSM::stateId<I>(),	 Event::UPDATE },

				{ FSM::stateId<N_000>(), Event::RANK },
				{ FSM::stateId<N_025>(), Event::RANK },
				{ FSM::stateId<N_050>(), Event::RANK },
				{ FSM::stateId<N_075>(), Event::RANK },
				{ FSM::stateId<N_100>(), Event::RANK },

				{ FSM::stateId<N_000>(), Event::UTILITY },
				{ FSM::stateId<N_025>(), Event::UTILITY },
				{ FSM::stateId<N_075>(), Event::UTILITY },

				{ FSM::stateId<N>(),	 Event::RANDOM_RESOLUTION, 3 },

				{ FSM::stateId<C_000>(), Event::RANK },
				{ FSM::stateId<C_025>(), Event::RANK },
				{ FSM::stateId<C_050>(), Event::RANK },
				{ FSM::stateId<C_075>(), Event::RANK },
				{ FSM::stateId<C_100>(), Event::RANK },

				{ FSM::stateId<C_000>(), Event::UTILITY },
				{ FSM::stateId<C_025>(), Event::UTILITY },
				{ FSM::stateId<C_075>(), Event::UTILITY },

				{ FSM::stateId<C>(),	 Event::RANDOM_RESOLUTION, 3 },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<O>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<N>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<N_075>(), Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_075>(), Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<O>(),	 Event::ENTER },
				{ FSM::stateId<N>(),	 Event::ENTER },
				{ FSM::stateId<N_075>(), Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_075>(), Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<Apex>(),
				FSM::stateId<O>(),
				FSM::stateId<N>(),
				FSM::stateId<N_075>(),
				FSM::stateId<C>(),
				FSM::stateId<C_075>(),
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
