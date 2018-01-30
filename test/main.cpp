#define K9_MACHINE_ENABLE_STRUCTURE_REPORT
#include "../hfsm/machine.hpp"

#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace Event {
	enum Enum : unsigned {
		Substitute,
		Enter,
		Update,
		Transition,
		ReactionRequest,
		Reaction,
		Leave,

		Restart,
		Resume,
		Schedule,

		COUNT
	};
};

//------------------------------------------------------------------------------

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

	return Status{ event, std::type_index(typeid(Type)) };
}

//------------------------------------------------------------------------------

struct Context {
	using History = std::vector<Status>;

	template <unsigned TCapacity>
	void assertHistory(const Status (&reference)[TCapacity]) {
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

	float deltaTime = 0.0f;

	History history;
};
using M = hfsm::Machine<Context>;

//------------------------------------------------------------------------------

class Timed
	: public M::Bare
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
	: public M::Bare
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
	: M::Bare
{
	void preSubstitute(Context& _) const {
		_.history.push_back(Status{ Event::Substitute, hfsm::detail::TypeInfo::get<T>() });
	}

	void preEnter(Context& _) {
		_.history.push_back(Status{ Event::Enter, hfsm::detail::TypeInfo::get<T>() });
	}

	void preUpdate(Context& _) {
		_.history.push_back(Status{ Event::Update, hfsm::detail::TypeInfo::get<T>() });
	}

	void preTransition(Context& _) const {
		_.history.push_back(Status{ Event::Transition, hfsm::detail::TypeInfo::get<T>() });
	}

	void preReact(const Action&, Context& _) {
		_.history.push_back(Status{ Event::ReactionRequest, hfsm::detail::TypeInfo::get<T>() });
	}

	void postLeave(Context& _) {
		_.history.push_back(Status{ Event::Leave, hfsm::detail::TypeInfo::get<T>() });
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using Base = M::BaseT<Tracked, Timed, HistoryBase<T>>;

//------------------------------------------------------------------------------

template <typename T>
void
changeTo(M::Control& control, Context::History& history) {
	control.template changeTo<T>();
	history.push_back(Status{ Event::Restart, hfsm::detail::TypeInfo::get<T>() });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
resume(M::Control& control, Context::History& history) {
	control.template resume<T>();
	history.push_back(Status{ Event::Resume, hfsm::detail::TypeInfo::get<T>() });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
schedule(M::Control& control, Context::History& history) {
	control.template schedule<T>();
	history.push_back(Status{ Event::Schedule, hfsm::detail::TypeInfo::get<T>() });
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Reacting
	: Base<T>
{
	void react(const Action&, M::Control&, Context& _) {
		_.history.push_back(Status{ Event::Reaction, hfsm::detail::TypeInfo::get<T>() });
	}
};

////////////////////////////////////////////////////////////////////////////////

struct A : Reacting<A> {};

//------------------------------------------------------------------------------

struct A_2;

struct A_1
	: Base<A_1>
{
	void transition(Control& control, Context& _) {
		changeTo<A_2>(control, _.history);
	}
};

//------------------------------------------------------------------------------

struct B;
struct B_2_2;

struct A_2
	: Base<A_2>
{
	void transition(Control& control, Context& _) {
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
	void substitute(Control& control, Context& _) {
		resume<B_2_2>(control, _.history);
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: Base<B_2_2>
{
	void substitute(Control& control, Context&) {
		if (entryCount() == 2)
			control.resume<A>();
	}

	void transition(Control& control, Context& _) {
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

int
main(int, char*[]) {
	Context _;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		M::PeerRoot<
			M::Composite<A,
				A_1,
				M::Composite<A_2,
					A_2_1,
					A_2_2
				>
			>,
			M::Orthogonal<B,
				M::Composite<B_1,
					B_1_1,
					B_1_2
				>,
				M::Composite<B_2,
					B_2_1,
					B_2_2
				>
			>
		> machine(_);

		static_assert(machine.DeepWidth  ==  2, "");
		static_assert(machine.StateCount == 13, "");
		static_assert(machine.ForkCount  ==  6, "");
		static_assert(machine.ProngCount == 10, "");

		const Status created[] = {
			status<A>(Event::Enter),
			status<A_1>(Event::Enter),
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
			status<A>(Event::ReactionRequest),
			status<A>(Event::Reaction),
			status<A_1>(Event::ReactionRequest),
		};
		_.assertHistory(reacted1);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
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
			status<A>(Event::ReactionRequest),
			status<A>(Event::Reaction),
			status<A_2>(Event::ReactionRequest),
			status<A_2_1>(Event::ReactionRequest),
			status<A_2_1>(Event::Reaction),
		};
		_.assertHistory(reacted2);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
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
			status<B>(Event::ReactionRequest),
			status<B>(Event::Reaction),
			status<B_1>(Event::ReactionRequest),
			status<B_1_1>(Event::ReactionRequest),
			status<B_2>(Event::ReactionRequest),
			status<B_2_2>(Event::ReactionRequest),
		};
		_.assertHistory(reacted3);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.update();
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

		machine.update();
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

		machine.update();
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

		machine.update();
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

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
