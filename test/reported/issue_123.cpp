// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported in https://github.com/andrew-gresyk/HFSM2/issues/123

#define HFSM2_ENABLE_LOG_INTERFACE
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "../tools.hpp"

using namespace test_tools;

namespace issue_123 {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

struct CloseEvent {};

//------------------------------------------------------------------------------

using FSM = M::PeerRoot<
				struct Off,
				M::Composite<struct Work,
					struct On
				>
			>;

////////////////////////////////////////////////////////////////////////////////

struct Off
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.changeTo<On>();
	}
};

struct Work
	: FSM::State
{};

struct On
	: FSM::State
{
	void react(const CloseEvent& UNUSED(event), EventControl& control) {
		control.changeTo<Off>();
	}
};

////////////////////////////////////////////////////////////////////////////////

void step0(FSM::Instance& machine, Logger& logger) {
	logger.assertSequence({
		{ hfsm2::ROOT_ID      , Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Off >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Off >(), Event::Type::CHANGE, FSM::stateId<On  >() },

		{ hfsm2::ROOT_ID      , Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Work>(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<On  >(), Event::Type::ENTRY_GUARD },

		{ hfsm2::ROOT_ID      , Event::Type::ENTER },
		{ FSM::stateId<Work>(), Event::Type::ENTER },
		{ FSM::stateId<On  >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID      ,
		FSM::stateId<Work>(),
		FSM::stateId<On  >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step1(FSM::Instance& machine, Logger& logger) {
	machine.react(CloseEvent{});

	logger.assertSequence({
		{ hfsm2::ROOT_ID      , Event::Type::PRE_REACT },
		{ FSM::stateId<Work>(), Event::Type::PRE_REACT },
		{ FSM::stateId<On  >(), Event::Type::PRE_REACT },

		{ hfsm2::ROOT_ID      , Event::Type::REACT },
		{ FSM::stateId<Work>(), Event::Type::REACT },
		{ FSM::stateId<On  >(), Event::Type::REACT },

		{ FSM::stateId<On  >(), Event::Type::CHANGE, FSM::stateId<Off >() },

		{ FSM::stateId<On  >(), Event::Type::POST_REACT },
		{ FSM::stateId<Work>(), Event::Type::POST_REACT },
		{ hfsm2::ROOT_ID      , Event::Type::POST_REACT },

		{ FSM::stateId<On  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Work>(), Event::Type::EXIT_GUARD },

		{ FSM::stateId<Off >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Off >(), Event::Type::CHANGE, FSM::stateId<On  >() },

		{ FSM::stateId<On  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Work>(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Work>(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<On  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<On  >(), Event::Type::EXIT },
		{ FSM::stateId<Work>(), Event::Type::EXIT },
		{ FSM::stateId<Work>(), Event::Type::ENTER },
		{ FSM::stateId<On  >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID      ,
		FSM::stateId<Work>(),
		FSM::stateId<On  >(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine, Logger& logger) {
	machine.react(CloseEvent{});

	logger.assertSequence({
		{ hfsm2::ROOT_ID      , Event::Type::PRE_REACT },
		{ FSM::stateId<Work>(), Event::Type::PRE_REACT },
		{ FSM::stateId<On  >(), Event::Type::PRE_REACT },

		{ hfsm2::ROOT_ID      , Event::Type::REACT },
		{ FSM::stateId<Work>(), Event::Type::REACT },
		{ FSM::stateId<On  >(), Event::Type::REACT },

		{ FSM::stateId<On  >(), Event::Type::CHANGE, FSM::stateId<Off >() },

		{ FSM::stateId<On  >(), Event::Type::POST_REACT },
		{ FSM::stateId<Work>(), Event::Type::POST_REACT },
		{ hfsm2::ROOT_ID      , Event::Type::POST_REACT },

		{ FSM::stateId<On  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<Work>(), Event::Type::EXIT_GUARD },

		{ FSM::stateId<Off >(), Event::Type::ENTRY_GUARD },
		{ FSM::stateId<Off >(), Event::Type::CHANGE, FSM::stateId<On  >() },

		{ FSM::stateId<On  >(), Event::Type::EXIT_GUARD },
		{ FSM::stateId<On  >(), Event::Type::ENTRY_GUARD },

		{ FSM::stateId<On  >(), Event::Type::EXIT },
		{ FSM::stateId<On  >(), Event::Type::ENTER },
	});

	assertActive(machine, {
		hfsm2::ROOT_ID      ,
		FSM::stateId<Work>(),
		FSM::stateId<On  >(),
	});
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Reported.Issue_123") {
	Logger logger;

	{
		FSM::Instance machine{&logger};

		step0(machine, logger);
		step1(machine, logger);
	}

	logger.assertSequence({
		{ FSM::stateId<On  >(), Event::Type::EXIT },
		{ FSM::stateId<Work>(), Event::Type::EXIT },
		{ hfsm2::ROOT_ID      , Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
