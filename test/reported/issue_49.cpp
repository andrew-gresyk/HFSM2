// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported in https://github.com/andrew-gresyk/HFSM2/issues/49

#define HFSM2_ENABLE_PLANS
#include <hfsm2/machine.hpp>

#include <doctest/doctest.h>

using Config = hfsm2::Config
					::TaskCapacityN<3>;

using M = hfsm2::MachineT<Config>;

using FSM = M::PeerRoot<
				struct Disabled,
				M::Composite<struct Enabled,
					M::Composite<struct Active,
						struct A,
						struct B,
						struct C,
						struct D
					>,
					struct Idle
				>
			>;

// Events
struct Enable {};
struct Disable {};

struct Disabled : FSM::State
{
	void update(FullControl& control)
	{
		control.changeTo<Enabled>();
	}

	void react(const Enable&, EventControl& control)
	{
		control.changeTo<Enabled>();
	}

	using FSM::State::react;
};

struct Enabled : FSM::State
{
	void react(const Disable&, EventControl& control)
	{
		control.changeTo<Disabled>();
	}

	using FSM::State::react;
};

struct Active : FSM::State
{
	void planSucceeded(FullControl& control)
	{
		control.changeTo<Idle>();
	}
};

struct Idle : FSM::State
{
	void update(FullControl& control)
	{
		control.changeTo<Active>();
	}
};

struct A : FSM::State
{
	void enter(PlanControl& control)
	{
		auto plan = control.plan();
		plan.clear();

		plan.change<A, B>();
		plan.change<B, C>();
		plan.change<C, D>();
	}

	void update(FullControl& control) { control.succeed(); }
};

struct B : FSM::State
{
	void update(FullControl& control) { control.succeed(); }
};

struct C : FSM::State
{
	void update(FullControl& control) { control.succeed(); }
};

struct D : FSM::State
{
	void update(FullControl& control) { control.succeed(); }
};

TEST_CASE("FSM.Reported.issue_49")
{
	FSM::Instance fsm;

	REQUIRE(fsm.isActive<Disabled>());

	fsm.update();
	REQUIRE(fsm.isActive<Enabled>());
	REQUIRE(fsm.isActive<Active>());
	REQUIRE(fsm.isActive<A>());

	fsm.update();
	REQUIRE(fsm.isActive<B>());

	fsm.update();
	REQUIRE(fsm.isActive<C>());

	fsm.update();
	REQUIRE(fsm.isActive<D>());

	fsm.update();
	REQUIRE(fsm.isActive<Idle>());

	fsm.update();
	REQUIRE(fsm.isActive<A>());

	fsm.update();
	REQUIRE(fsm.isActive<B>());

	// uncomment to make test pass
	//fsm.update();
	//REQUIRE(fsm.isActive<C>());

	fsm.react(Disable{});
	REQUIRE(fsm.isActive<Disabled>());

	fsm.react(Enable{});
	REQUIRE(fsm.isActive<Enabled>());
	REQUIRE(fsm.isActive<Active>());
	REQUIRE(fsm.isActive<A>());

	fsm.update();
	REQUIRE(fsm.isActive<B>());
}
