#include "hfsm/machine.hpp"

#include <algorithm>
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

		COUNT
	};
};

struct Status {
	Event::Enum func;
	hfsm::detail::TypeInfo state;

	inline bool operator == (const Status& reference) const {
		return func == reference.func && state == reference.state;
	}
};

//··············································································

struct Context {
	using History = std::vector<Status>;

	template <unsigned TCapacity>
	void assertHistory(const Status(&reference)[TCapacity]) {
		const unsigned historySize = (unsigned)history.size();
		const unsigned referenceSize = hfsm::detail::count(reference);
		assert(historySize == referenceSize);

		for (unsigned i = 0; i < std::min(historySize, referenceSize); ++i) {
			HSFM_ASSERT_ONLY(const auto h = history[i]);
			HSFM_ASSERT_ONLY(const auto r = reference[i]);
			assert(h == r);
		}

		history.clear();
	}

	History history;
};

//··············································································

using Machine = hfsm::detail::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct HistoryBase
	: Machine::Bare
{
	void preSubstitute(Context& _, const Time) const {
		_.history.push_back(Status{ Event::Substitute, hfsm::detail::TypeInfo::get<T>() });
	}

	void preEnter(Context& _, const Time) {
		_.history.push_back(Status{ Event::Enter, hfsm::detail::TypeInfo::get<T>() });
	}

	void preUpdate(Context& _, const Time) {
		_.history.push_back(Status{ Event::Update, hfsm::detail::TypeInfo::get<T>() });
	}

	void preTransition(Context& _, const Time) const {
		_.history.push_back(Status{ Event::Transition, hfsm::detail::TypeInfo::get<T>() });
	}

	void postLeave(Context& _, const Time) {
		_.history.push_back(Status{ Event::Leave, hfsm::detail::TypeInfo::get<T>() });
	}
};

template <typename T>
using Base = Machine::BaseT<Machine::Tracked, Machine::Timed, HistoryBase<T>>;

//------------------------------------------------------------------------------

template <typename T, typename TControl>
void
changeTo(TControl& control, Context::History& history) {
	control.changeTo<T>();
	history.push_back(Status{ Event::Restart, typeid(T) });
}

//··············································································

template <typename T, typename TControl>
void
resume(TControl& control, Context::History& history) {
	control.resume<T>();
	history.push_back(Status{ Event::Resume, typeid(T) });
}

//··············································································

template <typename T, typename TControl>
void
schedule(TControl& control, Context::History& history) {
	control.schedule<T>();
	history.push_back(Status{ Event::Schedule, typeid(T) });
}

////////////////////////////////////////////////////////////////////////////////

struct A : Base<A> {};

//------------------------------------------------------------------------------

struct A_2;

struct A_1
	: Base<A_1>
{
	void transition(Control& control, Context& _, const Time) const {
		changeTo<A_2>(control, _.history);
	}
};

//------------------------------------------------------------------------------

struct B;
struct B_2_2;

struct A_2
	: Base<A_2>
{
	void transition(Control& control, Context& _, const Time) const {
		switch (entryCount()) {
		case 1:
			changeTo<B_2_2>(control, _.history);
			break;

		case 2:
			resume<B>(control, _.history);
			break;
		}
	}
};

//··············································································

struct A_2_1 : Base<A_2_1> {};
struct A_2_2 : Base<A_2_2> {};

//------------------------------------------------------------------------------

struct B : Base<B> {};

struct B_1 : Base<B_1> {};
struct B_1_1 : Base<B_1_1> {};
struct B_1_2 : Base<B_1_2> {};

struct B_2 : Base<B_2> {};

//··············································································

struct B_2_1
	: Base<B_2_1>
{
	void substitute(Control& control, Context& _, const Time) const {
		resume<B_2_2>(control, _.history);
	}
};

//··············································································

struct B_2_2
	: Base<B_2_2>
{
	void substitute(Control& control, Context&, const Time) const {
		if (entryCount() == 2)
			control.resume<A>();
	}

	void transition(Control& control, Context& _, const Time) const {
		switch (totalUpdateCount()) {
		case 1:
			resume<A>(control, _.history);
			break;

		case 2:
			changeTo<B>(control, _.history);
			break;

		case 3:
			schedule<A_2_2>(control, _.history);
			resume<A>(control, _.history);
			break;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////

void
main(int /*argc*/, char* /*argv*/[]) {
	Context _;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		Machine::CompositeRoot<
			Machine::Composite<A,
				Machine::State<A_1>,
				Machine::Composite<A_2,
					Machine::State<A_2_1>,
					Machine::State<A_2_2>
				>
			>,
			Machine::Orthogonal<B,
				Machine::Composite<B_1,
					Machine::State<B_1_1>,
					Machine::State<B_1_2>
				>,
				Machine::Composite<B_2,
					Machine::State<B_2_1>,
					Machine::State<B_2_2>
				>
			>
		> machine(_);

		static_assert(machine.DeepWidth == 2);
		static_assert(machine.StateCount == 13);
		static_assert(machine.ForkCount == 5);
		static_assert(machine.ProngCount == 10);

		const Status created[] = {
			Status{ Event::Enter, typeid(A) },
			Status{ Event::Enter, typeid(A_1) },
		};
		_.assertHistory(created);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update1[] = {
			Status{ Event::Update, typeid(A) },
			Status{ Event::Transition, typeid(A) },
			Status{ Event::Update, typeid(A_1) },
			Status{ Event::Transition, typeid(A_1) },

			Status{ Event::Restart, typeid(A_2) },

			Status{ Event::Substitute, typeid(A_2) },
			Status{ Event::Substitute, typeid(A_2_1) },

			Status{ Event::Leave, typeid(A_1) },

			Status{ Event::Enter, typeid(A_2) },
			Status{ Event::Enter, typeid(A_2_1) },
		};
		_.assertHistory(update1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update2[] = {
			Status{ Event::Update, typeid(A) },
			Status{ Event::Transition, typeid(A) },
			Status{ Event::Update, typeid(A_2) },
			Status{ Event::Transition, typeid(A_2) },
			Status{ Event::Restart, typeid(B_2_2) },

			Status{ Event::Update, typeid(A_2_1) },

			Status{ Event::Substitute, typeid(B) },
			Status{ Event::Substitute, typeid(B_1) },
			Status{ Event::Substitute, typeid(B_1_1) },
			Status{ Event::Substitute, typeid(B_2) },
			Status{ Event::Substitute, typeid(B_2_2) },

			Status{ Event::Leave, typeid(A_2_1) },
			Status{ Event::Leave, typeid(A_2) },
			Status{ Event::Leave, typeid(A) },

			Status{ Event::Enter, typeid(B) },
			Status{ Event::Enter, typeid(B_1) },
			Status{ Event::Enter, typeid(B_1_1) },
			Status{ Event::Enter, typeid(B_2) },
			Status{ Event::Enter, typeid(B_2_2) },
		};
		_.assertHistory(update2);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update3[] = {
			Status{ Event::Update, typeid(B) },
			Status{ Event::Transition, typeid(B) },
			Status{ Event::Update, typeid(B_1) },
			Status{ Event::Transition, typeid(B_1) },
			Status{ Event::Update, typeid(B_1_1) },
			Status{ Event::Transition, typeid(B_1_1) },
			Status{ Event::Update, typeid(B_2) },
			Status{ Event::Transition, typeid(B_2) },
			Status{ Event::Update, typeid(B_2_2) },
			Status{ Event::Transition, typeid(B_2_2) },

			Status{ Event::Resume, typeid(A) },

			Status{ Event::Substitute, typeid(A) },
			Status{ Event::Substitute, typeid(A_2) },
			Status{ Event::Substitute, typeid(A_2_1) },

			Status{ Event::Leave, typeid(B_1_1) },
			Status{ Event::Leave, typeid(B_1) },
			Status{ Event::Leave, typeid(B_2_2) },
			Status{ Event::Leave, typeid(B_2) },
			Status{ Event::Leave, typeid(B) },

			Status{ Event::Enter, typeid(A) },
			Status{ Event::Enter, typeid(A_2) },
			Status{ Event::Enter, typeid(A_2_1) },
		};
		_.assertHistory(update3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update4[] = {
			Status{ Event::Update, typeid(A) },
			Status{ Event::Transition, typeid(A) },
			Status{ Event::Update, typeid(A_2) },
			Status{ Event::Transition, typeid(A_2) },
			Status{ Event::Resume, typeid(B) },
			Status{ Event::Update, typeid(A_2_1) },

			Status{ Event::Substitute, typeid(B) },
			Status{ Event::Substitute, typeid(B_1) },
			Status{ Event::Substitute, typeid(B_1_1) },
			Status{ Event::Substitute, typeid(B_2) },
			Status{ Event::Substitute, typeid(B_2_2) },

			Status{ Event::Leave, typeid(A_2_1) },
			Status{ Event::Leave, typeid(A_2) },
			Status{ Event::Leave, typeid(A) },

			Status{ Event::Enter, typeid(B) },
			Status{ Event::Enter, typeid(B_1) },
			Status{ Event::Enter, typeid(B_1_1) },
			Status{ Event::Enter, typeid(B_2) },
			Status{ Event::Enter, typeid(B_2_2) },
		};
		_.assertHistory(update4);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update5[] = {
			Status{ Event::Update, typeid(B) },
			Status{ Event::Transition, typeid(B) },
			Status{ Event::Update, typeid(B_1) },
			Status{ Event::Transition, typeid(B_1) },
			Status{ Event::Update, typeid(B_1_1) },
			Status{ Event::Transition, typeid(B_1_1) },
			Status{ Event::Update, typeid(B_2) },
			Status{ Event::Transition, typeid(B_2) },
			Status{ Event::Update, typeid(B_2_2) },
			Status{ Event::Transition, typeid(B_2_2) },

			Status{ Event::Restart, typeid(B) },

			Status{ Event::Substitute, typeid(B_2_1) },

			Status{ Event::Resume, typeid(B_2_2) },
		};
		_.assertHistory(update5);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update6[] = {
			Status{ Event::Update, typeid(B) },
			Status{ Event::Transition, typeid(B) },
			Status{ Event::Update, typeid(B_1) },
			Status{ Event::Transition, typeid(B_1) },
			Status{ Event::Update, typeid(B_1_1) },
			Status{ Event::Transition, typeid(B_1_1) },
			Status{ Event::Update, typeid(B_2) },
			Status{ Event::Transition, typeid(B_2) },
			Status{ Event::Update, typeid(B_2_2) },
			Status{ Event::Transition, typeid(B_2_2) },

			Status{ Event::Schedule, typeid(A_2_2) },
			Status{ Event::Resume, typeid(A) },

			Status{ Event::Substitute, typeid(A) },
			Status{ Event::Substitute, typeid(A_2) },
			Status{ Event::Substitute, typeid(A_2_2) },

			Status{ Event::Leave, typeid(B_1_1) },
			Status{ Event::Leave, typeid(B_1) },
			Status{ Event::Leave, typeid(B_2_2) },
			Status{ Event::Leave, typeid(B_2) },
			Status{ Event::Leave, typeid(B) },

			Status{ Event::Enter, typeid(A) },
			Status{ Event::Enter, typeid(A_2) },
			Status{ Event::Enter, typeid(A_2_2) },
		};
		_.assertHistory(update6);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	const Status destroyed[] = {
		Status{ Event::Leave, typeid(A_2_2) },
		Status{ Event::Leave, typeid(A_2) },
		Status{ Event::Leave, typeid(A) },
	};
	_.assertHistory(destroyed);
}

////////////////////////////////////////////////////////////////////////////////
