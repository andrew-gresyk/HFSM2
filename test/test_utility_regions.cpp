#include "test_utility_regions.hpp"

using namespace test_utility_regions;

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<I>(),
		FSM::stateId<F>(),
		FSM::stateId<C>(),
		FSM::stateId<C_1>(),
		FSM::stateId<C_2>(),
		FSM::stateId<C_3>(),
		FSM::stateId<R>(),
		FSM::stateId<R_1>(),
		FSM::stateId<R_2>(),
		FSM::stateId<R_3>(),
		FSM::stateId<U>(),
		FSM::stateId<U_1>(),
		FSM::stateId<U_2>(),
		FSM::stateId<U_3>(),
		FSM::stateId<O>(),
		FSM::stateId<OC>(),
		FSM::stateId<OC_1>(),
		FSM::stateId<OC_2>(),
		FSM::stateId<OC_3>(),
		FSM::stateId<OR>(),
		FSM::stateId<OR_1>(),
		FSM::stateId<OR_2>(),
		FSM::stateId<OR_3>(),
		FSM::stateId<OU>(),
		FSM::stateId<OU_1>(),
		FSM::stateId<OU_2>(),
		FSM::stateId<OU_3>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Utility Regions", "[machine]") {
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

	{
		machine.changeTo<F>();
		machine.update();
		{
			const Events reference = {
				{						 Event::CHANGE, FSM::stateId<F>() },

				{ FSM::stateId<Apex>(),	 Event::UPDATE },
				{ FSM::stateId<I>(),	 Event::UPDATE },

				{ FSM::stateId<C>(),	 Event::UTILITY },
				{ FSM::stateId<C_1>(),	 Event::UTILITY },

				{ FSM::stateId<R>(),	 Event::UTILITY },
				{ FSM::stateId<R_1>(),	 Event::UTILITY },

				{ FSM::stateId<U>(),	 Event::UTILITY },
				{ FSM::stateId<U_1>(),	 Event::UTILITY },
				{ FSM::stateId<U_2>(),	 Event::UTILITY },
				{ FSM::stateId<U_3>(),	 Event::UTILITY },
				{ FSM::stateId<U>(),	 Event::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<O>(),	 Event::UTILITY },

				{ FSM::stateId<OC>(),	 Event::UTILITY },
				{ FSM::stateId<OC_1>(),	 Event::UTILITY },

				{ FSM::stateId<OR>(),	 Event::UTILITY },
				{ FSM::stateId<OR_1>(),	 Event::UTILITY },

				{ FSM::stateId<OU>(),	 Event::UTILITY },
				{ FSM::stateId<OU_1>(),	 Event::UTILITY },
				{ FSM::stateId<OU_2>(),	 Event::UTILITY },
				{ FSM::stateId<OU_3>(),	 Event::UTILITY },
				{ FSM::stateId<OU>(),	 Event::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<O>(),	 Event::UTILITY_RESOLUTION },
				{ FSM::stateId<F>(),	 Event::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<I>(),	 Event::EXIT_GUARD },

				{ FSM::stateId<F>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C>(),	 Event::ENTRY_GUARD },
				{ FSM::stateId<C_1>(),	 Event::ENTRY_GUARD },

				{ FSM::stateId<I>(),	 Event::EXIT },

				{ FSM::stateId<F>(),	 Event::ENTER },
				{ FSM::stateId<C>(),	 Event::ENTER },
				{ FSM::stateId<C_1>(),	 Event::ENTER },
			};
			logger.assertSequence(reference);

			const Types active = {
				FSM::stateId<F>(),
				FSM::stateId<C>(),
				FSM::stateId<C_1>(),
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
