// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_PLANS
#include "tools.hpp"

using namespace test_tools;

namespace test_react_order {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::BottomUpReactions;

using M = hfsm2::MachineT<Config>;

using Logger = LoggerT<Config>;

struct Empty {};

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::OrthogonalRoot<S(R),
				M::Composite<S(C1),
					S(I1),
					S(D1)
				>,
				M::Composite<S(C2),
					S(I2),
					S(D2)
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<R >() == 0, "");
static_assert(FSM::stateId<C1>() == 1, "");
static_assert(FSM::stateId<I1>() == 2, "");
static_assert(FSM::stateId<D1>() == 3, "");
static_assert(FSM::stateId<C2>() == 4, "");
static_assert(FSM::stateId<I2>() == 5, "");
static_assert(FSM::stateId<D2>() == 6, "");

////////////////////////////////////////////////////////////////////////////////

struct R  : FSM::State {};
struct C1 : FSM::State {};
struct I1 : FSM::State {};

struct D1
	: FSM::State
{
	void preReact(const Empty&, EventControl& control) {
		control.consumeEvent();
	}

	void react(const Empty&, EventControl& control) {
		control.consumeEvent();
	}

	void postReact(const Empty&, EventControl& control) {
		control.consumeEvent();
	}
};

struct C2 : FSM::State {};
struct I2 : FSM::State {};
struct D2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Prepend Plans") {
	Logger logger;

	WHEN("Created") {
		FSM::Instance machine{&logger};

		logger.assertSequence({});

		assertActive(machine, {
			FSM::stateId<R >(),
			FSM::stateId<C1>(),
			FSM::stateId<I1>(),
			FSM::stateId<C2>(),
			FSM::stateId<I2>(),
		});

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		THEN("Initial reactions") {
			machine.react(Empty{});

			logger.assertSequence({
				{ FSM::stateId<I1>(),	Event::Type::PRE_REACT },
				{ FSM::stateId<C1>(),	Event::Type::PRE_REACT },
				{ FSM::stateId<I2>(),	Event::Type::PRE_REACT },
				{ FSM::stateId<C2>(),	Event::Type::PRE_REACT },
				{ FSM::stateId<R >(),	Event::Type::PRE_REACT },

				{ FSM::stateId<I1>(),	Event::Type::REACT },
				{ FSM::stateId<C1>(),	Event::Type::REACT },
				{ FSM::stateId<I2>(),	Event::Type::REACT },
				{ FSM::stateId<C2>(),	Event::Type::REACT },
				{ FSM::stateId<R >(),	Event::Type::REACT },

				{ FSM::stateId<R >(),	Event::Type::POST_REACT },
				{ FSM::stateId<C1>(),	Event::Type::POST_REACT },
				{ FSM::stateId<I1>(),	Event::Type::POST_REACT },
				{ FSM::stateId<C2>(),	Event::Type::POST_REACT },
				{ FSM::stateId<I2>(),	Event::Type::POST_REACT },
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		AND_THEN("Secondary reactions") {
			machine.		 changeTo<D1>();
			machine.immediateChangeTo<D2>();

			assertActive(machine, {
				FSM::stateId<R >(),
				FSM::stateId<C1>(),
				FSM::stateId<D1>(),
				FSM::stateId<C2>(),
				FSM::stateId<D2>(),
			});

			machine.react(Empty{});

			logger.assertSequence({
				{						Event::Type::CHANGE, FSM::stateId<D1>() },
				{						Event::Type::CHANGE, FSM::stateId<D2>() },

				{ FSM::stateId<D1>(),	Event::Type::PRE_REACT },

				{ FSM::stateId<D1>(),	Event::Type::REACT },

				{ FSM::stateId<R >(),	Event::Type::POST_REACT },
				{ FSM::stateId<C1>(),	Event::Type::POST_REACT },
				{ FSM::stateId<D1>(),	Event::Type::POST_REACT },
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}
}

////////////////////////////////////////////////////////////////////////////////

}
