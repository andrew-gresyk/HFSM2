// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#if !defined _MSC_VER || _MSC_VER > 1900

#define HFSM2_ENABLE_UTILITY_THEORY
#define HFSM2_ENABLE_PLANS
#include "tools.hpp"

using namespace test_tools;

namespace test_types {

////////////////////////////////////////////////////////////////////////////////

using M = hfsm2::Machine;

//------------------------------------------------------------------------------

#define S(s) struct s

using FSM = M::Root<S(Apex),

// #pragma region 100

				M::Orthogonal<
					S(S00_O),
					S(S00_I),
					M::Composite<
						S(S00_C),
						S(S00_C0),
						S(S00_C1),
						S(S00_C2)
					>,
					M::Resumable<
						S(S00_R),
						S(S00_R0),
						S(S00_R1),
						S(S00_R2)
					>
				>,

				M::Orthogonal<
					S(S01_O),
					S(S01_I),
					M::Composite<
						S(S01_C),
						S(S01_C0),
						S(S01_C1),
						S(S01_C2)
					>,
					M::Resumable<
						S(S01_R),
						S(S01_R0),
						S(S01_R1),
						S(S01_R2)
					>
				>,

				M::Orthogonal<
					S(S02_O),
					S(S02_I),
					M::Composite<
						S(S02_C),
						S(S02_C0),
						S(S02_C1),
						S(S02_C2)
					>,
					M::Resumable<
						S(S02_R),
						S(S02_R0),
						S(S02_R1),
						S(S02_R2)
					>
				>,

				M::Orthogonal<
					S(S03_O),
					S(S03_I),
					M::Composite<
						S(S03_C),
						S(S03_C0),
						S(S03_C1),
						S(S03_C2)
					>,
					M::Resumable<
						S(S03_R),
						S(S03_R0),
						S(S03_R1),
						S(S03_R2)
					>
				>,

				M::Orthogonal<
					S(S04_O),
					S(S04_I),
					M::Composite<
						S(S04_C),
						S(S04_C0),
						S(S04_C1),
						S(S04_C2)
					>,
					M::Resumable<
						S(S04_R),
						S(S04_R0),
						S(S04_R1),
						S(S04_R2)
					>
				>,

				M::Orthogonal<
					S(S05_O),
					S(S05_I),
					M::Composite<
						S(S05_C),
						S(S05_C0),
						S(S05_C1),
						S(S05_C2)
					>,
					M::Resumable<
						S(S05_R),
						S(S05_R0),
						S(S05_R1),
						S(S05_R2)
					>
				>,

				M::Orthogonal<
					S(S06_O),
					S(S06_I),
					M::Composite<
						S(S06_C),
						S(S06_C0),
						S(S06_C1),
						S(S06_C2)
					>,
					M::Resumable<
						S(S06_R),
						S(S06_R0),
						S(S06_R1),
						S(S06_R2)
					>
				>,

				M::Orthogonal<
					S(S07_O),
					S(S07_I),
					M::Composite<
						S(S07_C),
						S(S07_C0),
						S(S07_C1),
						S(S07_C2)
					>,
					M::Resumable<
						S(S07_R),
						S(S07_R0),
						S(S07_R1),
						S(S07_R2)
					>
				>,

				M::Orthogonal<
					S(S08_O),
					S(S08_I),
					M::Composite<
						S(S08_C),
						S(S08_C0),
						S(S08_C1),
						S(S08_C2)
					>,
					M::Resumable<
						S(S08_R),
						S(S08_R0),
						S(S08_R1),
						S(S08_R2)
					>
				>,

				M::Orthogonal<
					S(S09_O),
					S(S09_I),
					M::Composite<
						S(S09_C),
						S(S09_C0),
						S(S09_C1),
						S(S09_C2)
					>,
					M::Resumable<
						S(S09_R),
						S(S09_R0),
						S(S09_R1),
						S(S09_R2)
					>
				>,

// #pragma endregion

// #pragma region 200

				M::Orthogonal<
					S(S10_O),
					S(S10_I),
					M::Composite<
						S(S10_C),
						S(S10_C0),
						S(S10_C1),
						S(S10_C2)
					>,
					M::Resumable<
						S(S10_R),
						S(S10_R0),
						S(S10_R1),
						S(S10_R2)
					>
				>,

				M::Orthogonal<
					S(S11_O),
					S(S11_I),
					M::Composite<
						S(S11_C),
						S(S11_C0),
						S(S11_C1),
						S(S11_C2)
					>,
					M::Resumable<
						S(S11_R),
						S(S11_R0),
						S(S11_R1),
						S(S11_R2)
					>
				>,

				M::Orthogonal<
					S(S12_O),
					S(S12_I),
					M::Composite<
						S(S12_C),
						S(S12_C0),
						S(S12_C1),
						S(S12_C2)
					>,
					M::Resumable<
						S(S12_R),
						S(S12_R0),
						S(S12_R1),
						S(S12_R2)
					>
				>,

				M::Orthogonal<
					S(S13_O),
					S(S13_I),
					M::Composite<
						S(S13_C),
						S(S13_C0),
						S(S13_C1),
						S(S13_C2)
					>,
					M::Resumable<
						S(S13_R),
						S(S13_R0),
						S(S13_R1),
						S(S13_R2)
					>
				>,

				M::Orthogonal<
					S(S14_O),
					S(S14_I),
					M::Composite<
						S(S14_C),
						S(S14_C0),
						S(S14_C1),
						S(S14_C2)
					>,
					M::Resumable<
						S(S14_R),
						S(S14_R0),
						S(S14_R1),
						S(S14_R2)
					>
				>,

				M::Orthogonal<
					S(S15_O),
					S(S15_I),
					M::Composite<
						S(S15_C),
						S(S15_C0),
						S(S15_C1),
						S(S15_C2)
					>,
					M::Resumable<
						S(S15_R),
						S(S15_R0),
						S(S15_R1),
						S(S15_R2)
					>
				>,

				M::Orthogonal<
					S(S16_O),
					S(S16_I),
					M::Composite<
						S(S16_C),
						S(S16_C0),
						S(S16_C1),
						S(S16_C2)
					>,
					M::Resumable<
						S(S16_R),
						S(S16_R0),
						S(S16_R1),
						S(S16_R2)
					>
				>,

				M::Orthogonal<
					S(S17_O),
					S(S17_I),
					M::Composite<
						S(S17_C),
						S(S17_C0),
						S(S17_C1),
						S(S17_C2)
					>,
					M::Resumable<
						S(S17_R),
						S(S17_R0),
						S(S17_R1),
						S(S17_R2)
					>
				>,

				M::Orthogonal<
					S(S18_O),
					S(S18_I),
					M::Composite<
						S(S18_C),
						S(S18_C0),
						S(S18_C1),
						S(S18_C2)
					>,
					M::Resumable<
						S(S18_R),
						S(S18_R0),
						S(S18_R1),
						S(S18_R2)
					>
				>,

				M::Orthogonal<
					S(S19_O),
					S(S19_I),
					M::Composite<
						S(S19_C),
						S(S19_C0),
						S(S19_C1),
						S(S19_C2)
					>,
					M::Resumable<
						S(S19_R),
						S(S19_R0),
						S(S19_R1),
						S(S19_R2)
					>
				>,

// #pragma endregion

// #pragma region 300

				M::Orthogonal<
					S(S20_O),
					S(S20_I),
					M::Composite<
						S(S20_C),
						S(S20_C0),
						S(S20_C1),
						S(S20_C2)
					>,
					M::Resumable<
						S(S20_R),
						S(S20_R0),
						S(S20_R1),
						S(S20_R2)
					>
				>,

				M::Orthogonal<
					S(S21_O),
					S(S21_I),
					M::Composite<
						S(S21_C),
						S(S21_C0),
						S(S21_C1),
						S(S21_C2)
					>,
					M::Resumable<
						S(S21_R),
						S(S21_R0),
						S(S21_R1),
						S(S21_R2)
					>
				>,

				M::Orthogonal<
					S(S22_O),
					S(S22_I),
					M::Composite<
						S(S22_C),
						S(S22_C0),
						S(S22_C1),
						S(S22_C2)
					>,
					M::Resumable<
						S(S22_R),
						S(S22_R0),
						S(S22_R1),
						S(S22_R2)
					>
				>,

				M::Orthogonal<
					S(S23_O),
					S(S23_I),
					M::Composite<
						S(S23_C),
						S(S23_C0),
						S(S23_C1),
						S(S23_C2)
					>,
					M::Resumable<
						S(S23_R),
						S(S23_R0),
						S(S23_R1),
						S(S23_R2)
					>
				>,

				M::Orthogonal<
					S(S24_O),
					S(S24_I),
					M::Composite<
						S(S24_C),
						S(S24_C0),
						S(S24_C1),
						S(S24_C2)
					>,
					M::Resumable<
						S(S24_R),
						S(S24_R0),
						S(S24_R1),
						S(S24_R2)
					>
				>,

				M::Orthogonal<
					S(S25_O),
					S(S25_I),
					M::Composite<
						S(S25_C),
						S(S25_C0),
						S(S25_C1),
						S(S25_C2)
					>,
					M::Resumable<
						S(S25_R),
						S(S25_R0),
						S(S25_R1),
						S(S25_R2)
					>
				>,

				M::Orthogonal<
					S(S26_O),
					S(S26_I),
					M::Composite<
						S(S26_C),
						S(S26_C0),
						S(S26_C1),
						S(S26_C2)
					>,
					M::Resumable<
						S(S26_R),
						S(S26_R0),
						S(S26_R1),
						S(S26_R2)
					>
				>

// #pragma endregion

			>;

#undef S

////////////////////////////////////////////////////////////////////////////////

struct Apex	: FSM::State {};

// #pragma region 100

struct S00_O  : FSM::State {};
struct S00_I  : FSM::State {};
struct S00_C  : FSM::State {};
struct S00_C0 : FSM::State {};
struct S00_C1 : FSM::State {};
struct S00_C2 : FSM::State {};
struct S00_R  : FSM::State {};
struct S00_R0 : FSM::State { Utility utility(const Control&) { return 0.0f; } };
struct S00_R1 : FSM::State { Utility utility(const Control&) { return 0.5f; } };
struct S00_R2 : FSM::State { Utility utility(const Control&) { return 1.0f; } };

struct S01_O  : FSM::State {};
struct S01_I  : FSM::State {};
struct S01_C  : FSM::State {};
struct S01_C0 : FSM::State {};
struct S01_C1 : FSM::State {};
struct S01_C2 : FSM::State {};
struct S01_R  : FSM::State {};
struct S01_R0 : FSM::State {};
struct S01_R1 : FSM::State {};
struct S01_R2 : FSM::State {};

struct S02_O  : FSM::State {};
struct S02_I  : FSM::State {};
struct S02_C  : FSM::State {};
struct S02_C0 : FSM::State {};
struct S02_C1 : FSM::State {};
struct S02_C2 : FSM::State {};
struct S02_R  : FSM::State {};
struct S02_R0 : FSM::State {};
struct S02_R1 : FSM::State {};
struct S02_R2 : FSM::State {};

struct S03_O  : FSM::State {};
struct S03_I  : FSM::State {};
struct S03_C  : FSM::State {};
struct S03_C0 : FSM::State {};
struct S03_C1 : FSM::State {};
struct S03_C2 : FSM::State {};
struct S03_R  : FSM::State {};
struct S03_R0 : FSM::State {};
struct S03_R1 : FSM::State {};
struct S03_R2 : FSM::State {};

struct S04_O  : FSM::State {};
struct S04_I  : FSM::State {};
struct S04_C  : FSM::State {};
struct S04_C0 : FSM::State {};
struct S04_C1 : FSM::State {};
struct S04_C2 : FSM::State {};
struct S04_R  : FSM::State {};
struct S04_R0 : FSM::State {};
struct S04_R1 : FSM::State {};
struct S04_R2 : FSM::State {};

struct S05_O  : FSM::State {};
struct S05_I  : FSM::State {};
struct S05_C  : FSM::State {};
struct S05_C0 : FSM::State {};
struct S05_C1 : FSM::State {};
struct S05_C2 : FSM::State {};
struct S05_R  : FSM::State {};
struct S05_R0 : FSM::State {};
struct S05_R1 : FSM::State {};
struct S05_R2 : FSM::State {};

struct S06_O  : FSM::State {};
struct S06_I  : FSM::State {};
struct S06_C  : FSM::State {};
struct S06_C0 : FSM::State {};
struct S06_C1 : FSM::State {};
struct S06_C2 : FSM::State {};
struct S06_R  : FSM::State {};
struct S06_R0 : FSM::State {};
struct S06_R1 : FSM::State {};
struct S06_R2 : FSM::State {};

struct S07_O  : FSM::State {};
struct S07_I  : FSM::State {};
struct S07_C  : FSM::State {};
struct S07_C0 : FSM::State {};
struct S07_C1 : FSM::State {};
struct S07_C2 : FSM::State {};
struct S07_R  : FSM::State {};
struct S07_R0 : FSM::State {};
struct S07_R1 : FSM::State {};
struct S07_R2 : FSM::State {};

struct S08_O  : FSM::State {};
struct S08_I  : FSM::State {};
struct S08_C  : FSM::State {};
struct S08_C0 : FSM::State {};
struct S08_C1 : FSM::State {};
struct S08_C2 : FSM::State {};
struct S08_R  : FSM::State {};
struct S08_R0 : FSM::State {};
struct S08_R1 : FSM::State {};
struct S08_R2 : FSM::State {};

struct S09_O  : FSM::State {};
struct S09_I  : FSM::State {};
struct S09_C  : FSM::State {};
struct S09_C0 : FSM::State {};
struct S09_C1 : FSM::State {};
struct S09_C2 : FSM::State {};
struct S09_R  : FSM::State {};
struct S09_R0 : FSM::State {};
struct S09_R1 : FSM::State {};
struct S09_R2 : FSM::State {};

// #pragma endregion

// #pragma region 200

struct S10_O  : FSM::State {};
struct S10_I  : FSM::State {};
struct S10_C  : FSM::State {};
struct S10_C0 : FSM::State {};
struct S10_C1 : FSM::State {};
struct S10_C2 : FSM::State {};
struct S10_R  : FSM::State {};
struct S10_R0 : FSM::State {};
struct S10_R1 : FSM::State {};
struct S10_R2 : FSM::State {};

struct S11_O  : FSM::State {};
struct S11_I  : FSM::State {};
struct S11_C  : FSM::State {};
struct S11_C0 : FSM::State {};
struct S11_C1 : FSM::State {};
struct S11_C2 : FSM::State {};
struct S11_R  : FSM::State {};
struct S11_R0 : FSM::State {};
struct S11_R1 : FSM::State {};
struct S11_R2 : FSM::State {};

struct S12_O  : FSM::State {};
struct S12_I  : FSM::State {};
struct S12_C  : FSM::State {};
struct S12_C0 : FSM::State {};
struct S12_C1 : FSM::State {};
struct S12_C2 : FSM::State {};
struct S12_R  : FSM::State {};
struct S12_R0 : FSM::State {};
struct S12_R1 : FSM::State {};
struct S12_R2 : FSM::State {};

struct S13_O  : FSM::State {};
struct S13_I  : FSM::State {};
struct S13_C  : FSM::State {};
struct S13_C0 : FSM::State {};
struct S13_C1 : FSM::State {};
struct S13_C2 : FSM::State {};
struct S13_R  : FSM::State {};
struct S13_R0 : FSM::State {};
struct S13_R1 : FSM::State {};
struct S13_R2 : FSM::State {};

struct S14_O  : FSM::State {};
struct S14_I  : FSM::State {};
struct S14_C  : FSM::State {};
struct S14_C0 : FSM::State {};
struct S14_C1 : FSM::State {};
struct S14_C2 : FSM::State {};
struct S14_R  : FSM::State {};
struct S14_R0 : FSM::State {};
struct S14_R1 : FSM::State {};
struct S14_R2 : FSM::State {};

struct S15_O  : FSM::State {};
struct S15_I  : FSM::State {};
struct S15_C  : FSM::State {};
struct S15_C0 : FSM::State {};
struct S15_C1 : FSM::State {};
struct S15_C2 : FSM::State {};
struct S15_R  : FSM::State {};
struct S15_R0 : FSM::State {};
struct S15_R1 : FSM::State {};
struct S15_R2 : FSM::State {};

struct S16_O  : FSM::State {};
struct S16_I  : FSM::State {};
struct S16_C  : FSM::State {};
struct S16_C0 : FSM::State {};
struct S16_C1 : FSM::State {};
struct S16_C2 : FSM::State {};
struct S16_R  : FSM::State {};
struct S16_R0 : FSM::State {};
struct S16_R1 : FSM::State {};
struct S16_R2 : FSM::State {};

struct S17_O  : FSM::State {};
struct S17_I  : FSM::State {};
struct S17_C  : FSM::State {};
struct S17_C0 : FSM::State {};
struct S17_C1 : FSM::State {};
struct S17_C2 : FSM::State {};
struct S17_R  : FSM::State {};
struct S17_R0 : FSM::State {};
struct S17_R1 : FSM::State {};
struct S17_R2 : FSM::State {};

struct S18_O  : FSM::State {};
struct S18_I  : FSM::State {};
struct S18_C  : FSM::State {};
struct S18_C0 : FSM::State {};
struct S18_C1 : FSM::State {};
struct S18_C2 : FSM::State {};
struct S18_R  : FSM::State {};
struct S18_R0 : FSM::State {};
struct S18_R1 : FSM::State {};
struct S18_R2 : FSM::State {};

struct S19_O  : FSM::State {};
struct S19_I  : FSM::State {};
struct S19_C  : FSM::State {};
struct S19_C0 : FSM::State {};
struct S19_C1 : FSM::State {};
struct S19_C2 : FSM::State {};
struct S19_R  : FSM::State {};
struct S19_R0 : FSM::State {};
struct S19_R1 : FSM::State {};
struct S19_R2 : FSM::State {};

// #pragma endregion

// #pragma region 300

struct S20_O  : FSM::State {};
struct S20_I  : FSM::State {};
struct S20_C  : FSM::State {};
struct S20_C0 : FSM::State {};
struct S20_C1 : FSM::State {};
struct S20_C2 : FSM::State {};
struct S20_R  : FSM::State {};
struct S20_R0 : FSM::State {};
struct S20_R1 : FSM::State {};
struct S20_R2 : FSM::State {};

struct S21_O  : FSM::State {};
struct S21_I  : FSM::State {};
struct S21_C  : FSM::State {};
struct S21_C0 : FSM::State {};
struct S21_C1 : FSM::State {};
struct S21_C2 : FSM::State {};
struct S21_R  : FSM::State {};
struct S21_R0 : FSM::State {};
struct S21_R1 : FSM::State {};
struct S21_R2 : FSM::State {};

struct S22_O  : FSM::State {};
struct S22_I  : FSM::State {};
struct S22_C  : FSM::State {};
struct S22_C0 : FSM::State {};
struct S22_C1 : FSM::State {};
struct S22_C2 : FSM::State {};
struct S22_R  : FSM::State {};
struct S22_R0 : FSM::State {};
struct S22_R1 : FSM::State {};
struct S22_R2 : FSM::State {};

struct S23_O  : FSM::State {};
struct S23_I  : FSM::State {};
struct S23_C  : FSM::State {};
struct S23_C0 : FSM::State {};
struct S23_C1 : FSM::State {};
struct S23_C2 : FSM::State {};
struct S23_R  : FSM::State {};
struct S23_R0 : FSM::State {};
struct S23_R1 : FSM::State {};
struct S23_R2 : FSM::State {};

struct S24_O  : FSM::State {};
struct S24_I  : FSM::State {};
struct S24_C  : FSM::State {};
struct S24_C0 : FSM::State {};
struct S24_C1 : FSM::State {};
struct S24_C2 : FSM::State {};
struct S24_R  : FSM::State {};
struct S24_R0 : FSM::State {};
struct S24_R1 : FSM::State {};
struct S24_R2 : FSM::State {};

struct S25_O  : FSM::State {};
struct S25_I  : FSM::State {};
struct S25_C  : FSM::State {};
struct S25_C0 : FSM::State {};
struct S25_C1 : FSM::State {};
struct S25_C2 : FSM::State {};
struct S25_R  : FSM::State {};
struct S25_R0 : FSM::State {};
struct S25_R1 : FSM::State {};
struct S25_R2 : FSM::State {};

struct S26_O  : FSM::State {};
struct S26_I  : FSM::State {};

struct S26_C
	: FSM::State
{
	Short select(const Control&) { return 1; };
};

struct S26_C0 : FSM::State {};
struct S26_C1 : FSM::State {};
struct S26_C2 : FSM::State {};

struct S26_R  : FSM::State {};

struct S26_R0 : FSM::State { Utility utility(const Control&) { return 0.0f; } };
struct S26_R1 : FSM::State { Utility utility(const Control&) { return 0.5f; } };
struct S26_R2 : FSM::State { Utility utility(const Control&) { return 1.0f; } };

// #pragma endregion

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 271, "STATE_COUNT" );
static_assert(FSM::Instance::Info::REGION_COUNT  ==  82, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_COUNT	 ==  55, "COMPO_COUNT" );
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 189, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_COUNT	 ==  27, "ORTHO_COUNT" );
static_assert(FSM::Instance::Info::ORTHO_UNITS   ==  27, "ORTHO_UNITS" );

////////////////////////////////////////////////////////////////////////////////

const Types all = {

// #pragma region 100

	FSM::stateId<S00_O >(),
	FSM::stateId<S00_I >(),
	FSM::stateId<S00_C >(),
	FSM::stateId<S00_C0>(),
	FSM::stateId<S00_C1>(),
	FSM::stateId<S00_C2>(),
	FSM::stateId<S00_R >(),
	FSM::stateId<S00_R0>(),
	FSM::stateId<S00_R1>(),
	FSM::stateId<S00_R2>(),

	FSM::stateId<S01_O >(),
	FSM::stateId<S01_I >(),
	FSM::stateId<S01_C >(),
	FSM::stateId<S01_C0>(),
	FSM::stateId<S01_C1>(),
	FSM::stateId<S01_C2>(),
	FSM::stateId<S01_R >(),
	FSM::stateId<S01_R0>(),
	FSM::stateId<S01_R1>(),
	FSM::stateId<S01_R2>(),

	FSM::stateId<S02_O >(),
	FSM::stateId<S02_I >(),
	FSM::stateId<S02_C >(),
	FSM::stateId<S02_C0>(),
	FSM::stateId<S02_C1>(),
	FSM::stateId<S02_C2>(),
	FSM::stateId<S02_R >(),
	FSM::stateId<S02_R0>(),
	FSM::stateId<S02_R1>(),
	FSM::stateId<S02_R2>(),

	FSM::stateId<S03_O >(),
	FSM::stateId<S03_I >(),
	FSM::stateId<S03_C >(),
	FSM::stateId<S03_C0>(),
	FSM::stateId<S03_C1>(),
	FSM::stateId<S03_C2>(),
	FSM::stateId<S03_R >(),
	FSM::stateId<S03_R0>(),
	FSM::stateId<S03_R1>(),
	FSM::stateId<S03_R2>(),

	FSM::stateId<S04_O >(),
	FSM::stateId<S04_I >(),
	FSM::stateId<S04_C >(),
	FSM::stateId<S04_C0>(),
	FSM::stateId<S04_C1>(),
	FSM::stateId<S04_C2>(),
	FSM::stateId<S04_R >(),
	FSM::stateId<S04_R0>(),
	FSM::stateId<S04_R1>(),
	FSM::stateId<S04_R2>(),

	FSM::stateId<S05_O >(),
	FSM::stateId<S05_I >(),
	FSM::stateId<S05_C >(),
	FSM::stateId<S05_C0>(),
	FSM::stateId<S05_C1>(),
	FSM::stateId<S05_C2>(),
	FSM::stateId<S05_R >(),
	FSM::stateId<S05_R0>(),
	FSM::stateId<S05_R1>(),
	FSM::stateId<S05_R2>(),

	FSM::stateId<S06_O >(),
	FSM::stateId<S06_I >(),
	FSM::stateId<S06_C >(),
	FSM::stateId<S06_C0>(),
	FSM::stateId<S06_C1>(),
	FSM::stateId<S06_C2>(),
	FSM::stateId<S06_R >(),
	FSM::stateId<S06_R0>(),
	FSM::stateId<S06_R1>(),
	FSM::stateId<S06_R2>(),

	FSM::stateId<S07_O >(),
	FSM::stateId<S07_I >(),
	FSM::stateId<S07_C >(),
	FSM::stateId<S07_C0>(),
	FSM::stateId<S07_C1>(),
	FSM::stateId<S07_C2>(),
	FSM::stateId<S07_R >(),
	FSM::stateId<S07_R0>(),
	FSM::stateId<S07_R1>(),
	FSM::stateId<S07_R2>(),

	FSM::stateId<S08_O >(),
	FSM::stateId<S08_I >(),
	FSM::stateId<S08_C >(),
	FSM::stateId<S08_C0>(),
	FSM::stateId<S08_C1>(),
	FSM::stateId<S08_C2>(),
	FSM::stateId<S08_R >(),
	FSM::stateId<S08_R0>(),
	FSM::stateId<S08_R1>(),
	FSM::stateId<S08_R2>(),

	FSM::stateId<S09_O >(),
	FSM::stateId<S09_I >(),
	FSM::stateId<S09_C >(),
	FSM::stateId<S09_C0>(),
	FSM::stateId<S09_C1>(),
	FSM::stateId<S09_C2>(),
	FSM::stateId<S09_R >(),
	FSM::stateId<S09_R0>(),
	FSM::stateId<S09_R1>(),
	FSM::stateId<S09_R2>(),

// #pragma endregion

// #pragma region 200

	FSM::stateId<S10_O >(),
	FSM::stateId<S10_I >(),
	FSM::stateId<S10_C >(),
	FSM::stateId<S10_C0>(),
	FSM::stateId<S10_C1>(),
	FSM::stateId<S10_C2>(),
	FSM::stateId<S10_R >(),
	FSM::stateId<S10_R0>(),
	FSM::stateId<S10_R1>(),
	FSM::stateId<S10_R2>(),

	FSM::stateId<S11_O >(),
	FSM::stateId<S11_I >(),
	FSM::stateId<S11_C >(),
	FSM::stateId<S11_C0>(),
	FSM::stateId<S11_C1>(),
	FSM::stateId<S11_C2>(),
	FSM::stateId<S11_R >(),
	FSM::stateId<S11_R0>(),
	FSM::stateId<S11_R1>(),
	FSM::stateId<S11_R2>(),

	FSM::stateId<S12_O >(),
	FSM::stateId<S12_I >(),
	FSM::stateId<S12_C >(),
	FSM::stateId<S12_C0>(),
	FSM::stateId<S12_C1>(),
	FSM::stateId<S12_C2>(),
	FSM::stateId<S12_R >(),
	FSM::stateId<S12_R0>(),
	FSM::stateId<S12_R1>(),
	FSM::stateId<S12_R2>(),

	FSM::stateId<S13_O >(),
	FSM::stateId<S13_I >(),
	FSM::stateId<S13_C >(),
	FSM::stateId<S13_C0>(),
	FSM::stateId<S13_C1>(),
	FSM::stateId<S13_C2>(),
	FSM::stateId<S13_R >(),
	FSM::stateId<S13_R0>(),
	FSM::stateId<S13_R1>(),
	FSM::stateId<S13_R2>(),

	FSM::stateId<S14_O >(),
	FSM::stateId<S14_I >(),
	FSM::stateId<S14_C >(),
	FSM::stateId<S14_C0>(),
	FSM::stateId<S14_C1>(),
	FSM::stateId<S14_C2>(),
	FSM::stateId<S14_R >(),
	FSM::stateId<S14_R0>(),
	FSM::stateId<S14_R1>(),
	FSM::stateId<S14_R2>(),

	FSM::stateId<S15_O >(),
	FSM::stateId<S15_I >(),
	FSM::stateId<S15_C >(),
	FSM::stateId<S15_C0>(),
	FSM::stateId<S15_C1>(),
	FSM::stateId<S15_C2>(),
	FSM::stateId<S15_R >(),
	FSM::stateId<S15_R0>(),
	FSM::stateId<S15_R1>(),
	FSM::stateId<S15_R2>(),

	FSM::stateId<S16_O >(),
	FSM::stateId<S16_I >(),
	FSM::stateId<S16_C >(),
	FSM::stateId<S16_C0>(),
	FSM::stateId<S16_C1>(),
	FSM::stateId<S16_C2>(),
	FSM::stateId<S16_R >(),
	FSM::stateId<S16_R0>(),
	FSM::stateId<S16_R1>(),
	FSM::stateId<S16_R2>(),

	FSM::stateId<S17_O >(),
	FSM::stateId<S17_I >(),
	FSM::stateId<S17_C >(),
	FSM::stateId<S17_C0>(),
	FSM::stateId<S17_C1>(),
	FSM::stateId<S17_C2>(),
	FSM::stateId<S17_R >(),
	FSM::stateId<S17_R0>(),
	FSM::stateId<S17_R1>(),
	FSM::stateId<S17_R2>(),

	FSM::stateId<S18_O >(),
	FSM::stateId<S18_I >(),
	FSM::stateId<S18_C >(),
	FSM::stateId<S18_C0>(),
	FSM::stateId<S18_C1>(),
	FSM::stateId<S18_C2>(),
	FSM::stateId<S18_R >(),
	FSM::stateId<S18_R0>(),
	FSM::stateId<S18_R1>(),
	FSM::stateId<S18_R2>(),

	FSM::stateId<S19_O >(),
	FSM::stateId<S19_I >(),
	FSM::stateId<S19_C >(),
	FSM::stateId<S19_C0>(),
	FSM::stateId<S19_C1>(),
	FSM::stateId<S19_C2>(),
	FSM::stateId<S19_R >(),
	FSM::stateId<S19_R0>(),
	FSM::stateId<S19_R1>(),
	FSM::stateId<S19_R2>(),

// #pragma endregion

// #pragma region 300

	FSM::stateId<S20_O >(),
	FSM::stateId<S20_I >(),
	FSM::stateId<S20_C >(),
	FSM::stateId<S20_C0>(),
	FSM::stateId<S20_C1>(),
	FSM::stateId<S20_C2>(),
	FSM::stateId<S20_R >(),
	FSM::stateId<S20_R0>(),
	FSM::stateId<S20_R1>(),
	FSM::stateId<S20_R2>(),

	FSM::stateId<S21_O >(),
	FSM::stateId<S21_I >(),
	FSM::stateId<S21_C >(),
	FSM::stateId<S21_C0>(),
	FSM::stateId<S21_C1>(),
	FSM::stateId<S21_C2>(),
	FSM::stateId<S21_R >(),
	FSM::stateId<S21_R0>(),
	FSM::stateId<S21_R1>(),
	FSM::stateId<S21_R2>(),

	FSM::stateId<S22_O >(),
	FSM::stateId<S22_I >(),
	FSM::stateId<S22_C >(),
	FSM::stateId<S22_C0>(),
	FSM::stateId<S22_C1>(),
	FSM::stateId<S22_C2>(),
	FSM::stateId<S22_R >(),
	FSM::stateId<S22_R0>(),
	FSM::stateId<S22_R1>(),
	FSM::stateId<S22_R2>(),

	FSM::stateId<S23_O >(),
	FSM::stateId<S23_I >(),
	FSM::stateId<S23_C >(),
	FSM::stateId<S23_C0>(),
	FSM::stateId<S23_C1>(),
	FSM::stateId<S23_C2>(),
	FSM::stateId<S23_R >(),
	FSM::stateId<S23_R0>(),
	FSM::stateId<S23_R1>(),
	FSM::stateId<S23_R2>(),

	FSM::stateId<S24_O >(),
	FSM::stateId<S24_I >(),
	FSM::stateId<S24_C >(),
	FSM::stateId<S24_C0>(),
	FSM::stateId<S24_C1>(),
	FSM::stateId<S24_C2>(),
	FSM::stateId<S24_R >(),
	FSM::stateId<S24_R0>(),
	FSM::stateId<S24_R1>(),
	FSM::stateId<S24_R2>(),

	FSM::stateId<S25_O >(),
	FSM::stateId<S25_I >(),
	FSM::stateId<S25_C >(),
	FSM::stateId<S25_C0>(),
	FSM::stateId<S25_C1>(),
	FSM::stateId<S25_C2>(),
	FSM::stateId<S25_R >(),
	FSM::stateId<S25_R0>(),
	FSM::stateId<S25_R1>(),
	FSM::stateId<S25_R2>(),

	FSM::stateId<S26_O >(),
	FSM::stateId<S26_I >(),
	FSM::stateId<S26_C >(),
	FSM::stateId<S26_C0>(),
	FSM::stateId<S26_C1>(),
	FSM::stateId<S26_C2>(),
	FSM::stateId<S26_R >(),
	FSM::stateId<S26_R0>(),
	FSM::stateId<S26_R1>(),
	FSM::stateId<S26_R2>(),

// #pragma endregion

};

//------------------------------------------------------------------------------

void step0(FSM::Instance& machine) {
	assertActive(machine, all, {
		FSM::stateId<Apex  >(),
		FSM::stateId<S00_O >(),
		FSM::stateId<S00_I >(),
		FSM::stateId<S00_C >(),
		FSM::stateId<S00_C0>(),
		FSM::stateId<S00_R >(),
		FSM::stateId<S00_R0>(),
	});

	assertResumable(machine, all, {});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step1(FSM::Instance& machine) {
	machine.immediateChangeTo<S26_O>();

	assertActive(machine, all, {
		FSM::stateId<Apex  >(),
		FSM::stateId<S26_O >(),
		FSM::stateId<S26_I >(),
		FSM::stateId<S26_C >(),
		FSM::stateId<S26_C0>(),
		FSM::stateId<S26_R >(),
		FSM::stateId<S26_R0>(),
	});

	assertResumable(machine, all, {
		FSM::stateId<S00_O >(),
		FSM::stateId<S00_I >(),
		FSM::stateId<S00_C >(),
		FSM::stateId<S00_C0>(),
		FSM::stateId<S00_R >(),
		FSM::stateId<S00_R0>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step2(FSM::Instance& machine) {
	machine.immediateSelect<S26_C>();

	assertActive(machine, all, {
		FSM::stateId<Apex  >(),
		FSM::stateId<S26_O >(),
		FSM::stateId<S26_I >(),
		FSM::stateId<S26_C >(),
		FSM::stateId<S26_C1>(),
		FSM::stateId<S26_R >(),
		FSM::stateId<S26_R0>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step3(FSM::Instance& machine) {
	machine.immediateUtilize<S26_R>();

	assertActive(machine, all, {
		FSM::stateId<Apex  >(),
		FSM::stateId<S26_O >(),
		FSM::stateId<S26_I >(),
		FSM::stateId<S26_C >(),
		FSM::stateId<S26_C0>(),
		FSM::stateId<S26_R >(),
		FSM::stateId<S26_R2>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void step4(FSM::Instance& machine) {
	machine.immediateUtilize<S00_R>();

	assertActive(machine, all, {
		FSM::stateId<Apex  >(),
		FSM::stateId<S00_O >(),
		FSM::stateId<S00_I >(),
		FSM::stateId<S00_C >(),
		FSM::stateId<S00_C0>(),
		FSM::stateId<S00_R >(),
		FSM::stateId<S00_R2>(),
	});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TEST_CASE("FSM.Types") {
	Logger logger;
	FSM::Instance machine{&logger};

	step0(machine);
	step1(machine);
	step2(machine);
	step3(machine);
	step4(machine);
}

////////////////////////////////////////////////////////////////////////////////

}

#endif
