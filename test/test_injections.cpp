// HFSM2 (flat state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#define HFSM2_DISABLE_TYPEINDEX
#include <hfsm2/machine.hpp>

#include "tools.hpp"

namespace test_injections {

//------------------------------------------------------------------------------

struct Event {
	enum class Type {
		PRE_ENTRY_GUARD,

		PRE_ENTER,
		PRE_REENTER,

		PRE_UPDATE,
		POST_UPDATE,

		PRE_REVERSE_UPDATE,
		POST_REVERSE_UPDATE,

		PRE_REACT,
		POST_REACT,

		PRE_REVERSE_REACT,
		POST_REVERSE_REACT,

		PRE_EXIT_GUARD,

		POST_EXIT,

		COUNT
	};

	Event(const hfsm2::StateID origin_,
		  const Type type_) noexcept
		: origin{origin_}
		, type{type_}
	{}

	Event(const Type type_) noexcept
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
struct InjectionT
	: FSM::Injection
{
	void preEntryGuard	  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_ENTRY_GUARD);		}

	void preEnter		  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_ENTER);			}
	void preReenter		  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_REENTER);			}

	void preUpdate		  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_UPDATE);			}
	void postUpdate		  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::POST_UPDATE);			}

	void preReverseUpdate (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_REVERSE_UPDATE);	}
	void postReverseUpdate(Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::POST_REVERSE_UPDATE);	}

	void preReact		  (const Event&,
						   Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_REACT);			}

	void postReact		  (const Event&,
						   Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::POST_REACT);			}

	void preReverseReact  (const Event&,
						   Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_REVERSE_REACT);	}

	void postReverseReact (const Event&,
						   Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::POST_REVERSE_REACT);	}

	void preExitGuard	  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::PRE_EXIT_GUARD);		}

	void postExit		  (Events& events)	{ events.emplace_back(FSM::stateId<T>(), Event::Type::POST_EXIT);			}
};

//------------------------------------------------------------------------------

struct R
	: FSM::StateT<InjectionT<R>>
{
	void entryGuard(GuardControl& control) {
		control.changeTo<B>();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A
	: FSM::StateT<InjectionT<A>>
{};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B
	: FSM::StateT<InjectionT<B>>
{};

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<R>(),
	FSM::stateId<A>(),
	FSM::stateId<B>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Manual Activation") {
	Events events;

	{
		FSM::Instance machine{events};

		assertSequence(events, {});

		machine.enter();
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::PRE_ENTRY_GUARD },
			{ FSM::stateId<A>(), Event::Type::PRE_ENTRY_GUARD },
			{ FSM::stateId<R>(), Event::Type::PRE_ENTRY_GUARD },
			{ FSM::stateId<B>(), Event::Type::PRE_ENTRY_GUARD },
			{ FSM::stateId<R>(), Event::Type::PRE_ENTER  },
			{ FSM::stateId<B>(), Event::Type::PRE_ENTER  },
		});

		assertActive(machine, all, {
			FSM::stateId<R>(),
			FSM::stateId<B>(),
		});
		assertResumable(machine, all, {});

		machine.update();
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::PRE_UPDATE  },
			{ FSM::stateId<R>(), Event::Type::POST_UPDATE  },
			{ FSM::stateId<B>(), Event::Type::PRE_UPDATE  },
			{ FSM::stateId<B>(), Event::Type::POST_UPDATE  },
		});

		machine.reverseUpdate();
		assertSequence(events, {
			{ FSM::stateId<B>(), Event::Type::PRE_REVERSE_UPDATE },
			{ FSM::stateId<B>(), Event::Type::POST_REVERSE_UPDATE },
			{ FSM::stateId<R>(), Event::Type::PRE_REVERSE_UPDATE },
			{ FSM::stateId<R>(), Event::Type::POST_REVERSE_UPDATE },
		});

		machine.react(Event::Type::COUNT);
		assertSequence(events, {
			{ FSM::stateId<R>(), Event::Type::PRE_REACT  },
			{ FSM::stateId<R>(), Event::Type::POST_REACT  },
			{ FSM::stateId<B>(), Event::Type::PRE_REACT  },
			{ FSM::stateId<B>(), Event::Type::POST_REACT  },
		});

		machine.reverseReact(Event::Type::COUNT);
		assertSequence(events, {
			{ FSM::stateId<B>(), Event::Type::PRE_REVERSE_REACT },
			{ FSM::stateId<B>(), Event::Type::POST_REVERSE_REACT },
			{ FSM::stateId<R>(), Event::Type::PRE_REVERSE_REACT },
			{ FSM::stateId<R>(), Event::Type::POST_REVERSE_REACT },
		});

		machine.exit();
		assertSequence(events, {
			{ FSM::stateId<B>(), Event::Type::POST_EXIT  },
			{ FSM::stateId<R>(), Event::Type::POST_EXIT  },
		});
	}

	assertSequence(events, {});
}

////////////////////////////////////////////////////////////////////////////////

}
