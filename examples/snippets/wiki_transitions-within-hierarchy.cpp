// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#include <hfsm2/machine.hpp>

#include <catch2/catch.hpp>

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.Transitions into States", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct Origin,
					struct Destination
				>;

	struct Origin      : FSM::State {};
	struct Destination : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<Origin>());

	fsm.changeTo<Destination>();
	fsm.update();
	REQUIRE(fsm.isActive<Destination>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.Transitions into Regions", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct Origin,
					M::Composite<struct Destination,
						struct SubState1,
						struct SubState2
					>
				>;

	struct Origin      : FSM::State {};
	struct Destination : FSM::State {};
	struct SubState1   : FSM::State {};
	struct SubState2   : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<Origin>());

	fsm.changeTo<Destination>();
	fsm.update();
	REQUIRE(fsm.isActive<Destination>());
	REQUIRE(fsm.isActive<SubState1>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.External Transition Interface", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct Origin,
					struct Destination
				>;

	struct Origin	   : FSM::State {};
	struct Destination : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<Origin>());

	fsm.changeTo<Destination>(); // external transition
	fsm.update();
	REQUIRE(fsm.isActive<Destination>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.Internal Transition Interface", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct Origin,
					struct Destination
				>;

	struct Origin
		: FSM::State
	{
		void update(FullControl& control) {
			control.changeTo<Destination>(); // internal transition
		}
	};

	struct Destination : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<Origin>());

	fsm.update();
	REQUIRE(fsm.isActive<Destination>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.'Restart' Transition", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct State,
					M::Composite<struct Region,
						struct Initial,
						struct Secondary
					>
				>;

	struct State     : FSM::State {};
	struct Region    : FSM::State {};
	struct Initial   : FSM::State {};
	struct Secondary : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<State>());

	fsm.changeTo<Region>();
	fsm.update();
	REQUIRE(fsm.isActive<Initial>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.'Resume' Transition", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct State,
					M::Composite<struct Region,
						struct Initial,
						struct Secondary
					>
				>;

	struct State     : FSM::State {};
	struct Region    : FSM::State {};
	struct Initial   : FSM::State {};
	struct Secondary : FSM::State {};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<State>());

	fsm.changeTo<Secondary>();
	fsm.update();
	REQUIRE(fsm.isActive<Secondary>());

	fsm.changeTo<State>();
	fsm.update();
	REQUIRE(fsm.isActive<State>());

	fsm.resume<Region>();
	fsm.update();
	REQUIRE(fsm.isActive<Secondary>());

	fsm.restart<Region>();
	fsm.update();
	REQUIRE(fsm.isActive<Initial>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.'Utilize' Transition", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct State,
					M::Composite<struct Region,
						struct LowRated,
						struct HighRated
					>
				>;

	struct State     : FSM::State {};
	struct Region    : FSM::State {};

	struct LowRated
		: FSM::State
	{
		float utility(const Control&) { return 0.5f; }
	};

	struct HighRated
		: FSM::State
	{
		float utility(const Control&) { return 2.0f; }
	};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<State>());

	fsm.utilize<Region>();
	fsm.update();
	REQUIRE(fsm.isActive<HighRated>());
}

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Wiki.Transitions within Hierarchy.'Randomize' Transition", "[Wiki]") {
	using M = hfsm2::Machine;

	using FSM = M::PeerRoot<
					struct State,
					M::Composite<struct Region,
						struct FilteredOut,
						struct LowRated,
						struct HighRated
					>
				>;

	struct State     : FSM::State {};
	struct Region    : FSM::State {};

	struct FilteredOut
		: FSM::State
	{
		int8_t rank(const Control&) { return 0; } // filter out using low rank

		float utility(const Control&) { return 0.5f; }
	};

	struct LowRated
		: FSM::State
	{
		int8_t rank(const Control&) { return 1; }

		float utility(const Control&) { return 0.5f; }
	};

	struct HighRated
		: FSM::State
	{
		int8_t rank(const Control&) { return 1; }

		float utility(const Control&) { return 2.0f; }
	};

	FSM::Instance fsm;
	REQUIRE(fsm.isActive<State>());

	fsm.randomize<Region>();
	fsm.update();
	REQUIRE(fsm.isActive<HighRated>()); // note, it could be LowRated if the PRNG is seeded differently
}

////////////////////////////////////////////////////////////////////////////////
