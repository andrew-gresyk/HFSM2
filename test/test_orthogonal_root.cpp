// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_orthogonal_root {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config::ContextT<float>;

using M = hfsm2::MachineT<Config>;

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

static_assert(FSM::Instance::Info::STATE_COUNT   == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  3, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  ==  6, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  3, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  3, "ORTHO_UNITS");

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

TEST_CASE("FSM.Orthogonal Root") {
	float _ = 0.0f;
	LoggerT<Config> logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{_, &logger};
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},		Event::Type::ENTRY_GUARD },
				{ FSM::stateId<C1      >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<C1_S1   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<O2      >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<O2_C1   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<O2_C1_S1>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<O2_C2   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<O2_C2_S1>(),	Event::Type::ENTRY_GUARD },

				{ hfsm2::StateID{0},		Event::Type::CONSTRUCT },
				{ FSM::stateId<C1      >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<C1_S1   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<O2      >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<O2_C1   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<O2_C1_S1>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<O2_C2   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<O2_C2_S1>(),	Event::Type::CONSTRUCT },

				{ hfsm2::StateID{0},		Event::Type::ENTER },
				{ FSM::stateId<C1      >(),	Event::Type::ENTER },
				{ FSM::stateId<C1_S1   >(),	Event::Type::ENTER },
				{ FSM::stateId<O2      >(),	Event::Type::ENTER },
				{ FSM::stateId<O2_C1   >(),	Event::Type::ENTER },
				{ FSM::stateId<O2_C1_S1>(),	Event::Type::ENTER },
				{ FSM::stateId<O2_C2   >(),	Event::Type::ENTER },
				{ FSM::stateId<O2_C2_S1>(),	Event::Type::ENTER },
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
		{ FSM::stateId<C1_S1   >(),	Event::Type::EXIT },
		{ FSM::stateId<C1      >(),	Event::Type::EXIT },
		{ FSM::stateId<O2_C1_S1>(),	Event::Type::EXIT },
		{ FSM::stateId<O2_C1   >(),	Event::Type::EXIT },
		{ FSM::stateId<O2_C2_S1>(),	Event::Type::EXIT },
		{ FSM::stateId<O2_C2   >(),	Event::Type::EXIT },
		{ FSM::stateId<O2      >(),	Event::Type::EXIT },
		{ hfsm2::StateID{0},		Event::Type::EXIT },

		{ FSM::stateId<C1_S1   >(),	Event::Type::DESTRUCT },
		{ FSM::stateId<C1      >(),	Event::Type::DESTRUCT },
		{ FSM::stateId<O2_C1_S1>(),	Event::Type::DESTRUCT },
		{ FSM::stateId<O2_C1   >(),	Event::Type::DESTRUCT },
		{ FSM::stateId<O2_C2_S1>(),	Event::Type::DESTRUCT },
		{ FSM::stateId<O2_C2   >(),	Event::Type::DESTRUCT },
		{ FSM::stateId<O2      >(),	Event::Type::DESTRUCT },
		{ hfsm2::StateID{0},		Event::Type::DESTRUCT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
