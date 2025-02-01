// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

// Issue reported in https://discord.com/channels/755015945269018695/759099438353350696/1334175930188300348

#define HFSM2_ENABLE_PLANS
#include <hfsm2/machine.hpp>

#include <doctest/doctest.h>

using M = hfsm2::Machine;

struct Manual;
struct Automatic;
struct Idling;
struct WaitInput;
struct WorkOnBox;
struct BoxA;
struct BoxB;
struct BoxC;

enum class Input { retry, stop };

struct BoxPresent { };

// clang-format off
using FSM = M::PeerRoot<
				Manual,
				M::Composite<Automatic,
					Idling,
					WaitInput,
					M::Composite<WorkOnBox,
						BoxA,
						BoxB,
						BoxC
					>
				>
			>;
// clang-format on

struct Manual : FSM::State { };

struct Automatic : FSM::State { };

struct Idling : FSM::State {
	using FSM::State::react;

	void react(const BoxPresent&, EventControl& c)
	{
		auto plan = c.plan<WorkOnBox>();

		plan.change<BoxA, BoxB>();
		plan.change<BoxB, BoxC>();

		c.changeTo<WorkOnBox>();
	}
};

struct WaitInput : FSM::State {
	using FSM::State::react;

	void react(const Input& e, EventControl& c)
	{
		if (e == Input::retry)
			c.resume<WorkOnBox>();
		else
			c.changeTo<Manual>();
	}
};

struct WorkOnBox : FSM::State {
	void planSucceeded(FullControl& c)
	{
		c.changeTo<Idling>();
	}

	void planFailed(FullControl& c)
	{
		c.changeTo<WaitInput>();
	}
};

struct BoxA : FSM::State {
	void update(FullControl& c)
	{
		c.succeed();
	}
};

struct BoxB : FSM::State {
	bool should_fail = true;

	void update(FullControl& c)
	{
		if (should_fail)
			c.fail();
		else
			c.succeed();

		should_fail = !should_fail;
	}
};

struct BoxC : FSM::State {
	void update(FullControl& c)
	{
		c.succeed();
	}
};

TEST_CASE("FSM.Reported.Resuming Plan") {
	FSM::Instance machine;

	machine.immediateChangeTo<Automatic>();
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<Idling>());

	machine.react(BoxPresent {});
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<WorkOnBox>());
	REQUIRE(machine.isActive<BoxA>());

	machine.update();
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<WorkOnBox>());
	REQUIRE(machine.isActive<BoxB>());

	machine.update();
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<WaitInput>());

	machine.react(Input::retry);
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<WorkOnBox>());
	REQUIRE(machine.isActive<BoxB>());

	machine.update();
	REQUIRE(machine.isActive<Automatic>());
	REQUIRE(machine.isActive<WorkOnBox>());
	REQUIRE(machine.isActive<BoxC>());
}
