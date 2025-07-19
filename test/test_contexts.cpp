// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "tools.hpp"

namespace test_contexts {

//------------------------------------------------------------------------------

struct Context {
	int value;
	bool moved = false;

	constexpr Context(int value_)
		: value{value_}
		, moved{false}
	{}

	constexpr Context(const Context& other) = default;

	constexpr Context(Context&& other)
		: value{hfsm2::move(other.value)}
		, moved{true}
	{}

	Context& operator = (const Context& other) = default;
};

////////////////////////////////////////////////////////////////////////////////

namespace value_context_automatic {

using Config = hfsm2::Config
					::ContextT<Context>;

using M = hfsm2::MachineT<Config>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

//------------------------------------------------------------------------------

namespace value_context_manual {

using Config = hfsm2::Config
					::ContextT<Context>
					::ManualActivation;

using M = hfsm2::MachineT<Config>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace reference_context {

using Config = hfsm2::Config
					::ContextT<Context&>;

using M = hfsm2::MachineT<Config>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace pointer_context {

using Config = hfsm2::Config
					::ContextT<Context*>;

using M = hfsm2::MachineT<Config>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace no_context {

using M = hfsm2::Machine;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
constexpr
T
consume(T&& value) {
	return T{hfsm2::move(value)};
}

//------------------------------------------------------------------------------

constexpr
bool
operator == (const Context& lhs,
			 const Context& rhs)
{
	return lhs.value == rhs.value;
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Contexts") {
	Context primary	 {  7};
	Context secondary{-39};

	SUBCASE("context is a value (automatic)") {
		using Instance = value_context_automatic::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context>::value, "");

		//Instance defaultConstructed;

		const Instance constant{primary};
		REQUIRE(constant.context() == primary);

		Instance machine{primary};
		REQUIRE(machine.context() == primary);

		Instance recepient{hfsm2::move(Context{7})};
		REQUIRE(recepient.context().value == 7);
		REQUIRE(recepient.context().moved);

		Instance copy{machine};
		REQUIRE(copy.context().value == 7);
		REQUIRE(copy.context().moved == false);

		const Instance moved = consume(hfsm2::move(Instance{primary}));
		REQUIRE(moved.context().value == primary.value);
		REQUIRE(moved.context().moved);
	}

	SUBCASE("context is a value (manual)") {
		using Instance = value_context_manual::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context>::value, "");

		//Instance defaultConstructed;

		const Instance constant{primary};
		REQUIRE(constant.context() == primary);

		Instance machine{primary};
		REQUIRE(machine.context() == primary);

		Instance recepient{hfsm2::move(Context{7})};
		REQUIRE(recepient.context().value == 7);
		REQUIRE(recepient.context().moved);

		Instance copy{machine};
		REQUIRE(copy.context().value == 7);
		REQUIRE(copy.context().moved == false);

		const Instance moved = consume(hfsm2::move(Instance{primary}));
		REQUIRE(moved.context().value == primary.value);
		REQUIRE(moved.context().moved);
	}

	SUBCASE("context is a reference") {
		using Instance = reference_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context&>::value, "");

		const Instance constant{primary};
		REQUIRE(constant.context() == primary);

		Instance machine{primary};
		REQUIRE(machine.context() == primary);

		Instance copy{machine};
		REQUIRE(copy.context().value == primary.value);
		REQUIRE(copy.context().moved == false);

		//const Instance moved = consume(hfsm2::move(Instance{primary}));
		//REQUIRE(moved.context().value == primary.value);
		//REQUIRE(moved.context().moved);
	}

	SUBCASE("context is a pointer") {
		using Instance = pointer_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context*>::value, "");

		pointer_context::FSM::Instance defaultConstructed;
		REQUIRE(defaultConstructed.context() == nullptr);

		const Instance constant{&primary};
		REQUIRE(constant.context() == &primary);

		Instance machine{&primary};
		REQUIRE(machine.context() == &primary);

		machine.setContext(&secondary);
		REQUIRE(machine.context() == &secondary);

		Instance copy{machine};
		REQUIRE(copy.context()->value == secondary.value);
		REQUIRE(copy.context()->moved == false);
	}

	SUBCASE("empty context") {
		no_context::FSM::Instance defaultConstructed;
		static_assert(std::is_same<decltype(defaultConstructed.context()), hfsm2::EmptyContext&>::value, "");
	}
}

////////////////////////////////////////////////////////////////////////////////

}
