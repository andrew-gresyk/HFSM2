#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_orthogonal_root {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::MachineT<hfsm2::Config::ContextT<float>>;

using Action = bool;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::OrthogonalPeerRoot<
				M::Composite<S(C1),
					S(C1_S1),
					M::OrthogonalPeers<
						S(C1_O2_S1),
						S(C1_O2_S2)
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
static_assert(FSM::regionId<O2   >()   ==  3, "");
static_assert(FSM::regionId<O2_C1>()   ==  4, "");
static_assert(FSM::regionId<O2_C2>()   ==  5, "");

static_assert(FSM::stateId<C1      >() ==  1, "");
static_assert(FSM::stateId<C1_S1   >() ==  2, "");
static_assert(FSM::stateId<C1_O2_S1>() ==  4, "");
static_assert(FSM::stateId<C1_O2_S2>() ==  5, "");
static_assert(FSM::stateId<O2      >() ==  6, "");
static_assert(FSM::stateId<O2_C1   >() ==  7, "");
static_assert(FSM::stateId<O2_C1_S1>() ==  8, "");
static_assert(FSM::stateId<O2_C1_S2>() ==  9, "");
static_assert(FSM::stateId<O2_C2   >() == 10, "");
static_assert(FSM::stateId<O2_C2_S1>() == 11, "");
static_assert(FSM::stateId<O2_C2_S2>() == 12, "");

////////////////////////////////////////////////////////////////////////////////

struct C1		: FSM::State {};
struct C1_S1	: FSM::State {};
struct C1_O2_S1 : FSM::State {};
struct C1_O2_S2 : FSM::State {};
struct O2		: FSM::State {};
struct O2_C1	: FSM::State {};
struct O2_C1_S1 : FSM::State {};
struct O2_C1_S2 : FSM::State {};
struct O2_C2	: FSM::State {};
struct O2_C2_S1 : FSM::State {};
struct O2_C2_S2 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 13, "");
static_assert(FSM::Instance::COMPO_REGIONS ==  3, "");
static_assert(FSM::Instance::COMPO_PRONGS  ==  6, "");
static_assert(FSM::Instance::ORTHO_REGIONS ==  3, "");
static_assert(FSM::Instance::ORTHO_UNITS   ==  3, "");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<C1      >(),
	FSM::stateId<C1_S1   >(),
	FSM::stateId<C1_O2_S1>(),
	FSM::stateId<C1_O2_S2>(),
	FSM::stateId<O2      >(),
	FSM::stateId<O2_C1   >(),
	FSM::stateId<O2_C1_S1>(),
	FSM::stateId<O2_C1_S2>(),
	FSM::stateId<O2_C2   >(),
	FSM::stateId<O2_C2_S1>(),
	FSM::stateId<O2_C2_S2>(),
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
				{ hfsm2::StateID{0},		Event::ENTRY_GUARD },
				{ FSM::stateId<C1      >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<C1_S1   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<O2      >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<O2_C1   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<O2_C1_S1>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<O2_C2   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<O2_C2_S1>(),	Event::ENTRY_GUARD },

				{ hfsm2::StateID{0},		Event::CONSTRUCT },
				{ FSM::stateId<C1      >(),	Event::CONSTRUCT },
				{ FSM::stateId<C1_S1   >(),	Event::CONSTRUCT },
				{ FSM::stateId<O2      >(),	Event::CONSTRUCT },
				{ FSM::stateId<O2_C1   >(),	Event::CONSTRUCT },
				{ FSM::stateId<O2_C1_S1>(),	Event::CONSTRUCT },
				{ FSM::stateId<O2_C2   >(),	Event::CONSTRUCT },
				{ FSM::stateId<O2_C2_S1>(),	Event::CONSTRUCT },

				{ hfsm2::StateID{0},		Event::ENTER },
				{ FSM::stateId<C1      >(),	Event::ENTER },
				{ FSM::stateId<C1_S1   >(),	Event::ENTER },
				{ FSM::stateId<O2      >(),	Event::ENTER },
				{ FSM::stateId<O2_C1   >(),	Event::ENTER },
				{ FSM::stateId<O2_C1_S1>(),	Event::ENTER },
				{ FSM::stateId<O2_C2   >(),	Event::ENTER },
				{ FSM::stateId<O2_C2_S1>(),	Event::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<C1      >(),
				FSM::stateId<C1_S1   >(),
				FSM::stateId<O2      >(),
				FSM::stateId<O2_C1   >(),
				FSM::stateId<O2_C1_S1>(),
				FSM::stateId<O2_C2   >(),
				FSM::stateId<O2_C2_S1>(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<C1_S1   >(),	Event::EXIT },
		{ FSM::stateId<C1      >(),	Event::EXIT },
		{ FSM::stateId<O2_C1_S1>(),	Event::EXIT },
		{ FSM::stateId<O2_C1   >(),	Event::EXIT },
		{ FSM::stateId<O2_C2_S1>(),	Event::EXIT },
		{ FSM::stateId<O2_C2   >(),	Event::EXIT },
		{ FSM::stateId<O2      >(),	Event::EXIT },
		{ hfsm2::StateID{0},		Event::EXIT },

		{ FSM::stateId<C1_S1   >(),	Event::DESTRUCT },
		{ FSM::stateId<C1      >(),	Event::DESTRUCT },
		{ FSM::stateId<O2_C1_S1>(),	Event::DESTRUCT },
		{ FSM::stateId<O2_C1   >(),	Event::DESTRUCT },
		{ FSM::stateId<O2_C2_S1>(),	Event::DESTRUCT },
		{ FSM::stateId<O2_C2   >(),	Event::DESTRUCT },
		{ FSM::stateId<O2      >(),	Event::DESTRUCT },
		{ hfsm2::StateID{0},		Event::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
