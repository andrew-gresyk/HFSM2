#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_dynamic {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

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

static_assert(FSM::regionId<A  >()	==  1, "");
static_assert(FSM::regionId<A_2>()	==  2, "");
static_assert(FSM::regionId<B  >()	==  3, "");
static_assert(FSM::regionId<B_1>()	==  4, "");
static_assert(FSM::regionId<B_2>()	==  5, "");

static_assert(FSM::stateId<A    >() ==  1, "");
static_assert(FSM::stateId<A_1  >() ==  2, "");
static_assert(FSM::stateId<A_2  >() ==  3, "");
static_assert(FSM::stateId<A_2_1>() ==  4, "");
static_assert(FSM::stateId<A_2_2>() ==  5, "");
static_assert(FSM::stateId<B    >() ==  6, "");
static_assert(FSM::stateId<B_1  >() ==  7, "");
static_assert(FSM::stateId<B_1_1>() ==  8, "");
static_assert(FSM::stateId<B_1_2>() ==  9, "");
static_assert(FSM::stateId<B_2  >() == 10, "");
static_assert(FSM::stateId<B_2_1>() == 11, "");
static_assert(FSM::stateId<B_2_2>() == 12, "");

////////////////////////////////////////////////////////////////////////////////

struct A     : FSM::DynamicState	{ static int c; A    () { ++c; } ~A    () { --c; } };	int A    ::c = 0;
struct A_1   : FSM::DynamicState	{ static int c; A_1  () { ++c; } ~A_1  () { --c; } };	int A_1  ::c = 0;
struct A_2   : FSM::DynamicState	{ static int c; A_2  () { ++c; } ~A_2  () { --c; } };	int A_2  ::c = 0;
struct A_2_1 : FSM::DynamicState	{ static int c; A_2_1() { ++c; } ~A_2_1() { --c; } };	int A_2_1::c = 0;
struct A_2_2 : FSM::DynamicState	{ static int c; A_2_2() { ++c; } ~A_2_2() { --c; } };	int A_2_2::c = 0;
struct B     : FSM::DynamicState	{ static int c; B    () { ++c; } ~B    () { --c; } };	int B    ::c = 0;
struct B_1   : FSM::DynamicState	{ static int c; B_1  () { ++c; } ~B_1  () { --c; } };	int B_1  ::c = 0;
struct B_1_1 : FSM::DynamicState	{ static int c; B_1_1() { ++c; } ~B_1_1() { --c; } };	int B_1_1::c = 0;
struct B_1_2 : FSM::DynamicState	{ static int c; B_1_2() { ++c; } ~B_1_2() { --c; } };	int B_1_2::c = 0;
struct B_2   : FSM::DynamicState	{ static int c; B_2  () { ++c; } ~B_2  () { --c; } };	int B_2  ::c = 0;
struct B_2_1 : FSM::DynamicState	{ static int c; B_2_1() { ++c; } ~B_2_1() { --c; } };	int B_2_1::c = 0;
struct B_2_2 : FSM::DynamicState	{ static int c; B_2_2() { ++c; } ~B_2_2() { --c; } };	int B_2_2::c = 0;

//------------------------------------------------------------------------------

}

namespace hfsm2 {
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <>
	struct Guard<test_dynamic::A_2> {
		static void execute(test_dynamic::FSM::GuardControl& control) {
			control.cancelPendingTransitions();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

namespace test_dynamic {

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<A    >(),
	FSM::stateId<A_1  >(),
	FSM::stateId<A_2  >(),
	FSM::stateId<A_2_1>(),
	FSM::stateId<A_2_2>(),
	FSM::stateId<B    >(),
	FSM::stateId<B_1  >(),
	FSM::stateId<B_1_1>(),
	FSM::stateId<B_1_2>(),
	FSM::stateId<B_2  >(),
	FSM::stateId<B_2_1>(),
	FSM::stateId<B_2_2>(),
};

//------------------------------------------------------------------------------

template <typename... Ts>
using TypeList = hfsm2::detail::ITL_<Ts...>;

//------------------------------------------------------------------------------

template <typename TState,
		  typename TAactiveList,
		  bool = TAactiveList::template contains<TState>()>
struct CheckActive {
	CheckActive() { REQUIRE(TState::c == 0); } //-V521
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TState,
		  typename TAactiveList>
struct CheckActive<TState, TAactiveList, true> {
	CheckActive() { REQUIRE(TState::c == 1); } //-V521
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TAactiveList,
		  bool BContains>
struct CheckActive<void, TAactiveList, BContains> {
	CheckActive() {}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TAactiveList>
struct CheckActive<void, TAactiveList, true> {
	CheckActive() {}
};

//------------------------------------------------------------------------------

template <typename, typename>
struct CheckActives;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... TAllStates, typename... TActiveStates>
struct CheckActives<
	TypeList<TAllStates...>,
	TypeList<TActiveStates...>
>
	: CheckActive<TAllStates, TypeList<TActiveStates...>>...
{
	CheckActives()
		: CheckActive<TAllStates, TypeList<TActiveStates...>>()...
	{}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename... TActiveStates>
void checkActive() {
	CheckActives<FSM::StateList, TypeList<TActiveStates...>>{};
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Dynamic", "[machine]") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},	 Event::ENTRY_GUARD },
				{ FSM::stateId<A    >(), Event::ENTRY_GUARD },
				{ FSM::stateId<A_1  >(), Event::ENTRY_GUARD },

				{ hfsm2::StateID{0},	 Event::CONSTRUCT },
				{ FSM::stateId<A    >(), Event::CONSTRUCT },
				{ FSM::stateId<A_1  >(), Event::CONSTRUCT },

				{ hfsm2::StateID{0},	 Event::ENTER },
				{ FSM::stateId<A    >(), Event::ENTER },
				{ FSM::stateId<A_1  >(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
			});

			checkActive<A, A_1>();

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.changeTo<A_2>();
		machine.update();
		{
			logger.assertSequence({
				{						 Event::CHANGE,		FSM::stateId<A_2  >() },

				{ hfsm2::StateID{0},	 Event::UPDATE },
				{ FSM::stateId<A    >(), Event::UPDATE },
				{ FSM::stateId<A_1  >(), Event::UPDATE },

				{ FSM::stateId<A_1  >(), Event::EXIT_GUARD },
				{ FSM::stateId<A_2  >(), Event::ENTRY_GUARD },
				{ FSM::stateId<A_2  >(), Event::CANCEL_PENDING },
			});

			checkActive<A, A_1>();

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<A_1  >(), Event::EXIT },
		{ FSM::stateId<A    >(), Event::EXIT },
		{ hfsm2::StateID{0},	 Event::EXIT },

		{ FSM::stateId<A_1  >(), Event::DESTRUCT },
		{ FSM::stateId<A    >(), Event::DESTRUCT },
		{ hfsm2::StateID{0},	 Event::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
