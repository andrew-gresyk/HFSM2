// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include <hfsm2/machine_dev.hpp>

#include <doctest/doctest.h>

namespace test_contexts {

//------------------------------------------------------------------------------

using Context = int;

////////////////////////////////////////////////////////////////////////////////

namespace value_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace reference_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context&>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace pointer_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context*>>;

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

TEST_CASE("FSM.Contexts") {
	Context primary	  =   7;
	Context secondary = -39;

	// context is a value
	{
		using Instance = value_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context>::value, "");

		//Instance defaultConstructed;

		const Instance constant{primary};
		REQUIRE(constant.context() == primary);

		Instance machine{primary};
		REQUIRE(machine.context() == primary);

		machine.setContext(secondary);
		REQUIRE(machine.context() == secondary);

		machine.setContext(8);
		REQUIRE(machine.context() == 8);
	}

	// context is a reference
	{
		using Instance = reference_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context&>::value, "");

		const Instance constant{primary};
		REQUIRE(constant.context() == primary);

		Instance machine{primary};
		REQUIRE(machine.context() == primary);

		machine.setContext(secondary);
		REQUIRE(machine.context() == secondary);
	}

	// context is a pointer
	{
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
	}

	// empty context
	{
		no_context::FSM::Instance defaultConstructed;
		static_assert(std::is_same<decltype(defaultConstructed.context()), hfsm2::EmptyContext&>::value, "");
	}
}

////////////////////////////////////////////////////////////////////////////////

}
