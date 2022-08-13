// HFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#define HFSM2_DISABLE_TYPEINDEX
#include "tools.hpp"

namespace test_ancestors {

//------------------------------------------------------------------------------

struct Event {
	enum class Type {
		ENTRY_GUARD,

		ENTER,
		REENTER,

		PRE_UPDATE,
		UPDATE,
		POST_UPDATE,

		PRE_REACT,
		REACT,
		POST_REACT,

		EXIT_GUARD,

		EXIT,

		COUNT
	};

	Event(const hfsm2::StateID origin_,
		  const Type type_) noexcept
		: origin{origin_}
		, type{type_}
	{}

	Event(const Type type_ = Type::COUNT) noexcept
		: type{type_}
	{}

	hfsm2::StateID origin = hfsm2::INVALID_STATE_ID;
	Type type;
};

using Events = std::vector<Event>;

//------------------------------------------------------------------------------

void
assertSequence(Events& history,
			   const Events& reference) noexcept
{
	const auto count = std::max(history.size(), reference.size());

	for (unsigned i = 0; i < count; ++i) {
		REQUIRE(i < history.size()); //-V521
		REQUIRE(i < reference.size()); //-V521

		if (i < history.size() &&
			i < reference.size())
		{
			REQUIRE(history[i].type	  == reference[i].type); //-V521
			REQUIRE(history[i].origin == reference[i].origin); //-V521
		}
	}

	history.clear();
}

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::ContextT<Events&>
					::ManualActivation;

using M = hfsm2::MachineT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(R),
				S(A),
				S(B)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<R>() == 0, "");
static_assert(FSM::stateId<A>() == 1, "");
static_assert(FSM::stateId<B>() == 2, "");

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct AncestorT
	: FSM::State
{
	void entryGuard		  (GuardControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::ENTRY_GUARD);	REQUIRE(control.stateId() == stateId<T>());	}

	void enter			  ( PlanControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::ENTER);		REQUIRE(control.stateId() == stateId<T>());	}
	void reenter		  ( PlanControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::REENTER);		REQUIRE(control.stateId() == stateId<T>());	}

	void preUpdate		  ( FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::PRE_UPDATE);	REQUIRE(control.stateId() == stateId<T>());	}
	void update			  ( FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::UPDATE);		REQUIRE(control.stateId() == stateId<T>());	}
	void postUpdate		  ( FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::POST_UPDATE);	REQUIRE(control.stateId() == stateId<T>());	}

	void preReact		  (const Event&,
							FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::PRE_REACT);	REQUIRE(control.stateId() == stateId<T>());	}

	void react			  (const Event&,
							FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::REACT);		REQUIRE(control.stateId() == stateId<T>());	}

	void postReact		  (const Event&,
							FullControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::POST_REACT);	REQUIRE(control.stateId() == stateId<T>());	}

	void exitGuard		  (GuardControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::EXIT_GUARD);	REQUIRE(control.stateId() == stateId<T>());	}

	void exit			  ( PlanControl& control)	{ control._().emplace_back(stateId<T>(), Event::Type::EXIT);		REQUIRE(control.stateId() == stateId<T>());	}
};

//------------------------------------------------------------------------------

struct R
	: FSM::StateT<AncestorT<R>>
{
	void entryGuard(GuardControl& control) {
		control.changeTo<B>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A
	: FSM::StateT<AncestorT<A>>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B
	: FSM::StateT<AncestorT<B>>
{};

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<R>(),
	FSM::stateId<A>(),
	FSM::stateId<B>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Ancestors") {
	Events events;

	{
		FSM::Instance machine{events};

		assertSequence(events, {});

		machine.enter();
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<A>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<R>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<B>(), Event::Type::ENTRY_GUARD },
			{ FSM::stateId<R>(), Event::Type::ENTER },
			{ FSM::stateId<B>(), Event::Type::ENTER },
		});

		assertActive(machine, all, {
			FSM::stateId<R>(),
			FSM::stateId<B>(),
		});
		assertResumable(machine, all, {});

		machine.update();
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::PRE_UPDATE },
			{ FSM::stateId<B>(), Event::Type::PRE_UPDATE },
			{ FSM::stateId<R>(), Event::Type::UPDATE },
			{ FSM::stateId<B>(), Event::Type::UPDATE },
			{ FSM::stateId<B>(), Event::Type::POST_UPDATE },
			{ FSM::stateId<R>(), Event::Type::POST_UPDATE },
		});

		machine.react(Event{});
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::PRE_REACT },
			{ FSM::stateId<B>(), Event::Type::PRE_REACT },
			{ FSM::stateId<R>(), Event::Type::REACT },
			{ FSM::stateId<B>(), Event::Type::REACT },
			{ FSM::stateId<B>(), Event::Type::POST_REACT },
			{ FSM::stateId<R>(), Event::Type::POST_REACT },
		});

		machine.exit();
		assertSequence(events, {
			{ FSM::stateId<B>(), Event::Type::EXIT },
			{ FSM::stateId<R>(), Event::Type::EXIT },
		});

		REQUIRE(machine.isActive() == false);
	}

	assertSequence(events, {});
}

////////////////////////////////////////////////////////////////////////////////

}
