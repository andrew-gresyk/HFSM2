﻿// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_STRUCTURE_REPORT
#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

#include <algorithm>
#include <vector>

#include <wchar.h>

namespace test_debug {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::RandomT<hfsm2::FloatRandom>;

using M = hfsm2::MachineT<Config>;

using Logger = LoggerT<Config>;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Resumable<S(R),
						S(R_1),
						S(R_2)
					>,
					M::CompositePeers<
						S(C_1),
						S(C_2)
					>,
					M::Utilitarian<S(U),
						S(U_1),
						S(U_2)
					>,
					M::Random<S(N),
						S(N_1),
						S(N_2)
					>
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>()	==  0, "");
static_assert(FSM::regionId<O	>()	==  1, "");
static_assert(FSM::regionId<R	>()	==  2, "");
static_assert(FSM::regionId<U	>()	==  4, "");
static_assert(FSM::regionId<N	>()	==  5, "");

static_assert(FSM::stateId<Apex>() ==  0, "");
static_assert(FSM::stateId<I   >() ==  1, "");
static_assert(FSM::stateId<O   >() ==  2, "");
static_assert(FSM::stateId<R   >() ==  3, "");
static_assert(FSM::stateId<R_1 >() ==  4, "");
static_assert(FSM::stateId<R_2 >() ==  5, "");
static_assert(FSM::stateId<C_1 >() ==  7, "");
static_assert(FSM::stateId<C_2 >() ==  8, "");
static_assert(FSM::stateId<U   >() ==  9, "");
static_assert(FSM::stateId<U_1 >() == 10, "");
static_assert(FSM::stateId<U_2 >() == 11, "");
static_assert(FSM::stateId<N   >() == 12, "");
static_assert(FSM::stateId<N_1 >() == 13, "");
static_assert(FSM::stateId<N_2 >() == 14, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	: FSM::State {};
struct I	: FSM::State {};
struct O	: FSM::State {};
struct R	: FSM::State {};
struct R_1	: FSM::State {};
struct R_2	: FSM::State {};
struct C_1	: FSM::State {};
struct C_2	: FSM::State {};
struct U	: FSM::State {};
struct U_1	: FSM::State {};
struct U_2	: FSM::State {};
struct N	: FSM::State {};
struct N_1	: FSM::State {};
struct N_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 15, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

using StructureReference = std::vector<hfsm2::StructureEntry>;

void
assertStructure(const FSM::Instance::Structure& structure,
				const StructureReference& reference)
{
	const std::size_t count = std::max(static_cast<std::size_t>(structure.count()), reference.size());

	for (std::size_t i = 0; i < count; ++i) {
		REQUIRE(i < structure.count());
		REQUIRE(i < reference.size());

		if (i < structure.count() &&
			i < reference.size())
		{
			REQUIRE(structure[i].isActive == reference[i].isActive);
			REQUIRE(wcscmp(structure[i].prefix, reference[i].prefix) == 0);

		#ifdef _MSC_VER
			REQUIRE(strcmp(structure[i].name,   reference[i].name)	 == 0);
		#endif
		}
	}
}

//------------------------------------------------------------------------------

using ActivityReference = std::vector<int8_t>;

void
assertActivity(const FSM::Instance::ActivityHistory& activity,
			   const ActivityReference& reference)
{
	const std::size_t count = std::max(static_cast<std::size_t>(activity.count()), reference.size());

	for (std::size_t i = 0; i < count; ++i) {
		REQUIRE(i < activity.count());
		REQUIRE(i < reference.size());

		if (i < activity.count() &&
			i < reference.size())
		{
			REQUIRE(activity[i] == reference[i]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<I  >(),
	FSM::stateId<O  >(),
	FSM::stateId<R  >(),
	FSM::stateId<R_1>(),
	FSM::stateId<R_2>(),
	FSM::stateId<C_1>(),
	FSM::stateId<C_2>(),
	FSM::stateId<U  >(),
	FSM::stateId<U_1>(),
	FSM::stateId<U_2>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Debug") {
	hfsm2::FloatRandom generator{0};
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{generator, &logger};
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

			assertStructure(machine.structure(), {
				hfsm2::StructureEntry{ true,  L"", "Apex"},
				hfsm2::StructureEntry{ true,  L" ├ ", "I"},
				hfsm2::StructureEntry{ false, L" └ ", "O"},
				hfsm2::StructureEntry{ false, L"   ╟ ", "R"},
				hfsm2::StructureEntry{ false, L"   ║ ├ ", "R_1"},
				hfsm2::StructureEntry{ false, L"   ║ └ ", "R_2"},
				hfsm2::StructureEntry{ false, L"   ╟ ┬ ", "C_1"},
				hfsm2::StructureEntry{ false, L"   ║ └ ", "C_2"},
				hfsm2::StructureEntry{ false, L"   ╟ ", "U"},
				hfsm2::StructureEntry{ false, L"   ║ ├ ", "U_1"},
				hfsm2::StructureEntry{ false, L"   ║ └ ", "U_2"},
				hfsm2::StructureEntry{ false, L"   ╙ ", "N"},
				hfsm2::StructureEntry{ false, L"     ├ ", "N_1"},
				hfsm2::StructureEntry{ false, L"     └ ", "N_2"},
			});

			assertActivity(machine.activityHistory(), {
				+1,
				+1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
				-1,
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.immediateChangeTo<O   >();
		{
			logger.assertSequence({
				{						Event::Type::CHANGE, FSM::stateId<O   >() },

				{ FSM::stateId<U_1 >(),	Event::Type::UTILITY },
				{ FSM::stateId<U_2 >(),	Event::Type::UTILITY },
				{ FSM::stateId<U   >(),	Event::Type::UTILITY_RESOLUTION, 0 },

				{ FSM::stateId<N_1 >(),	Event::Type::REPORT_RANK },
				{ FSM::stateId<N_2 >(),	Event::Type::REPORT_RANK },
				{ FSM::stateId<N_1 >(),	Event::Type::UTILITY },
				{ FSM::stateId<N_2 >(),	Event::Type::UTILITY },
				{ FSM::stateId<N   >(),	Event::Type::RANDOM_RESOLUTION, 1 },

				{ FSM::stateId<I   >(),	Event::Type::EXIT_GUARD },

				{ FSM::stateId<O   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<R   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<R_1 >(),	Event::Type::ENTRY_GUARD },
				{ hfsm2::StateID{6}	  ,	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<C_1 >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<U   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<U_1 >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<N   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<N_2 >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::Type::EXIT },

				{ FSM::stateId<O   >(),	Event::Type::ENTER },
				{ FSM::stateId<R   >(),	Event::Type::ENTER },
				{ FSM::stateId<R_1 >(),	Event::Type::ENTER },
				{ hfsm2::StateID{6}	  ,	Event::Type::ENTER },
				{ FSM::stateId<C_1 >(),	Event::Type::ENTER },
				{ FSM::stateId<U   >(),	Event::Type::ENTER },
				{ FSM::stateId<U_1 >(),	Event::Type::ENTER },
				{ FSM::stateId<N   >(),	Event::Type::ENTER },
				{ FSM::stateId<N_2 >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<R   >(),
				FSM::stateId<R_1 >(),
				hfsm2::StateID{6}	,
				FSM::stateId<C_1 >(),
				FSM::stateId<U   >(),
				FSM::stateId<U_1 >(),
				FSM::stateId<N   >(),
				FSM::stateId<N_2 >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
			});

			assertActivity(machine.activityHistory(), {
				+2,
				-1,
				+1,
				+1,
				+1,
				-2,
				+1,
				-2,
				+1,
				+1,
				-2,
				+1,
				-2,
				+1,
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}

	logger.assertSequence({
		{ FSM::stateId<R_1 >(),	Event::Type::EXIT },
		{ FSM::stateId<R   >(),	Event::Type::EXIT },
		{ FSM::stateId<C_1 >(),	Event::Type::EXIT },
		{ hfsm2::StateID{6}	  ,	Event::Type::EXIT },
		{ FSM::stateId<U_1 >(),	Event::Type::EXIT },
		{ FSM::stateId<U   >(),	Event::Type::EXIT },
		{ FSM::stateId<N_2 >(),	Event::Type::EXIT },
		{ FSM::stateId<N   >(),	Event::Type::EXIT },
		{ FSM::stateId<O   >(),	Event::Type::EXIT },
		{ hfsm2::StateID{0}, 	Event::Type::EXIT },
	});
}

////////////////////////////////////////////////////////////////////////////////

}
