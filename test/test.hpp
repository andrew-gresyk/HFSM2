#pragma once

#include "../hfsm/machine.hpp"

#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace Event {
	enum Enum : unsigned {
		Substitute,
		Enter,
		Update,
		Transition,
		Leave,

		Restart,
		Resume,
		Schedule,

		Apply,

		COUNT
	};
};

//------------------------------------------------------------------------------
//	single FSM event record for the test

struct Status {
	Event::Enum func;
	hfsm::detail::TypeInfo state;

	inline bool operator == (const Status& reference) const {
		return func == reference.func && state == reference.state;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
Status status(Event::Enum event) {
	using Type = T;

	return Status{event, std::type_index(typeid(Type))};
}

//------------------------------------------------------------------------------
//	machine context - data interface between the FSM and the code outside it

struct Context {
	using History = std::vector<Status>;

	template <unsigned TCapacity>
	void assertHistory(const Status(&reference)[TCapacity]);

	History history;
};
using Machine = hfsm::Machine<Context>;

//------------------------------------------------------------------------------
//	state injection for automagic event recording 

template <typename T>
struct HistoryBase
	: Machine::Bare		// base injection to bring Machine::Context and Machine::Time
						// inside HistoryBase scope for convenience
{
	void preSubstitute(Context& _, const Time) const {
		_.history.push_back(Status{Event::Substitute, hfsm::detail::TypeInfo::get<T>()});
	}

	void preEnter(Context& _, const Time) {
		_.history.push_back(Status{Event::Enter, hfsm::detail::TypeInfo::get<T>()});
	}

	void preUpdate(Context& _, const Time) {
		_.history.push_back(Status{Event::Update, hfsm::detail::TypeInfo::get<T>()});
	}

	void preTransition(Context& _, const Time) const {
		_.history.push_back(Status{Event::Transition, hfsm::detail::TypeInfo::get<T>()});
	}

	void postLeave(Context& _, const Time) {
		_.history.push_back(Status{Event::Leave, hfsm::detail::TypeInfo::get<T>()});
	}
};

//------------------------------------------------------------------------------

template <typename T>
using Base = Machine::BaseT<Machine::Tracked, Machine::Timed, HistoryBase<T>>;

////////////////////////////////////////////////////////////////////////////////

#include "test.inl"
