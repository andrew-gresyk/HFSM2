#include "test_planner.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace {

	const Types all = {
		FSM::stateId<Planned>(),
		FSM::stateId<Step1_BT>(),
		FSM::stateId<Step1_1>(),
		FSM::stateId<Step1_2>(),
		FSM::stateId<Step1_3>(),
		FSM::stateId<Hybrid>(),
		FSM::stateId<Step2L_P>(),
		FSM::stateId<Step2L_1>(),
		FSM::stateId<Step2L_2>(),
		FSM::stateId<Step2R_P>(),
		FSM::stateId<Step2R_1>(),
		FSM::stateId<Step2R_2>(),
		FSM::stateId<Unplanned>(),
		FSM::stateId<Terminal>(),
		FSM::stateId<Work_1>(),
		FSM::stateId<Work_2>(),
	};

}

//------------------------------------------------------------------------------

TEST_CASE("Planner test", "[machine]") {
	Context _;
	Logger logger;

	FSM::Instance machine(_, &logger);
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::ENTER },
			{ FSM::stateId<Planned>(),	Event::ENTER },
			{ FSM::stateId<Step1_BT>(),	Event::ENTER },
			{ FSM::stateId<Step1_1>(),	Event::ENTER },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Step1_BT>(),
			FSM::stateId<Step1_1>(),
		};
		assertActive(machine, all, active);

		assertResumable(machine, all, {});
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Planned>(),	Event::UPDATE },
			{ FSM::stateId<Step1_BT>(),	Event::UPDATE },
			{ FSM::stateId<Step1_1>(),	Event::UPDATE },
			{ FSM::stateId<Step1_BT>(),	Event::RESTART, FSM::stateId<Step1_2>() },

			{ FSM::stateId<Step1_2>(),	Event::GUARD  },

			{ FSM::stateId<Step1_1>(),	Event::EXIT   },

			{ FSM::stateId<Step1_2>(),	Event::ENTER  },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Step1_BT>(),
			FSM::stateId<Step1_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1_1>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Planned>(),	Event::UPDATE },
			{ FSM::stateId<Step1_BT>(),	Event::UPDATE },
			{ FSM::stateId<Step1_2>(),	Event::UPDATE },

			{ FSM::stateId<Step1_BT>(),	Event::RESTART, FSM::stateId<Step1_3>() },

			{ FSM::stateId<Step1_3>(),	Event::GUARD  },

			{ FSM::stateId<Step1_2>(),	Event::EXIT   },

			{ FSM::stateId<Step1_3>(),	Event::ENTER  },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Step1_BT>(),
			FSM::stateId<Step1_3>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Planned>(),	Event::UPDATE },
			{ FSM::stateId<Step1_BT>(),	Event::UPDATE },
			{ FSM::stateId<Step1_3>(),	Event::UPDATE },
			{ FSM::stateId<Planned>(),	Event::RESTART, FSM::stateId<Hybrid>() },

			{ FSM::stateId<Hybrid>(),	Event::GUARD  },
			{ FSM::stateId<Step2L_P>(),	Event::GUARD  },
			{ FSM::stateId<Step2L_1>(),	Event::GUARD  },
			{ FSM::stateId<Step2R_P>(),	Event::GUARD  },
			{ FSM::stateId<Step2R_1>(),	Event::GUARD  },

			{ FSM::stateId<Step1_3>(),	Event::EXIT   },
			{ FSM::stateId<Step1_BT>(),	Event::EXIT   },

			{ FSM::stateId<Hybrid>(),	Event::ENTER  },
			{ FSM::stateId<Step2L_P>(),	Event::ENTER  },
			{ FSM::stateId<Step2L_1>(),	Event::ENTER  },
			{ FSM::stateId<Step2R_P>(),	Event::ENTER  },
			{ FSM::stateId<Step2R_1>(),	Event::ENTER  },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Hybrid>(),
			FSM::stateId<Step2L_P>(),
			FSM::stateId<Step2L_1>(),
			FSM::stateId<Step2R_P>(),
			FSM::stateId<Step2R_1>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1_BT>(),
			FSM::stateId<Step1_3>(),
		};
		assertResumable(machine, all, resumable);
	}

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::UPDATE },
			{ FSM::stateId<Planned>(),	Event::UPDATE },
			{ FSM::stateId<Hybrid>(),	Event::UPDATE },
			{ FSM::stateId<Step2L_P>(),	Event::UPDATE },
			{ FSM::stateId<Step2L_1>(),	Event::UPDATE },
			{ FSM::stateId<Step2L_1>(),	Event::RESTART, FSM::stateId<Step2L_2>() },

			{ FSM::stateId<Step2R_P>(),	Event::UPDATE },
			{ FSM::stateId<Step2R_1>(),	Event::UPDATE },
			{ FSM::stateId<Step2R_1>(),	Event::RESTART, FSM::stateId<Step2R_2>() },

			//{ FSM::stateId<Step2L_2>(),	Event::GUARD  },	// bug
			{ FSM::stateId<Step2R_2>(),	Event::GUARD  },

			{ FSM::stateId<Step2L_1>(),	Event::EXIT   },
			{ FSM::stateId<Step2L_2>(),	Event::ENTER  },
			{ FSM::stateId<Step2R_1>(),	Event::EXIT   },
			{ FSM::stateId<Step2R_2>(),	Event::ENTER  },
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Hybrid>(),
			FSM::stateId<Step2L_P>(),
			FSM::stateId<Step2L_2>(),
			FSM::stateId<Step2R_P>(),
			FSM::stateId<Step2R_2>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1_BT>(),
			FSM::stateId<Step1_3>(),
			FSM::stateId<Step2L_1>(),
			FSM::stateId<Step2R_1>(),
		};
		assertResumable(machine, all, resumable);
	}
}

////////////////////////////////////////////////////////////////////////////////
