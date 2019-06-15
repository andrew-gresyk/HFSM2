#include "test_planner.hpp"

using namespace test_planner;

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

TEST_CASE("Planner", "[machine]") {
	Logger logger;

	FSM::Instance machine{&logger};
	{
		const Events reference = {
			{ FSM::stateId<Apex>(),		Event::ENTRY_GUARD },
			{ FSM::stateId<Planned>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step1_BT>(),	Event::ENTRY_GUARD },
			{ FSM::stateId<Step1_1>(),	Event::ENTRY_GUARD },

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
			{ FSM::stateId<Step1_1>(),	Event::CHANGE, FSM::stateId<Step1_2>() },

			{ FSM::stateId<Step1_1>(),	Event::EXIT_GUARD  },

			{ FSM::stateId<Step1_2>(),	Event::ENTRY_GUARD },

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
			{ FSM::stateId <Apex>(),	 Event::UPDATE },
			{ FSM::stateId <Planned>(),	 Event::UPDATE },
			{ FSM::stateId <Step1_BT>(), Event::UPDATE },
			{ FSM::stateId <Step1_2>(),	 Event::UPDATE },

			{ FSM::regionId<Step1_BT>(), Event::TASK_SUCCESS, FSM::stateId<Step1_2>() },
			{ FSM::stateId <Step1_BT>(), Event::CHANGE,		  FSM::stateId<Step1_3>() },

			{ FSM::stateId <Step1_2>(),	 Event::EXIT_GUARD  },

			{ FSM::stateId <Step1_3>(),	 Event::ENTRY_GUARD },

			{ FSM::stateId <Step1_2>(),	 Event::EXIT   },

			{ FSM::stateId <Step1_3>(),	 Event::ENTER  },
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
			{ FSM::stateId <Apex>(),	 Event::UPDATE },
			{ FSM::stateId <Planned>(),	 Event::UPDATE },
			{ FSM::stateId <Step1_BT>(), Event::UPDATE },
			{ FSM::stateId <Step1_3>(),	 Event::UPDATE },

			{ FSM::regionId<Step1_BT>(), Event::TASK_SUCCESS,   FSM::stateId<Step1_3>()	 },
			{ FSM::stateId <Step1_BT>(), Event::PLAN_SUCCEEDED },
			{ FSM::regionId<Step1_BT>(), Event::TASK_SUCCESS,   FSM::stateId<Step1_BT>() },
			{ FSM::regionId<Step1_BT>(), Event::PLAN_SUCCESS },
			{ FSM::stateId <Planned>(),	 Event::CHANGE,			FSM::stateId<Hybrid>()	 },

			{ FSM::stateId <Step1_BT>(), Event::EXIT_GUARD  },
			{ FSM::stateId <Step1_3>(),	 Event::EXIT_GUARD  },

			{ FSM::stateId <Hybrid>(),	 Event::ENTRY_GUARD },
			{ FSM::stateId <Step2L_P>(), Event::ENTRY_GUARD },
			{ FSM::stateId <Step2L_1>(), Event::ENTRY_GUARD },
			{ FSM::stateId <Step2R_P>(), Event::ENTRY_GUARD },
			{ FSM::stateId <Step2R_1>(), Event::ENTRY_GUARD },

			{ FSM::stateId <Step1_3>(),	 Event::EXIT   },
			{ FSM::stateId <Step1_BT>(), Event::EXIT   },

			{ FSM::stateId <Hybrid>(),	 Event::ENTER  },
			{ FSM::stateId <Step2L_P>(), Event::ENTER  },
			{ FSM::stateId <Step2L_1>(), Event::ENTER  },
			{ FSM::stateId <Step2R_P>(), Event::ENTER  },
			{ FSM::stateId <Step2R_1>(), Event::ENTER  },
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
			{ FSM::stateId <Apex>(),	 Event::UPDATE },
			{ FSM::stateId <Planned>(),	 Event::UPDATE },
			{ FSM::stateId <Hybrid>(),	 Event::UPDATE },
			{ FSM::stateId <Step2L_P>(), Event::UPDATE },
			{ FSM::stateId <Step2L_1>(), Event::UPDATE },

			{ FSM::regionId<Step2L_P>(), Event::TASK_SUCCESS, FSM::stateId<Step2L_1>() },

			{ FSM::stateId <Step2R_P>(), Event::UPDATE },
			{ FSM::stateId <Step2R_1>(), Event::UPDATE },

			{ FSM::regionId<Step2R_P>(), Event::TASK_SUCCESS, FSM::stateId<Step2R_1>() },

			{ FSM::stateId <Hybrid>(),	 Event::CHANGE,		  FSM::stateId<Step2L_2>() },
			{ FSM::stateId <Hybrid>(),	 Event::CHANGE,		  FSM::stateId<Step2R_2>() },

			{ FSM::stateId <Step2L_1>(), Event::EXIT_GUARD  },
			{ FSM::stateId <Step2R_1>(), Event::EXIT_GUARD  },

			{ FSM::stateId <Step2L_2>(), Event::ENTRY_GUARD },
			{ FSM::stateId <Step2R_2>(), Event::ENTRY_GUARD },

			{ FSM::stateId <Step2L_1>(), Event::EXIT   },
			{ FSM::stateId <Step2L_2>(), Event::ENTER  },
			{ FSM::stateId <Step2R_1>(), Event::EXIT   },
			{ FSM::stateId <Step2R_2>(), Event::ENTER  },
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

	machine.update();
	{
		const Events reference = {
			{ FSM::stateId <Apex>(),	   Event::UPDATE },
			{ FSM::stateId <Planned>(),	   Event::UPDATE },
			{ FSM::stateId <Hybrid>(),	   Event::UPDATE },
			{ FSM::stateId <Step2L_P>(),   Event::UPDATE },
			{ FSM::stateId <Step2L_2>(),   Event::UPDATE },

			{ FSM::regionId<Step2L_P>(),   Event::TASK_FAILURE,	  FSM::stateId<Step2L_2>() },

			{ FSM::stateId <Step2R_P>(),   Event::UPDATE },
			{ FSM::stateId <Step2R_2>(),   Event::UPDATE },

			{ FSM::regionId<Step2R_P>(),   Event::TASK_FAILURE,	  FSM::stateId<Step2R_2>() },

			{ FSM::stateId <Hybrid>(),	   Event::PLAN_FAILED },
			{ FSM::regionId<Hybrid>(),	   Event::TASK_SUCCESS,	  FSM::stateId<Hybrid>()   },
			{ FSM::regionId<Hybrid>(),	   Event::PLAN_SUCCESS },

			{ FSM::stateId <Planned>(),	   Event::CHANGE,		  FSM::stateId<Terminal>() },

			{ FSM::stateId <Hybrid>(),	   Event::EXIT_GUARD  },
			{ FSM::stateId <Step2L_P>(),   Event::EXIT_GUARD  },
			{ FSM::stateId <Step2L_2>(),   Event::EXIT_GUARD  },
			{ FSM::stateId <Step2R_P>(),   Event::EXIT_GUARD  },
			{ FSM::stateId <Step2R_2>(),   Event::EXIT_GUARD  },

			{ FSM::stateId <Terminal>(),   Event::ENTRY_GUARD },
			{ FSM::stateId <Terminal_L>(), Event::ENTRY_GUARD },
			{ FSM::stateId <Terminal_R>(), Event::ENTRY_GUARD },

			{ FSM::stateId <Step2L_2>(),   Event::EXIT	},
			{ FSM::stateId <Step2L_P>(),   Event::EXIT	},
			{ FSM::stateId <Step2R_2>(),   Event::EXIT	},
			{ FSM::stateId <Step2R_P>(),   Event::EXIT	},
			{ FSM::stateId <Hybrid>(),	   Event::EXIT	},

			{ FSM::stateId <Terminal>(),   Event::ENTER	},
			{ FSM::stateId <Terminal_L>(), Event::ENTER	},
			{ FSM::stateId <Terminal_R>(), Event::ENTER	},
		};
		logger.assertSequence(reference);

		const Types active = {
			FSM::stateId<Apex>(),
			FSM::stateId<Planned>(),
			FSM::stateId<Terminal>(),
			FSM::stateId<Terminal_L>(),
			FSM::stateId<Terminal_R>(),
		};
		assertActive(machine, all, active);

		const Types resumable = {
			FSM::stateId<Step1_3>(),
			FSM::stateId<Hybrid>(),
			FSM::stateId<Step2L_P>(),
			FSM::stateId<Step2L_2>(),
			FSM::stateId<Step2R_P>(),
			FSM::stateId<Step2R_2>(),
		};
		assertResumable(machine, all, resumable);
	}

	//machine.update();
	//{
	//	const Events reference = {
	//	};
	//	logger.assertSequence(reference);

	//	const Types active = {
	//	};
	//	assertActive(machine, all, active);

	//	const Types resumable = {
	//	};
	//	assertResumable(machine, all, resumable);
	//}
}

////////////////////////////////////////////////////////////////////////////////
