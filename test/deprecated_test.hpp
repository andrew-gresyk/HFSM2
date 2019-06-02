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
};

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

	float deltaTime = 0.0f;

	History history;
};
using M = hfsm2::MachineT<Context>;

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

static constexpr auto REGION = FSM::regionId<A>();

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
	void preUpdate(Context& _)	{ _elapsed += _.deltaTime;	}

	float elapsed() const		{ return _elapsed;			}

private:
	float _elapsed;
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
changeTo(FSM::FullControl& control, Context::History& history) {
	control.template changeTo<T>();
	history.push_back(Status{ typeid(T), Event::CHANGE });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
restart(FSM::FullControl& control, Context::History& history) {
	control.template restart<T>();
	history.push_back(Status{ typeid(T), Event::RESTART });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
resume(FSM::FullControl& control, Context::History& history) {
	control.template resume<T>();
	history.push_back(Status{ typeid(T), Event::RESUME });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
void
schedule(FSM::FullControl& control, Context::History& history) {
	control.template schedule<T>();
	history.push_back(Status{ typeid(T), Event::SCHEDULE });
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Reacting
	: State<T>
{
	void react(const Action&, FSM::FullControl& control) {
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
		changeTo<A_2>(control, control._().history);
	}
};

//------------------------------------------------------------------------------

struct A_2
	: State<A_2>
{
	void update(FullControl& control) {
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
		resume<B_2_2>(control, control._().history);
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_2
	: State<B_2_2>
{
	void entryGuard(GuardControl& control) {
		if (entryCount() == 3) {
			control.cancelPendingTransitions();
			resume<A>(control, control._().history);
		}
	}

	void update(FullControl& control) {
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

static_assert(FSM::Instance::STATE_COUNT == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT ==  5, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT ==  1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 10, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
