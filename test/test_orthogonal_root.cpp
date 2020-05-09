#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_orthogonal_root {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::MachineT<hfsm2::Config::ContextT<float>>;

using Action = bool;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::OrthogonalPeerRoot<
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

struct A	 : FSM::State {};
struct A_1	 : FSM::State {};
struct A_2	 : FSM::State {};
struct A_2_1 : FSM::State {};
struct A_2_2 : FSM::State {};
struct B	 : FSM::State {};
struct B_1	 : FSM::State {};
struct B_1_1 : FSM::State {};
struct B_1_2 : FSM::State {};
struct B_2	 : FSM::State {};
struct B_2_1 : FSM::State {};
struct B_2_2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  4, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  ==  8, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  2, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  2, "ORTHO_UNITS");

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

TEST_CASE("FSM.Orthogonal Root", "[machine]") {
	float _ = 0.0f;
	LoggerT<float> logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_, &logger};
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},	 Event::ENTRY_GUARD },
				{ FSM::stateId<A    >(), Event::ENTRY_GUARD },
				{ FSM::stateId<A_1  >(), Event::ENTRY_GUARD },
				{ FSM::stateId<B    >(), Event::ENTRY_GUARD },
				{ FSM::stateId<B_1  >(), Event::ENTRY_GUARD },
				{ FSM::stateId<B_1_1>(), Event::ENTRY_GUARD },
				{ FSM::stateId<B_2  >(), Event::ENTRY_GUARD },
				{ FSM::stateId<B_2_1>(), Event::ENTRY_GUARD },

				{ hfsm2::StateID{0},	 Event::CONSTRUCT },
				{ FSM::stateId<A    >(), Event::CONSTRUCT },
				{ FSM::stateId<A_1  >(), Event::CONSTRUCT },
				{ FSM::stateId<B    >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_1_1>(), Event::CONSTRUCT },
				{ FSM::stateId<B_2  >(), Event::CONSTRUCT },
				{ FSM::stateId<B_2_1>(), Event::CONSTRUCT },

				{ hfsm2::StateID{0},	 Event::ENTER },
				{ FSM::stateId<A    >(), Event::ENTER },
				{ FSM::stateId<A_1  >(), Event::ENTER },
				{ FSM::stateId<B    >(), Event::ENTER },
				{ FSM::stateId<B_1  >(), Event::ENTER },
				{ FSM::stateId<B_1_1>(), Event::ENTER },
				{ FSM::stateId<B_2  >(), Event::ENTER },
				{ FSM::stateId<B_2_1>(), Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<A    >(),
				FSM::stateId<A_1  >(),
				FSM::stateId<B    >(),
				FSM::stateId<B_1  >(),
				FSM::stateId<B_1_1>(),
				FSM::stateId<B_2  >(),
				FSM::stateId<B_2_1>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<A_1  >(), Event::EXIT },
		{ FSM::stateId<A    >(), Event::EXIT },
		{ FSM::stateId<B_1_1>(), Event::EXIT },
		{ FSM::stateId<B_1  >(), Event::EXIT },
		{ FSM::stateId<B_2_1>(), Event::EXIT },
		{ FSM::stateId<B_2  >(), Event::EXIT },
		{ FSM::stateId<B    >(), Event::EXIT },
		{ hfsm2::StateID{0},	 Event::EXIT },

		{ FSM::stateId<A_1  >(), Event::DESTRUCT },
		{ FSM::stateId<A    >(), Event::DESTRUCT },
		{ FSM::stateId<B_1_1>(), Event::DESTRUCT },
		{ FSM::stateId<B_1  >(), Event::DESTRUCT },
		{ FSM::stateId<B_2_1>(), Event::DESTRUCT },
		{ FSM::stateId<B_2  >(), Event::DESTRUCT },
		{ FSM::stateId<B    >(), Event::DESTRUCT },
		{ hfsm2::StateID{0},	 Event::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
