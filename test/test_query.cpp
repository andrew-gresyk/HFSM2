// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include "tools.hpp"

namespace test_query {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::PeerRoot<
				S(A),
				S(B),
				S(C),
				S(D),
				S(E)
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::stateId<A>() == 1, "");
static_assert(FSM::stateId<B>() == 2, "");
static_assert(FSM::stateId<C>() == 3, "");
static_assert(FSM::stateId<D>() == 4, "");
static_assert(FSM::stateId<E>() == 5, "");

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct BaseT
	: public FSM::State
{
	void query(hfsm2::StateID& stateId_,
			   ConstControl&) const
	{
		stateId_ = stateId<T>();
	}
};

////////////////////////////////////////////////////////////////////////////////

struct A : FSM::StateT<BaseT<A>> {};
struct B : FSM::StateT<BaseT<B>> {};
struct C : FSM::StateT<BaseT<C>> {};
struct D : FSM::StateT<BaseT<D>> {};
struct E : FSM::StateT<BaseT<E>> {};

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Query") {
	FSM::Instance machine;
	hfsm2::StateID stateId;

	machine.query(stateId);
	REQUIRE(stateId == FSM::stateId<A>());

	machine.immediateChangeTo<B>();
	machine.query(stateId);
	REQUIRE(stateId == FSM::stateId<B>());

	machine.immediateChangeTo<C>();
	machine.query(stateId);
	REQUIRE(stateId == FSM::stateId<C>());

	machine.immediateChangeTo<D>();
	machine.query(stateId);
	REQUIRE(stateId == FSM::stateId<D>());

	machine.immediateChangeTo<E>();
	machine.query(stateId);
	REQUIRE(stateId == FSM::stateId<E>());
}

////////////////////////////////////////////////////////////////////////////////

}
