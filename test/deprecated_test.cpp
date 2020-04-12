#define HFSM_ENABLE_ASSERT
#include <hfsm2/machine.hpp>

#include <algorithm>
#include <vector>

#include <catch2/catch.hpp>

namespace deprecated_test {

////////////////////////////////////////////////////////////////////////////////

namespace Event {
	enum Enum : unsigned {
		ENTRY_GUARD,
		ENTER,
		REENTER,
		UPDATE,
		REACT_REQUEST,
		REACT,
		EXIT_GUARD,
		EXIT,

		CHANGE,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};
}

//------------------------------------------------------------------------------

struct Status {
	std::type_index state;
	Event::Enum func;

	bool operator == (const Status& reference) const {
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

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <unsigned NCapacity>
	void assertHistory(const Status (&reference)[NCapacity]) {
		const auto historySize = (unsigned) history.size();
		const auto referenceSize = hfsm2::detail::count(reference);

		const auto size = std::min(historySize, referenceSize);

		for (unsigned i = 0; i < size; ++i) {
			const auto h = history[i];
			const auto r = reference[i];
			REQUIRE(h == r);
		}
		REQUIRE(historySize == referenceSize);

		history.clear();
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	float deltaTime = 0.0f;

	History history;
};

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

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

//------------------------------------------------------------------------------

static_assert(FSM::regionId<A>()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B>()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A>()		==  1, "");
static_assert(FSM::stateId<A_1>()	==  2, "");
static_assert(FSM::stateId<A_2>()	==  3, "");
static_assert(FSM::stateId<A_2_1>()	==  4, "");
static_assert(FSM::stateId<A_2_2>()	==  5, "");
static_assert(FSM::stateId<B>()		==  6, "");
static_assert(FSM::stateId<B_1>()	==  7, "");
static_assert(FSM::stateId<B_1_1>()	==  8, "");
static_assert(FSM::stateId<B_1_2>()	==  9, "");
static_assert(FSM::stateId<B_2>()	== 10, "");
static_assert(FSM::stateId<B_2_1>()	== 11, "");
static_assert(FSM::stateId<B_2_2>()	== 12, "");

////////////////////////////////////////////////////////////////////////////////

class Timed
	: public FSM::Injection
{
public:
	void preEnter(Context&)		{ _elapsed = 0.0f;			}
	void preUpdate(Context& _)	{ _elapsed += _.deltaTime;	} //-V669

	float elapsed() const		{ return _elapsed;			}

private:
	float _elapsed = 0.0f;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class EntryGuardTracked
	: public FSM::Injection
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
	: FSM::Injection
{
	void preEntryGuard(Context& _) const {
		_.history.push_back(Status{ typeid(T), Event::ENTRY_GUARD });
	}

	void preEnter(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::ENTER });
	}

	void preReenter(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::REENTER });
	}

	void preUpdate(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::UPDATE });
	}

	void preReact(const Action&, Context& _) {
		_.history.push_back(Status{ typeid(T), Event::REACT_REQUEST });
	}

	void preExitGuard(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::EXIT_GUARD });
	}

	void postExit(Context& _) {
		_.history.push_back(Status{ typeid(T), Event::EXIT });
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
using State = FSM::StateT<EntryGuardTracked, Timed, HistoryBase<T>>;

//------------------------------------------------------------------------------

template <typename T>
void
changeTo(FSM::FullControl& control) {
	control.template changeTo<T>();
	control._().history.push_back(Status{ typeid(T), Event::CHANGE });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
restart(FSM::FullControl& control) {
	control.template restart<T>();
	control._().history.push_back(Status{ typeid(T), Event::RESTART });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
resume(FSM::FullControl& control) {
	control.template resume<T>();
	control._().history.push_back(Status{ typeid(T), Event::RESUME });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
schedule(FSM::FullControl& control) {
	control.template schedule<T>();
	control._().history.push_back(Status{ typeid(T), Event::SCHEDULE });
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Reacting
	: State<T>
{
	using FullControl = typename State<T>::FullControl;

	void react(const Action&, FullControl& control) {
		control._().history.push_back(Status{ typeid(T), Event::REACT });
	}
};

////////////////////////////////////////////////////////////////////////////////

struct A : Reacting<A> {};

//------------------------------------------------------------------------------

struct A_1
	: State<A_1>
{
	void update(FullControl& control) {
		changeTo<A_2>(control);
	}
};

//------------------------------------------------------------------------------

struct A_2
	: State<A_2>
{
	void update(FullControl& control) {
		switch (entryCount()) {
		case 1:
			changeTo<B_2_2>(control);
			break;

		case 2:
			resume<B>(control);
			break;
		}
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1 : Reacting<A_2_1> {};
struct A_2_2 : Reacting<A_2_2> {};

//------------------------------------------------------------------------------

struct B	 : Reacting<B> {};

struct B_1	 : State<B_1> {};
struct B_1_1 : State<B_1_1> {};
struct B_1_2 : State<B_1_2> {};

struct B_2	 : State<B_2> {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: State<B_2_1>
{
	void entryGuard(GuardControl& control) {
		control.cancelPendingTransitions();
		resume<B_2_2>(control);
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: State<B_2_2>
{
	void entryGuard(GuardControl& control) {
		if (entryCount() == 3) {
			control.cancelPendingTransitions();
			resume<A>(control);
		}
	}

	void update(FullControl& control) {
		switch (totalUpdateCount()) {
		case 1:
			resume<A>(control);
			break;

		case 2:
			changeTo<B>(control);
			break;

		case 3:
			schedule<A_2_2>(control);
			resume<A>(control);
			break;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Deprecated", "[machine]") {
	Context _;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_};

		const Status created[] = {
			status<A>(Event::ENTRY_GUARD),
			status<A_1>(Event::ENTRY_GUARD),

			status<A>(Event::ENTER),
			status<A_1>(Event::ENTER),
		};
		_.assertHistory(created);

		REQUIRE( machine.isActive<A>());
		REQUIRE( machine.isActive<A_1>());
		REQUIRE(!machine.isActive<A_2>());
		REQUIRE(!machine.isActive<A_2_1>());
		REQUIRE(!machine.isActive<A_2_2>());
		REQUIRE(!machine.isActive<B>());
		REQUIRE(!machine.isActive<B_1>());
		REQUIRE(!machine.isActive<B_1_1>());
		REQUIRE(!machine.isActive<B_1_2>());
		REQUIRE(!machine.isActive<B_2>());
		REQUIRE(!machine.isActive<B_2_1>());
		REQUIRE(!machine.isActive<B_2_2>());

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

			status<A_2>(Event::CHANGE),

			status<A_1>(Event::EXIT_GUARD),

			status<A_2>(Event::ENTRY_GUARD),
			status<A_2_1>(Event::ENTRY_GUARD),

			status<A_1>(Event::EXIT),

			status<A_2>(Event::ENTER),
			status<A_2_1>(Event::ENTER),
		};
		_.assertHistory(update1);

		REQUIRE(!machine.isResumable<A>());
		REQUIRE( machine.isResumable<A_1>());
		REQUIRE(!machine.isResumable<A_2>());
		REQUIRE(!machine.isResumable<A_2_1>());
		REQUIRE(!machine.isResumable<A_2_2>());
		REQUIRE(!machine.isResumable<B>());
		REQUIRE(!machine.isResumable<B_1>());
		REQUIRE(!machine.isResumable<B_1_1>());
		REQUIRE(!machine.isResumable<B_1_2>());
		REQUIRE(!machine.isResumable<B_2>());
		REQUIRE(!machine.isResumable<B_2_1>());
		REQUIRE(!machine.isResumable<B_2_2>());

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
			status<B_2_2>(Event::CHANGE),

			status<A_2_1>(Event::UPDATE),

			status<A>(Event::EXIT_GUARD),
			status<A_2>(Event::EXIT_GUARD),
			status<A_2_1>(Event::EXIT_GUARD),

			status<B>(Event::ENTRY_GUARD),
			status<B_1>(Event::ENTRY_GUARD),
			status<B_1_1>(Event::ENTRY_GUARD),
			status<B_2>(Event::ENTRY_GUARD),
			status<B_2_2>(Event::ENTRY_GUARD),

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

		REQUIRE(!machine.isActive<A>());
		REQUIRE(!machine.isActive<A_1>());
		REQUIRE(!machine.isActive<A_2>());
		REQUIRE(!machine.isActive<A_2_1>());
		REQUIRE(!machine.isActive<A_2_2>());
		REQUIRE( machine.isActive<B>());
		REQUIRE( machine.isActive<B_1>());
		REQUIRE( machine.isActive<B_1_1>());
		REQUIRE(!machine.isActive<B_1_2>());
		REQUIRE( machine.isActive<B_2>());
		REQUIRE(!machine.isActive<B_2_1>());
		REQUIRE( machine.isActive<B_2_2>());

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

			status<B>(Event::EXIT_GUARD),
			status<B_1>(Event::EXIT_GUARD),
			status<B_1_1>(Event::EXIT_GUARD),
			status<B_2>(Event::EXIT_GUARD),
			status<B_2_2>(Event::EXIT_GUARD),

			status<A>(Event::ENTRY_GUARD),
			status<A_2>(Event::ENTRY_GUARD),
			status<A_2_1>(Event::ENTRY_GUARD),

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

			status<A>(Event::EXIT_GUARD),
			status<A_2>(Event::EXIT_GUARD),
			status<A_2_1>(Event::EXIT_GUARD),

			status<B>(Event::ENTRY_GUARD),
			status<B_1>(Event::ENTRY_GUARD),
			status<B_1_1>(Event::ENTRY_GUARD),
			status<B_2>(Event::ENTRY_GUARD),
			status<B_2_2>(Event::ENTRY_GUARD),

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

			status<B>(Event::CHANGE),

			status<B>(Event::EXIT_GUARD),
			status<B_1>(Event::EXIT_GUARD),
			status<B_1_1>(Event::EXIT_GUARD),
			status<B_2>(Event::EXIT_GUARD),
			status<B_2_2>(Event::EXIT_GUARD),

			status<B>(Event::ENTRY_GUARD),
			status<B_1>(Event::ENTRY_GUARD),
			status<B_1_1>(Event::ENTRY_GUARD),
			status<B_2>(Event::ENTRY_GUARD),
			status<B_2_1>(Event::ENTRY_GUARD),

			status<B_2_2>(Event::RESUME),

			status<B_2_2>(Event::EXIT_GUARD),
			status<B_2_2>(Event::ENTRY_GUARD),

			status<B_2_2>(Event::REENTER),
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

			status<B>(Event::EXIT_GUARD),
			status<B_1>(Event::EXIT_GUARD),
			status<B_1_1>(Event::EXIT_GUARD),
			status<B_2>(Event::EXIT_GUARD),
			status<B_2_2>(Event::EXIT_GUARD),

			status<A>(Event::ENTRY_GUARD),
			status<A_2>(Event::ENTRY_GUARD),
			status<A_2_2>(Event::ENTRY_GUARD),

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

}
