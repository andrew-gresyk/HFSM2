#define HFSM_ENABLE_ALIGNMENT_CHEKS
#include <hfsm/machine.hpp>

#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace Event {
	enum Enum : unsigned {
		GUARD,
		ENTER,
		UPDATE,
		REACT_REQUEST,
		REACT,
		EXIT,

		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};
};

//------------------------------------------------------------------------------

struct Status {
	std::type_index state;
	Event::Enum func;

	inline bool operator == (const Status& reference) const {
		return func == reference.func && state == reference.state;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
Status status(Event::Enum event) {
	using Type = T;

	return Status{ std::type_index(typeid(Type)), event };
}

//------------------------------------------------------------------------------

struct Context {
	using History = std::vector<Status>;

	template <unsigned NCapacity>
	void assertHistory(const Status (&reference)[NCapacity]) {
		const unsigned historySize = (unsigned)history.size();
		const unsigned referenceSize = hfsm::detail::count(reference);
		assert(historySize == referenceSize);

		for (unsigned i = 0; i < std::min(historySize, referenceSize); ++i) {
			HSFM_IF_ASSERT(const auto h = history[i]);
			HSFM_IF_ASSERT(const auto r = reference[i]);
			assert(h == r);
		}

		history.clear();
	}

	float deltaTime = 0.0f;

	History history;
};
using M = hfsm::Machine<Context>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(A),
					S(A_1),
					M::Composite<S(A_2),
						S(A_2_1),
						S(A_2_2)
					>
				>,
				M::Orthogonal<S(B),
					M::Composite<S(B_1),
						S(B_1_1),
						S(B_1_2)
					>,
					M::Composite<S(B_2),
						S(B_2_1),
						S(B_2_2)
					>
				>
			>;

#undef S

////////////////////////////////////////////////////////////////////////////////

class Timed
	: public FSM::Bare
{
public:
	void preEnter(Context&)		{ _elapsed = 0.0f;			}
	void preUpdate(Context& _)	{ _elapsed += _.deltaTime;	}

	auto elapsed() const		{ return _elapsed;			}

private:
	float _elapsed;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class Tracked
	: public FSM::Bare
{
public:
	void preEnter(Context&) {
		++_entryCount;
		_currentUpdateCount = 0;
	}

	void preUpdate(Context&) {
		++_currentUpdateCount;
		++_totalUpdateCount;
	}

	unsigned entryCount() const			{ return _entryCount;			}
	unsigned currentUpdateCount() const { return _currentUpdateCount;	}
	unsigned totalUpdateCount() const	{ return _totalUpdateCount;		}

private:
	unsigned _entryCount = 0;
	unsigned _currentUpdateCount = 0;
	unsigned _totalUpdateCount = 0;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Action {};

template <typename T>
struct HistoryBase
	: FSM::Bare
{
	void preGuard(Context& _) const {
		_.history.push_back(Status{ typeid(T), Event::GUARD });
	}

	void preEnter(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::ENTER });
	}

	void preUpdate(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::UPDATE });
	}

	void preReact(const Action&, Context& _) {
		_.history.push_back(Status{ typeid(T), Event::REACT_REQUEST });
	}

	void postExit(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::EXIT });
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using Base = FSM::BaseT<Tracked, Timed, HistoryBase<T>>;

//------------------------------------------------------------------------------

template <typename T>
void
changeTo(FSM::TransitionControl& control, Context::History& history) {
	control.template changeTo<T>();
	history.push_back(Status{ typeid(T), Event::RESTART });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
resume(FSM::TransitionControl& control, Context::History& history) {
	control.template resume<T>();
	history.push_back(Status{ typeid(T), Event::RESUME });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
schedule(FSM::TransitionControl& control, Context::History& history) {
	control.template schedule<T>();
	history.push_back(Status{ typeid(T), Event::SCHEDULE });
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Reacting
	: Base<T>
{
	void react(const Action&, FSM::TransitionControl& control) {
		control._().history.push_back(Status{ typeid(T), Event::REACT });
	}
};

////////////////////////////////////////////////////////////////////////////////

struct A : Reacting<A> {};

//------------------------------------------------------------------------------

struct A_2;

struct A_1
	: Base<A_1>
{
	void update(TransitionControl& control) {
		changeTo<A_2>(control, control._().history);
	}
};

//------------------------------------------------------------------------------

struct B;
struct B_2_2;

struct A_2
	: Base<A_2>
{
	void update(TransitionControl& control) {
		switch (entryCount()) {
		case 1:
			changeTo<B_2_2>(control, control._().history);
			break;

		case 2:
			resume<B>(control, control._().history);
			break;
		}
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1 : Reacting<A_2_1> {};
struct A_2_2 : Reacting<A_2_2> {};

//------------------------------------------------------------------------------

struct B : Reacting<B> {};

struct B_1 : Base<B_1> {};
struct B_1_1 : Base<B_1_1> {};
struct B_1_2 : Base<B_1_2> {};

struct B_2 : Base<B_2> {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: Base<B_2_1>
{
	void guard(TransitionControl& control) {
		resume<B_2_2>(control, control._().history);
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: Base<B_2_2>
{
	void guard(TransitionControl& control) {
		if (entryCount() == 2)
			control.resume<A>();
	}

	void update(TransitionControl& control) {
		switch (totalUpdateCount()) {
		case 1:
			resume<A>(control, control._().history);
			break;

		case 2:
			changeTo<B>(control, control._().history);
			break;

		case 3:
			schedule<A_2_2>(control, control._().history);
			resume<A>(control, control._().history);
			break;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////

void deprecated() {
	Context _;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine(_);

		static_assert(machine.DEEP_WIDTH  ==  2, "");
		static_assert(machine.STATE_COUNT == 13, "");
		static_assert(machine.FORK_COUNT  ==  6, "");
		static_assert(machine.PRONG_COUNT == 10, "");

		const Status created[] = {
			status<A>(Event::ENTER),
			status<A_1>(Event::ENTER),
		};
		_.assertHistory(created);

		assert( machine.isActive<A>());
		assert( machine.isActive<A_1>());
		assert(!machine.isActive<A_2>());
		assert(!machine.isActive<A_2_1>());
		assert(!machine.isActive<A_2_2>());
		assert(!machine.isActive<B>());
		assert(!machine.isActive<B_1>());
		assert(!machine.isActive<B_1_1>());
		assert(!machine.isActive<B_1_2>());
		assert(!machine.isActive<B_2>());
		assert(!machine.isActive<B_2_1>());
		assert(!machine.isActive<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted1[] = {
			status<A>(Event::REACT_REQUEST),
			status<A>(Event::REACT),
			status<A_1>(Event::REACT_REQUEST),
		};
		_.assertHistory(reacted1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update1[] = {
			status<A>(Event::UPDATE),
			status<A_1>(Event::UPDATE),

			status<A_2>(Event::RESTART),

			status<A_2>(Event::GUARD),
			status<A_2_1>(Event::GUARD),

			status<A_1>(Event::EXIT),

			status<A_2>(Event::ENTER),
			status<A_2_1>(Event::ENTER),
		};
		_.assertHistory(update1);

		assert(!machine.isResumable<A>());
		assert( machine.isResumable<A_1>());
		assert(!machine.isResumable<A_2>());
		assert(!machine.isResumable<A_2_1>());
		assert(!machine.isResumable<A_2_2>());
		assert(!machine.isResumable<B>());
		assert(!machine.isResumable<B_1>());
		assert(!machine.isResumable<B_1_1>());
		assert(!machine.isResumable<B_1_2>());
		assert(!machine.isResumable<B_2>());
		assert(!machine.isResumable<B_2_1>());
		assert(!machine.isResumable<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted2[] = {
			status<A>(Event::REACT_REQUEST),
			status<A>(Event::REACT),
			status<A_2>(Event::REACT_REQUEST),
			status<A_2_1>(Event::REACT_REQUEST),
			status<A_2_1>(Event::REACT),
		};
		_.assertHistory(reacted2);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update2[] = {
			status<A>(Event::UPDATE),
			status<A_2>(Event::UPDATE),
			status<B_2_2>(Event::RESTART),

			status<A_2_1>(Event::UPDATE),

			status<B>(Event::GUARD),
			status<B_1>(Event::GUARD),
			status<B_1_1>(Event::GUARD),
			status<B_2>(Event::GUARD),
			status<B_2_2>(Event::GUARD),

			status<A_2_1>(Event::EXIT),
			status<A_2>(Event::EXIT),
			status<A>(Event::EXIT),

			status<B>(Event::ENTER),
			status<B_1>(Event::ENTER),
			status<B_1_1>(Event::ENTER),
			status<B_2>(Event::ENTER),
			status<B_2_2>(Event::ENTER),
		};
		_.assertHistory(update2);

		assert(!machine.isActive<A>());
		assert(!machine.isActive<A_1>());
		assert(!machine.isActive<A_2>());
		assert(!machine.isActive<A_2_1>());
		assert(!machine.isActive<A_2_2>());
		assert( machine.isActive<B>());
		assert( machine.isActive<B_1>());
		assert( machine.isActive<B_1_1>());
		assert(!machine.isActive<B_1_2>());
		assert( machine.isActive<B_2>());
		assert(!machine.isActive<B_2_1>());
		assert( machine.isActive<B_2_2>());

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.react(Action{});

		const Status reacted3[] = {
			status<B>(Event::REACT_REQUEST),
			status<B>(Event::REACT),
			status<B_1>(Event::REACT_REQUEST),
			status<B_1_1>(Event::REACT_REQUEST),
			status<B_2>(Event::REACT_REQUEST),
			status<B_2_2>(Event::REACT_REQUEST),
		};
		_.assertHistory(reacted3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update3[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<A>(Event::RESUME),

			status<A>(Event::GUARD),
			status<A_2>(Event::GUARD),
			status<A_2_1>(Event::GUARD),

			status<B_1_1>(Event::EXIT),
			status<B_1>(Event::EXIT),
			status<B_2_2>(Event::EXIT),
			status<B_2>(Event::EXIT),
			status<B>(Event::EXIT),

			status<A>(Event::ENTER),
			status<A_2>(Event::ENTER),
			status<A_2_1>(Event::ENTER),
		};
		_.assertHistory(update3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update4[] = {
			status<A>(Event::UPDATE),
			status<A_2>(Event::UPDATE),
			status<B>(Event::RESUME),
			status<A_2_1>(Event::UPDATE),

			status<B>(Event::GUARD),
			status<B_1>(Event::GUARD),
			status<B_1_1>(Event::GUARD),
			status<B_2>(Event::GUARD),
			status<B_2_2>(Event::GUARD),

			status<A_2_1>(Event::EXIT),
			status<A_2>(Event::EXIT),
			status<A>(Event::EXIT),

			status<B>(Event::ENTER),
			status<B_1>(Event::ENTER),
			status<B_1_1>(Event::ENTER),
			status<B_2>(Event::ENTER),
			status<B_2_2>(Event::ENTER),
		};
		_.assertHistory(update4);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update5[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<B>(Event::RESTART),

			status<B_2_1>(Event::GUARD),
			status<B_2_2>(Event::RESUME),
		};
		_.assertHistory(update5);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
		const Status update6[] = {
			status<B>(Event::UPDATE),
			status<B_1>(Event::UPDATE),
			status<B_1_1>(Event::UPDATE),
			status<B_2>(Event::UPDATE),
			status<B_2_2>(Event::UPDATE),

			status<A_2_2>(Event::SCHEDULE),
			status<A>(Event::RESUME),

			status<A>(Event::GUARD),
			status<A_2>(Event::GUARD),
			status<A_2_2>(Event::GUARD),

			status<B_1_1>(Event::EXIT),
			status<B_1>(Event::EXIT),
			status<B_2_2>(Event::EXIT),
			status<B_2>(Event::EXIT),
			status<B>(Event::EXIT),

			status<A>(Event::ENTER),
			status<A_2>(Event::ENTER),
			status<A_2_2>(Event::ENTER),
		};
		_.assertHistory(update6);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	const Status destroyed[] = {
		status<A_2_2>(Event::EXIT),
		status<A_2>(Event::EXIT),
		status<A>(Event::EXIT),
	};
	_.assertHistory(destroyed);
}

////////////////////////////////////////////////////////////////////////////////
