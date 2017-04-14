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

template <typename T>
Status status(Event::Enum event) {
	using Type = T;

	return Status{ event, std::type_index(typeid(Type)) };
}

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

using Machine = hfsm::Machine<Context>;

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

////////////////////////////////////////////////////////////////////////////////

struct A : Base<A> {};

//------------------------------------------------------------------------------

struct A_2;

struct A_1
	: Base<A_1>
{
	void transition(Control& control, Context& _, const Time) const {
		control.changeTo<A_2>();
		_.history.push_back(status<A_2>(Event::Restart));
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
			control.changeTo<B_2_2>();
			_.history.push_back(status<B_2_2>(Event::Restart));
			break;

		case 2:
			control.resume<B>();
			_.history.push_back(status<B>(Event::Resume));
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
		control.resume<B_2_2>();
		_.history.push_back(status<B_2_2>(Event::Resume));
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
			control.resume<A>();
			_.history.push_back(status<A>(Event::Resume));
			break;

		case 2:
			control.changeTo<B>();
			_.history.push_back(status<B>(Event::Restart));
			break;

		case 3:
			control.schedule<A_2_2>();
			_.history.push_back(status<A_2_2>(Event::Schedule));

			control.resume<A>();
			_.history.push_back(status<A>(Event::Resume));
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

		static_assert(machine.DeepWidth  == 2,  "");
		static_assert(machine.StateCount == 13, "");
		static_assert(machine.ForkCount  == 5,  "");
		static_assert(machine.ProngCount == 10, "");

		const Status created[] = {
			status<A>(Event::Enter),
			status<A_1>(Event::Enter),
		};
		_.assertHistory(created);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update1[] = {
			status<A>(Event::Update),
			status<A>(Event::Transition),
			status<A_1>(Event::Update),
			status<A_1>(Event::Transition),

			status<A_2>(Event::Restart),

			status<A_2>(Event::Substitute),
			status<A_2_1>(Event::Substitute),

			status<A_1>(Event::Leave),

			status<A_2>(Event::Enter),
			status<A_2_1>(Event::Enter),
		};
		_.assertHistory(update1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update2[] = {
			status<A>(Event::Update),
			status<A>(Event::Transition),
			status<A_2>(Event::Update),
			status<A_2>(Event::Transition),
			status<B_2_2>(Event::Restart),

			status<A_2_1>(Event::Update),

			status<B>(Event::Substitute),
			status<B_1>(Event::Substitute),
			status<B_1_1>(Event::Substitute),
			status<B_2>(Event::Substitute),
			status<B_2_2>(Event::Substitute),

			status<A_2_1>(Event::Leave),
			status<A_2>(Event::Leave),
			status<A>(Event::Leave),

			status<B>(Event::Enter),
			status<B_1>(Event::Enter),
			status<B_1_1>(Event::Enter),
			status<B_2>(Event::Enter),
			status<B_2_2>(Event::Enter),
		};
		_.assertHistory(update2);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update3[] = {
			status<B>(Event::Update),
			status<B>(Event::Transition),
			status<B_1>(Event::Update),
			status<B_1>(Event::Transition),
			status<B_1_1>(Event::Update),
			status<B_1_1>(Event::Transition),
			status<B_2>(Event::Update),
			status<B_2>(Event::Transition),
			status<B_2_2>(Event::Update),
			status<B_2_2>(Event::Transition),

			status<A>(Event::Resume),

			status<A>(Event::Substitute),
			status<A_2>(Event::Substitute),
			status<A_2_1>(Event::Substitute),

			status<B_1_1>(Event::Leave),
			status<B_1>(Event::Leave),
			status<B_2_2>(Event::Leave),
			status<B_2>(Event::Leave),
			status<B>(Event::Leave),

			status<A>(Event::Enter),
			status<A_2>(Event::Enter),
			status<A_2_1>(Event::Enter),
		};
		_.assertHistory(update3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update4[] = {
			status<A>(Event::Update),
			status<A>(Event::Transition),
			status<A_2>(Event::Update),
			status<A_2>(Event::Transition),
			status<B>(Event::Resume),
			status<A_2_1>(Event::Update),

			status<B>(Event::Substitute),
			status<B_1>(Event::Substitute),
			status<B_1_1>(Event::Substitute),
			status<B_2>(Event::Substitute),
			status<B_2_2>(Event::Substitute),

			status<A_2_1>(Event::Leave),
			status<A_2>(Event::Leave),
			status<A>(Event::Leave),

			status<B>(Event::Enter),
			status<B_1>(Event::Enter),
			status<B_1_1>(Event::Enter),
			status<B_2>(Event::Enter),
			status<B_2_2>(Event::Enter),
		};
		_.assertHistory(update4);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update5[] = {
			status<B>(Event::Update),
			status<B>(Event::Transition),
			status<B_1>(Event::Update),
			status<B_1>(Event::Transition),
			status<B_1_1>(Event::Update),
			status<B_1_1>(Event::Transition),
			status<B_2>(Event::Update),
			status<B_2>(Event::Transition),
			status<B_2_2>(Event::Update),
			status<B_2_2>(Event::Transition),

			status<B>(Event::Restart),

			status<B_2_1>(Event::Substitute),

			status<B_2_2>(Event::Resume),
		};
		_.assertHistory(update5);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update(0.0f);
		const Status update6[] = {
			status<B>(Event::Update),
			status<B>(Event::Transition),
			status<B_1>(Event::Update),
			status<B_1>(Event::Transition),
			status<B_1_1>(Event::Update),
			status<B_1_1>(Event::Transition),
			status<B_2>(Event::Update),
			status<B_2>(Event::Transition),
			status<B_2_2>(Event::Update),
			status<B_2_2>(Event::Transition),

			status<A_2_2>(Event::Schedule),
			status<A>(Event::Resume),

			status<A>(Event::Substitute),
			status<A_2>(Event::Substitute),
			status<A_2_2>(Event::Substitute),

			status<B_1_1>(Event::Leave),
			status<B_1>(Event::Leave),
			status<B_2_2>(Event::Leave),
			status<B_2>(Event::Leave),
			status<B>(Event::Leave),

			status<A>(Event::Enter),
			status<A_2>(Event::Enter),
			status<A_2_2>(Event::Enter),
		};
		_.assertHistory(update6);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	const Status destroyed[] = {
		status<A_2_2>(Event::Leave),
		status<A_2>(Event::Leave),
		status<A>(Event::Leave),
	};
	_.assertHistory(destroyed);
}

////////////////////////////////////////////////////////////////////////////////
