#define HFSM2_ENABLE_TRANSITION_HISTORY
#include "tools.hpp"

namespace test_replication {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

namespace server {

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
		REQUIRE(control.currentTransitions().count() == 0);

		const auto& pendingTransitions = control.pendingTransitions();
		REQUIRE(pendingTransitions.count() == 1);
		REQUIRE(pendingTransitions[0] == M::Transition{FSM::stateId<C1_C2_S2>(),
													   hfsm2::TransitionType::CHANGE});

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

static_assert(FSM::Instance::Info::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

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

static_assert(FSM::Instance::Info::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

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

static_assert(server::FSM::Instance::Info::STATE_COUNT   == client::FSM::Instance::Info::STATE_COUNT  , "STATE_COUNT");
static_assert(server::FSM::Instance::Info::REGION_COUNT  == client::FSM::Instance::Info::REGION_COUNT , "REGION_COUNT");
static_assert(server::FSM::Instance::Info::COMPO_REGIONS == client::FSM::Instance::Info::COMPO_REGIONS, "COMPO_REGIONS");
static_assert(server::FSM::Instance::Info::COMPO_PRONGS  == client::FSM::Instance::Info::COMPO_PRONGS , "COMPO_PRONGS");
static_assert(server::FSM::Instance::Info::ORTHO_REGIONS == client::FSM::Instance::Info::ORTHO_REGIONS, "ORTHO_REGIONS");
static_assert(server::FSM::Instance::Info::ORTHO_UNITS   == client::FSM::Instance::Info::ORTHO_UNITS  , "ORTHO_UNITS");

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

			REQUIRE(authority .previousTransitions().count() == 0);
			REQUIRE(replicated.previousTransitions().count() == 0);

			assertLastTransitions(authority , client::all, {});
			assertLastTransitions(replicated, server::all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::O2_C1_S2>();
		authority.update();
		{
			assertActive(authority, server::all, {
				server::FSM::stateId<server::O2      >(),
				server::FSM::stateId<server::O2_C1   >(),
				server::FSM::stateId<server::O2_C1_S2>(),
				server::FSM::stateId<server::O2_C2   >(),
				server::FSM::stateId<server::O2_C2_S1>(),
			});

			assertResumable(authority, server::all, {
				server::FSM::stateId<server::C1      >(),
				server::FSM::stateId<server::C1_S1   >(),
			});

			const auto& previousTransitions = authority .previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{server::FSM::stateId<server::O2_C1_S2>(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(authority, server::all, {
				server::FSM::stateId<server::O2      >(),
				server::FSM::stateId<server::O2_C1   >(),
				server::FSM::stateId<server::O2_C1_S2>(),
				server::FSM::stateId<server::O2_C2   >(),
				server::FSM::stateId<server::O2_C2_S1>(),
			});

			REQUIRE(authority.lastTransition<server::O2      >() == &previousTransitions[0]);
			REQUIRE(authority.lastTransition<server::O2_C1   >() == &previousTransitions[0]);
			REQUIRE(authority.lastTransition<server::O2_C1_S2>() == &previousTransitions[0]);
			REQUIRE(authority.lastTransition<server::O2_C2   >() == &previousTransitions[0]);
			REQUIRE(authority.lastTransition<server::O2_C2_S1>() == &previousTransitions[0]);
		}

		REQUIRE(replicated.replayTransition(authority.previousTransitions()[0]));
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

			const auto& previousTransitions = replicated.previousTransitions();
			REQUIRE(previousTransitions.count() == 1);
			REQUIRE(previousTransitions[0] == M::Transition{client::FSM::stateId<client::O2_C1_S2>(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(replicated, server::all, {
				client::FSM::stateId<client::O2      >(),
				client::FSM::stateId<client::O2_C1   >(),
				client::FSM::stateId<client::O2_C1_S2>(),
				client::FSM::stateId<client::O2_C2   >(),
				client::FSM::stateId<client::O2_C2_S1>(),
			});

			REQUIRE(replicated.lastTransition<client::O2      >() == &previousTransitions[0]);
			REQUIRE(replicated.lastTransition<client::O2_C1   >() == &previousTransitions[0]);
			REQUIRE(replicated.lastTransition<client::O2_C1_S2>() == &previousTransitions[0]);
			REQUIRE(replicated.lastTransition<client::O2_C2   >() == &previousTransitions[0]);
			REQUIRE(replicated.lastTransition<client::O2_C2_S1>() == &previousTransitions[0]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::O2_C1_S1>();
		authority.changeTo<server::O2_C2_S2>();
		authority.update();
		{
			assertActive(authority, server::all, {
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

			const auto& previousTransitions = authority.previousTransitions();
			REQUIRE(previousTransitions.count() == 2);
			REQUIRE(previousTransitions[0] == M::Transition{server::FSM::stateId<server::O2_C1_S1>(),
															hfsm2::TransitionType::CHANGE});
			REQUIRE(previousTransitions[1] == M::Transition{server::FSM::stateId<server::O2_C2_S2>(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(authority, server::all, {
				server::FSM::stateId<server::O2_C1_S1>(),
				server::FSM::stateId<server::O2_C2_S2>(),
			});

			REQUIRE(authority.lastTransition<server::O2_C1_S1>() == &previousTransitions[1]);
			REQUIRE(authority.lastTransition<server::O2_C2_S2>() == &previousTransitions[1]);
		}

		REQUIRE(replicated.replayTransitions(authority.previousTransitions()));
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

			const auto& previousTransitions = replicated.previousTransitions();
			REQUIRE(previousTransitions.count() == 2);
			REQUIRE(previousTransitions[0] == M::Transition{client::FSM::stateId<client::O2_C1_S1>(),
															hfsm2::TransitionType::CHANGE});
			REQUIRE(previousTransitions[1] == M::Transition{client::FSM::stateId<client::O2_C2_S2>(),
															hfsm2::TransitionType::CHANGE});

			assertLastTransitions(replicated, client::all, {
				client::FSM::stateId<client::O2_C1_S1>(),
				client::FSM::stateId<client::O2_C2_S2>(),
			});

			REQUIRE(replicated.lastTransition<client::O2_C1_S1>() == &previousTransitions[1]);
			REQUIRE(replicated.lastTransition<client::O2_C2_S2>() == &previousTransitions[1]);
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		replicated.reset();
		{
			assertActive(replicated, client::all, {
				client::FSM::stateId<client::C1      >(),
				client::FSM::stateId<client::C1_S1   >(),
			});

			assertResumable(replicated, client::all, {});

			REQUIRE(replicated.previousTransitions().count() == 0);

			assertLastTransitions(replicated, client::all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		authority.changeTo<server::C1_C2_S2>();
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

			REQUIRE(authority.previousTransitions().count() == 0);

			assertLastTransitions(authority, server::all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}
}

////////////////////////////////////////////////////////////////////////////////

}
