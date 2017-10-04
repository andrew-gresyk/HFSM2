#include "test.hpp"

////////////////////////////////////////////////////////////////////////////////
//	static interface for event reaction using visit()

struct Reacting {
	Reacting(const hfsm::detail::TypeInfo type_)
		: type(type_)
	{}

	void react(Context& _) {
		_.history.push_back(Status{ Event::Apply, type });
	}

	hfsm::detail::TypeInfo type;
};

void dummy(Reacting&) {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	typed helper for Reacting

template <typename T>
struct ReactingT
	: Reacting
{
	ReactingT()
		: Reacting(hfsm::detail::TypeInfo::get<T>())
	{}
};

////////////////////////////////////////////////////////////////////////////////
//	user states

struct A
	: Base<A>
	, ReactingT<A>
{};

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct A_2_1 : Base<A_2_1>, ReactingT<A_2_1> {};
struct A_2_2 : Base<A_2_2>, ReactingT<A_2_2> {};

//------------------------------------------------------------------------------

struct B : Base<B>, ReactingT<B> {};

struct B_1 : Base<B_1> {};
struct B_1_1 : Base<B_1_1> {};
struct B_1_2 : Base<B_1_2> {};

struct B_2 : Base<B_2> {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct B_2_1
	: Base<B_2_1>
{
	void substitute(Control& control, Context& _, const Time) const {
		resume<B_2_2>(control, _.history);
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

int
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

		machine.visit(dummy);

		static_assert(machine.DeepWidth  ==  2, "");
		static_assert(machine.StateCount == 13, "");
		static_assert(machine.ForkCount  ==  5, "");
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

		machine.visit([&_](Reacting& interface) {
			interface.react(_);
		});

		const Status applied1[] = {
			status<A>(Event::Apply),
		};
		_.assertHistory(applied1);

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

		machine.visit([&_](Reacting& interface) {
			interface.react(_);
		});

		const Status applied2[] = {
			status<A>(Event::Apply),
			status<A_2_1>(Event::Apply),
		};
		_.assertHistory(applied2);

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

		machine.visit([&_](Reacting& interface) {
			interface.react(_);
		});

		const Status applied3[] = {
			status<B>(Event::Apply),
		};
		_.assertHistory(applied3);

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

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
