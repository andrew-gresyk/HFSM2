#define HFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

namespace test_replication {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

namespace server{

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(C1),
					S(C1_S1),
					M::Composite<S(C1_C2),
						S(C1_C2_S1),
						S(C1_C2_S2)
					>
				>,
				M::Orthogonal<S(O2),
					M::Composite<S(O2_C1),
						S(O2_C1_S1),
						S(O2_C1_S2)
					>,
					M::Composite<S(O2_C2),
						S(O2_C2_S1),
						S(O2_C2_S2)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<C1   >()   ==  1, "");
static_assert(FSM::regionId<C1_C2>()   ==  2, "");
static_assert(FSM::regionId<O2   >()   ==  3, "");
static_assert(FSM::regionId<O2_C1>()   ==  4, "");
static_assert(FSM::regionId<O2_C2>()   ==  5, "");

static_assert(FSM::stateId<C1      >() ==  1, "");
static_assert(FSM::stateId<C1_S1   >() ==  2, "");
static_assert(FSM::stateId<C1_C2   >() ==  3, "");
static_assert(FSM::stateId<C1_C2_S1>() ==  4, "");
static_assert(FSM::stateId<C1_C2_S2>() ==  5, "");
static_assert(FSM::stateId<O2      >() ==  6, "");
static_assert(FSM::stateId<O2_C1   >() ==  7, "");
static_assert(FSM::stateId<O2_C1_S1>() ==  8, "");
static_assert(FSM::stateId<O2_C1_S2>() ==  9, "");
static_assert(FSM::stateId<O2_C2   >() == 10, "");
static_assert(FSM::stateId<O2_C2_S1>() == 11, "");
static_assert(FSM::stateId<O2_C2_S2>() == 12, "");

//------------------------------------------------------------------------------

struct C1		: FSM::State {};
struct C1_S1	: FSM::State {};
struct C1_C2	: FSM::State {};
struct C1_C2_S1	: FSM::State {};

struct C1_C2_S2
	: FSM::State
{
	void entryGuard(GuardControl& control) {
		control.cancelPendingTransitions();
	}
};

struct O2		: FSM::State {};
struct O2_C1	: FSM::State {};
struct O2_C1_S1	: FSM::State {};
struct O2_C1_S2	: FSM::State {};
struct O2_C2	: FSM::State {};
struct O2_C2_S1	: FSM::State {};
struct O2_C2_S2	: FSM::State {};

//------------------------------------------------------------------------------

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

//------------------------------------------------------------------------------

const Types all = {
	FSM::stateId<C1      >(),
	FSM::stateId<C1_S1   >(),
	FSM::stateId<C1_C2   >(),
	FSM::stateId<C1_C2_S1>(),
	FSM::stateId<C1_C2_S2>(),
	FSM::stateId<O2      >(),
	FSM::stateId<O2_C1   >(),
	FSM::stateId<O2_C1_S1>(),
	FSM::stateId<O2_C1_S2>(),
	FSM::stateId<O2_C2   >(),
	FSM::stateId<O2_C2_S1>(),
	FSM::stateId<O2_C2_S2>(),
};

}

////////////////////////////////////////////////////////////////////////////////

namespace client {

#define S(s) struct s

using FSM = M::PeerRoot<
				M::Composite<S(C1),
					S(C1_S1),
					M::Composite<S(C1_C2),
						S(C1_C2_S1),
						S(C1_C2_S2)
					>
				>,
				M::Orthogonal<S(O2),
					M::Composite<S(O2_C1),
						S(O2_C1_S1),
						S(O2_C1_S2)
					>,
					M::Composite<S(O2_C2),
						S(O2_C2_S1),
						S(O2_C2_S2)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<C1   >()   ==  1, "");
static_assert(FSM::regionId<C1_C2>()   ==  2, "");
static_assert(FSM::regionId<O2   >()   ==  3, "");
static_assert(FSM::regionId<O2_C1>()   ==  4, "");
static_assert(FSM::regionId<O2_C2>()   ==  5, "");

static_assert(FSM::stateId<C1      >() ==  1, "");
static_assert(FSM::stateId<C1_S1   >() ==  2, "");
static_assert(FSM::stateId<C1_C2   >() ==  3, "");
static_assert(FSM::stateId<C1_C2_S1>() ==  4, "");
static_assert(FSM::stateId<C1_C2_S2>() ==  5, "");
static_assert(FSM::stateId<O2      >() ==  6, "");
static_assert(FSM::stateId<O2_C1   >() ==  7, "");
static_assert(FSM::stateId<O2_C1_S1>() ==  8, "");
static_assert(FSM::stateId<O2_C1_S2>() ==  9, "");
static_assert(FSM::stateId<O2_C2   >() == 10, "");
static_assert(FSM::stateId<O2_C2_S1>() == 11, "");
static_assert(FSM::stateId<O2_C2_S2>() == 12, "");

//------------------------------------------------------------------------------

struct C1		: FSM::State {};
struct C1_S1	: FSM::State {};
struct C1_C2	: FSM::State {};
struct C1_C2_S1	: FSM::State {};
struct C1_C2_S2	: FSM::State {};
struct O2		: FSM::State {};
struct O2_C1	: FSM::State {};
struct O2_C1_S1	: FSM::State {};
struct O2_C1_S2	: FSM::State {};
struct O2_C2	: FSM::State {};
struct O2_C2_S1	: FSM::State {};
struct O2_C2_S2	: FSM::State {};

//------------------------------------------------------------------------------

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

//------------------------------------------------------------------------------

const Types all = {
	FSM::stateId<C1      >(),
	FSM::stateId<C1_S1   >(),
	FSM::stateId<C1_C2   >(),
	FSM::stateId<C1_C2_S1>(),
	FSM::stateId<C1_C2_S2>(),
	FSM::stateId<O2      >(),
	FSM::stateId<O2_C1   >(),
	FSM::stateId<O2_C1_S1>(),
	FSM::stateId<O2_C1_S2>(),
	FSM::stateId<O2_C2   >(),
	FSM::stateId<O2_C2_S1>(),
	FSM::stateId<O2_C2_S2>(),
};

}

//------------------------------------------------------------------------------

static_assert(server::FSM::Instance::STATE_COUNT   == client::FSM::Instance::STATE_COUNT  , "STATE_COUNT");
static_assert(server::FSM::Instance::COMPO_REGIONS == client::FSM::Instance::COMPO_REGIONS, "COMPO_REGIONS");
static_assert(server::FSM::Instance::COMPO_PRONGS  == client::FSM::Instance::COMPO_PRONGS , "COMPO_PRONGS");
static_assert(server::FSM::Instance::ORTHO_REGIONS == client::FSM::Instance::ORTHO_REGIONS, "ORTHO_REGIONS");
static_assert(server::FSM::Instance::ORTHO_UNITS   == client::FSM::Instance::ORTHO_UNITS  , "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Replication", "[machine]") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		server::FSM::Instance authority {&logger};
		client::FSM::Instance replicated{&logger};
		{
			assertActive(authority,  server::all, {
				server::FSM::stateId<server::C1      >(),
				server::FSM::stateId<server::C1_S1   >(),
			});

			assertActive(replicated, client::all, {
				client::FSM::stateId<client::C1      >(),
				client::FSM::stateId<client::C1_S1   >(),
			});

			assertResumable(authority,  server::all, {});

			assertResumable(replicated, client::all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::O2_C1_S2>();
		authority.update();
		{
			assertActive(authority,  server::all, {
				server::FSM::stateId<server::O2      >(),
				server::FSM::stateId<server::O2_C1   >(),
				server::FSM::stateId<server::O2_C1_S2>(),
				server::FSM::stateId<server::O2_C2   >(),
				server::FSM::stateId<server::O2_C2_S1>(),
			});

			assertResumable(authority,  server::all, {
				server::FSM::stateId<server::C1      >(),
				server::FSM::stateId<server::C1_S1   >(),
			});

			const auto& transitionHistory = authority.transitionHistory();
			REQUIRE(transitionHistory.count() == 1);

			REQUIRE(transitionHistory[0] == hfsm2::Transition{server::FSM::stateId<server::O2_C1_S2>(),
															  hfsm2::Method::NONE,
															  hfsm2::TransitionType::CHANGE});
		}

		replicated.replayTransition(authority.transitionHistory()[0]);
		{
			assertActive(replicated, client::all, {
				client::FSM::stateId<client::O2      >(),
				client::FSM::stateId<client::O2_C1   >(),
				client::FSM::stateId<client::O2_C1_S2>(),
				client::FSM::stateId<client::O2_C2   >(),
				client::FSM::stateId<client::O2_C2_S1>(),
			});

			assertResumable(replicated, client::all, {
				client::FSM::stateId<client::C1      >(),
				client::FSM::stateId<client::C1_S1   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::O2_C1_S1>();
		authority.changeTo<server::O2_C2_S2>();
		authority.update();
		{
			assertActive(authority,  server::all, {
				server::FSM::stateId<server::O2      >(),
				server::FSM::stateId<server::O2_C1   >(),
				server::FSM::stateId<server::O2_C1_S1>(),
				server::FSM::stateId<server::O2_C2   >(),
				server::FSM::stateId<server::O2_C2_S2>(),
			});

			assertResumable(authority, server::all, {
				server::FSM::stateId<server::C1      >(),
				server::FSM::stateId<server::C1_S1   >(),
				server::FSM::stateId<server::O2_C1_S2>(),
				server::FSM::stateId<server::O2_C2_S1>(),
			});
		}

		{
			const auto& transitionHistory = authority.transitionHistory();
			REQUIRE(transitionHistory.count() == 2);

			REQUIRE(transitionHistory[0] == hfsm2::Transition{server::FSM::stateId<server::O2_C1_S1>(),
															  hfsm2::Method::NONE,
															  hfsm2::TransitionType::CHANGE});

			REQUIRE(transitionHistory[1] == hfsm2::Transition{server::FSM::stateId<server::O2_C2_S2>(),
															  hfsm2::Method::NONE,
															  hfsm2::TransitionType::CHANGE});
		}

		replicated.replayTransitions(&authority.transitionHistory()[0],
									  authority.transitionHistory().count());
		{
			assertActive(replicated, client::all, {
				client::FSM::stateId<client::O2      >(),
				client::FSM::stateId<client::O2_C1   >(),
				client::FSM::stateId<client::O2_C1_S1>(),
				client::FSM::stateId<client::O2_C2   >(),
				client::FSM::stateId<client::O2_C2_S2>(),
			});

			assertResumable(replicated, client::all, {
				client::FSM::stateId<client::C1      >(),
				client::FSM::stateId<client::C1_S1   >(),
				client::FSM::stateId<client::O2_C1_S2>(),
				client::FSM::stateId<client::O2_C2_S1>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		replicated.reset();
		{
			assertActive(replicated, client::all, {
				client::FSM::stateId<client::C1      >(),
				client::FSM::stateId<client::C1_S1   >(),
			});

			assertResumable(replicated, client::all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::C1_C2_S2>();
		authority.update();
		REQUIRE(authority.transitionHistory().count() == 0);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}
}

////////////////////////////////////////////////////////////////////////////////

}
