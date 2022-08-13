// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_utility_regions {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Utilitarian<S(F),
					M::Composite<S(C),
						S(C_1),
						S(C_2),
						S(C_3)
					>,
					M::Resumable<S(R),
						S(R_1),
						S(R_2),
						S(R_3)
					>,
					M::Utilitarian<S(U),
						S(U_1),
						S(U_2),
						S(U_3)
					>,
					M::Orthogonal<S(O),
						M::Composite<S(OC),
							S(OC_1),
							S(OC_2),
							S(OC_3)
						>,
						M::Resumable<S(OR),
							S(OR_1),
							S(OR_2),
							S(OR_3)
						>,
						M::Utilitarian<S(OU),
							S(OU_1),
							S(OU_2),
							S(OU_3)
						>
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<F   >()	==  1, "");
static_assert(FSM::regionId<C   >()	==  2, "");
static_assert(FSM::regionId<R   >()	==  3, "");
static_assert(FSM::regionId<U   >()	==  4, "");
static_assert(FSM::regionId<O   >()	==  5, "");
static_assert(FSM::regionId<OC  >()	==  6, "");
static_assert(FSM::regionId<OR  >()	==  7, "");
static_assert(FSM::regionId<OU  >()	==  8, "");

static_assert(FSM::stateId<Apex>()  ==  0, "");
static_assert(FSM::stateId<I   >()  ==  1, "");
static_assert(FSM::stateId<F   >()  ==  2, "");
static_assert(FSM::stateId<C   >()  ==  3, "");
static_assert(FSM::stateId<C_1 >()  ==  4, "");
static_assert(FSM::stateId<C_2 >()  ==  5, "");
static_assert(FSM::stateId<C_3 >()  ==  6, "");
static_assert(FSM::stateId<R   >()  ==  7, "");
static_assert(FSM::stateId<R_1 >()  ==  8, "");
static_assert(FSM::stateId<R_2 >()  ==  9, "");
static_assert(FSM::stateId<R_3 >()  == 10, "");
static_assert(FSM::stateId<U   >()  == 11, "");
static_assert(FSM::stateId<U_1 >()  == 12, "");
static_assert(FSM::stateId<U_2 >()  == 13, "");
static_assert(FSM::stateId<U_3 >()  == 14, "");
static_assert(FSM::stateId<O   >()  == 15, "");
static_assert(FSM::stateId<OC  >()  == 16, "");
static_assert(FSM::stateId<OC_1>()  == 17, "");
static_assert(FSM::stateId<OC_2>()  == 18, "");
static_assert(FSM::stateId<OC_3>()  == 19, "");
static_assert(FSM::stateId<OR  >()  == 20, "");
static_assert(FSM::stateId<OR_1>()  == 21, "");
static_assert(FSM::stateId<OR_2>()  == 22, "");
static_assert(FSM::stateId<OR_3>()  == 23, "");
static_assert(FSM::stateId<OU  >()  == 24, "");
static_assert(FSM::stateId<OU_1>()  == 25, "");
static_assert(FSM::stateId<OU_2>()  == 26, "");
static_assert(FSM::stateId<OU_3>()  == 27, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	: FSM::State {};

struct I    : FSM::State {};
struct F    : FSM::State {};

struct C    : FSM::State {};
struct C_1  : FSM::State {};
struct C_2  : FSM::State {};
struct C_3  : FSM::State {};

struct R    : FSM::State {};
struct R_1  : FSM::State {};
struct R_2  : FSM::State {};
struct R_3  : FSM::State {};

struct U    : FSM::State {};
struct U_1  : FSM::State {};
struct U_2  : FSM::State {};
struct U_3  : FSM::State {};

struct O    : FSM::State {};

struct OC   : FSM::State {};
struct OC_1 : FSM::State {};
struct OC_2 : FSM::State {};
struct OC_3 : FSM::State {};

struct OR   : FSM::State {};
struct OR_1 : FSM::State {};
struct OR_2 : FSM::State {};
struct OR_3 : FSM::State {};

struct OU   : FSM::State {};
struct OU_1 : FSM::State {};
struct OU_2 : FSM::State {};
struct OU_3 : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 28, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  9, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  8, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 24, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<I   >(),
	FSM::stateId<F   >(),
	FSM::stateId<C   >(),
	FSM::stateId<C_1 >(),
	FSM::stateId<C_2 >(),
	FSM::stateId<C_3 >(),
	FSM::stateId<R   >(),
	FSM::stateId<R_1 >(),
	FSM::stateId<R_2 >(),
	FSM::stateId<R_3 >(),
	FSM::stateId<U   >(),
	FSM::stateId<U_1 >(),
	FSM::stateId<U_2 >(),
	FSM::stateId<U_3 >(),
	FSM::stateId<O   >(),
	FSM::stateId<OC  >(),
	FSM::stateId<OC_1>(),
	FSM::stateId<OC_2>(),
	FSM::stateId<OC_3>(),
	FSM::stateId<OR  >(),
	FSM::stateId<OR_1>(),
	FSM::stateId<OR_2>(),
	FSM::stateId<OR_3>(),
	FSM::stateId<OU  >(),
	FSM::stateId<OU_1>(),
	FSM::stateId<OU_2>(),
	FSM::stateId<OU_3>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Utility Regions") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<I   >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Apex>(),	Event::Type::ENTER },
				{ FSM::stateId<I   >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex>(),
				FSM::stateId<I   >(),
			});

			assertResumable(machine, all, {});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<F>();
		{
			logger.assertSequence({
				{						Event::Type::CHANGE, FSM::stateId<F   >() },

				{ FSM::stateId<C   >(), Event::Type::UTILITY },
				{ FSM::stateId<C_1 >(), Event::Type::UTILITY },

				{ FSM::stateId<R   >(), Event::Type::UTILITY },
				{ FSM::stateId<R_1 >(), Event::Type::UTILITY },

				{ FSM::stateId<U   >(), Event::Type::UTILITY },
				{ FSM::stateId<U_1 >(), Event::Type::UTILITY },
				{ FSM::stateId<U_2 >(), Event::Type::UTILITY },
				{ FSM::stateId<U_3 >(), Event::Type::UTILITY },
				{ FSM::stateId<U   >(), Event::Type::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<O   >(), Event::Type::UTILITY },

				{ FSM::stateId<OC  >(), Event::Type::UTILITY },
				{ FSM::stateId<OC_1>(), Event::Type::UTILITY },

				{ FSM::stateId<OR  >(), Event::Type::UTILITY },
				{ FSM::stateId<OR_1>(), Event::Type::UTILITY },

				{ FSM::stateId<OU  >(), Event::Type::UTILITY },
				{ FSM::stateId<OU_1>(), Event::Type::UTILITY },
				{ FSM::stateId<OU_2>(), Event::Type::UTILITY },
				{ FSM::stateId<OU_3>(), Event::Type::UTILITY },
				{ FSM::stateId<OU  >(), Event::Type::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<O   >(), Event::Type::UTILITY_RESOLUTION },
				{ FSM::stateId<F   >(), Event::Type::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<I   >(), Event::Type::EXIT_GUARD },
				{ FSM::stateId<F   >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<C   >(), Event::Type::ENTRY_GUARD },
				{ FSM::stateId<C_1 >(), Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(), Event::Type::EXIT },

				{ FSM::stateId<F   >(), Event::Type::ENTER },
				{ FSM::stateId<C   >(), Event::Type::ENTER },
				{ FSM::stateId<C_1 >(), Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<F   >(),
				FSM::stateId<C   >(),
				FSM::stateId<C_1 >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<C_1 >(),	 Event::Type::EXIT },
		{ FSM::stateId<C   >(),	 Event::Type::EXIT },
		{ FSM::stateId<F   >(),	 Event::Type::EXIT },
		{ hfsm2::StateID{0}, 	 Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
