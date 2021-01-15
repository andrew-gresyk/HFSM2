#define HFSM2_ENABLE_UTILITY_THEORY
#include <hfsm2/machine.hpp>
#include <catch2/catch.hpp>

namespace test_contexts_random {

//------------------------------------------------------------------------------

using Context = int;

struct DummyRNG {
	inline float next() noexcept { return 0.0f; }
};

////////////////////////////////////////////////////////////////////////////////

namespace value_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>::RandomT<DummyRNG>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace reference_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context&>::RandomT<DummyRNG>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace pointer_context {

using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context*>::RandomT<DummyRNG>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

namespace no_context {

using M = hfsm2::MachineT<hfsm2::Config::RandomT<DummyRNG>>;

struct A;
struct B;

using FSM = M::PeerRoot<A, B>;

struct A : FSM::State {};
struct B : FSM::State {};

}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Contexts Random", "[machine]") {
	Context primary	  =   7;
	Context secondary = -39;
	DummyRNG rng;

	// context is a value
	{
		using Instance = value_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context>::value, "");

		//Instance defaultConstructed;

		const Instance constant{primary, rng};
		REQUIRE(constant.context() == primary);

		Instance machine{primary, rng};
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

		const Instance constant{primary, rng};
		REQUIRE(constant.context() == primary);

		Instance machine{primary, rng};
		REQUIRE(machine.context() == primary);

		machine.setContext(secondary);
		REQUIRE(machine.context() == secondary);
	}

	// context is a pointer
	{
		using Instance = pointer_context::FSM::Instance;
		static_assert(std::is_same<Instance::Context, Context*>::value, "");

		pointer_context::FSM::Instance defaultConstructed(nullptr, rng);
		REQUIRE(defaultConstructed.context() == nullptr);

		const Instance constant{&primary, rng};
		REQUIRE(constant.context() == &primary);

		Instance machine{&primary, rng};
		REQUIRE(machine.context() == &primary);

		machine.setContext(&secondary);
		REQUIRE(machine.context() == &secondary);
	}

	// empty context
	{
		no_context::FSM::Instance defaultConstructed(rng);
		static_assert(std::is_same<decltype(defaultConstructed.context()), hfsm2::EmptyContext&>::value, "");
	}
}

////////////////////////////////////////////////////////////////////////////////

}
