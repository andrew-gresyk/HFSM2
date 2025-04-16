// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

using namespace test_tools;

namespace test_manual_activation {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::ManualActivation;

using M = hfsm2::MachineT<Config>;

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 1, "");
static_assert(FSM::stateId<B>() == 2, "");

////////////////////////////////////////////////////////////////////////////////

struct A
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.changeTo<B>();
	}
};

//------------------------------------------------------------------------------

struct B
	: FSM::State
{};

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<A>(),
	FSM::stateId<B>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Manual Activation") {
	Logger logger;

	{
		FSM::Instance machine{&logger};

		logger.assertSequence({});
		assertActive(machine, all, {});

		machine.enter();

		logger.assertSequence({
			{ hfsm2::StateID{0},	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<A>(),	Event::Type::ENTRY_GUARD },

			{ FSM::stateId<A>(),	Event::Type::CHANGE,	FSM::stateId<B>() },

			{ hfsm2::StateID{0},	Event::Type::ENTRY_GUARD },
			{ FSM::stateId<B>(),	Event::Type::ENTRY_GUARD },

			{ hfsm2::StateID{0},	Event::Type::ENTER },
			{ FSM::stateId<B>(),	Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<B>(),
		});

		machine.exit();

		logger.assertSequence({
			{ FSM::stateId<B>(),	Event::Type::EXIT },
			{ hfsm2::StateID{0},	Event::Type::EXIT },
		});

		assertActive(machine, all, {});
	}

	logger.assertSequence({});
}

////////////////////////////////////////////////////////////////////////////////

}
