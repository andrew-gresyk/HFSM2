// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_resume {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::ResumableRoot<S(Apex),
				S(I),
				M::Orthogonal<S(O),
					M::Resumable<S(OR),
						S(OR_1),
						S(OR_2)
					>,
					M::Composite<S(OC),
						S(OC_1),
						S(OC_2)
					>
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()	== 0, "");
static_assert(FSM::regionId<O   >()	== 1, "");
static_assert(FSM::regionId<OR  >()	== 2, "");
static_assert(FSM::regionId<OC  >()	== 3, "");

static_assert(FSM::stateId<Apex>() == 0, "");
static_assert(FSM::stateId<I   >() == 1, "");
static_assert(FSM::stateId<O   >() == 2, "");
static_assert(FSM::stateId<OR  >() == 3, "");
static_assert(FSM::stateId<OR_1>() == 4, "");
static_assert(FSM::stateId<OR_2>() == 5, "");
static_assert(FSM::stateId<OC  >() == 6, "");
static_assert(FSM::stateId<OC_1>() == 7, "");
static_assert(FSM::stateId<OC_2>() == 8, "");

//------------------------------------------------------------------------------

struct Apex	: FSM::State {};
struct I	: FSM::State {};
struct O	: FSM::State {};
struct OR	: FSM::State {};
struct OR_1	: FSM::State {};
struct OR_2	: FSM::State {};
struct OC	: FSM::State {};
struct OC_1	: FSM::State {};
struct OC_2	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT   == 9, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT  == 4, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS == 3, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS  == 6, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS == 1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS   == 1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

const Types all = {
	FSM::stateId<I   >(),
	FSM::stateId<O   >(),
	FSM::stateId<OR  >(),
	FSM::stateId<OR_1>(),
	FSM::stateId<OR_2>(),
	FSM::stateId<OC  >(),
	FSM::stateId<OC_1>(),
	FSM::stateId<OC_2>(),
};

//------------------------------------------------------------------------------

TEST_CASE("FSM.Resumable") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<I   >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<Apex>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<I   >(),	Event::Type::CONSTRUCT },

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

		machine.changeTo<OR_2>();
		machine.changeTo<OC_2>();
		machine.update();
		{
			logger.assertSequence({
				{						Event::Type::CHANGE, FSM::stateId<OR_2>() },
				{						Event::Type::CHANGE, FSM::stateId<OC_2>() },

				{ FSM::stateId<Apex>(),	Event::Type::UPDATE },
				{ FSM::stateId<I   >(),	Event::Type::UPDATE },

				{ FSM::stateId<I   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC_2>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::Type::EXIT },

				{ FSM::stateId<I   >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC_2>(),	Event::Type::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::Type::ENTER },
				{ FSM::stateId<OR  >(),	Event::Type::ENTER },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTER },
				{ FSM::stateId<OC  >(),	Event::Type::ENTER },
				{ FSM::stateId<OC_2>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<OR  >(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC  >(),
				FSM::stateId<OC_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		machine.resume<Apex>();
		machine.update();
		{
			logger.assertSequence({
				{						Event::Type::RESUME, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::Type::UPDATE },
				{ FSM::stateId<O   >(),	Event::Type::UPDATE },
				{ FSM::stateId<OR  >(),	Event::Type::UPDATE },
				{ FSM::stateId<OR_2>(),	Event::Type::UPDATE },
				{ FSM::stateId<OC  >(),	Event::Type::UPDATE },
				{ FSM::stateId<OC_2>(),	Event::Type::UPDATE },

				{ FSM::stateId<O   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<OR  >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<OR_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<OC  >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<OC_2>(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<I   >(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<OR_2>(),	Event::Type::EXIT },
				{ FSM::stateId<OR  >(),	Event::Type::EXIT },
				{ FSM::stateId<OC_2>(),	Event::Type::EXIT },
				{ FSM::stateId<OC  >(),	Event::Type::EXIT },
				{ FSM::stateId<O   >(),	Event::Type::EXIT },

				{ FSM::stateId<OR_2>(),	Event::Type::DESTRUCT },
				{ FSM::stateId<OR  >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<OC_2>(),	Event::Type::DESTRUCT },
				{ FSM::stateId<OC  >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<I   >(),	Event::Type::CONSTRUCT },

				{ FSM::stateId<I   >(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<Apex>(),
				FSM::stateId<I   >(),
			});

			assertResumable(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<OR  >(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC  >(),
				FSM::stateId<OC_2>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Changing to Apex") {
			machine.changeTo<Apex>();
			machine.update();

			logger.assertSequence({
				{						Event::Type::CHANGE, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::Type::UPDATE },
				{ FSM::stateId<I   >(),	Event::Type::UPDATE },

				{ FSM::stateId<I   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::Type::EXIT },

				{ FSM::stateId<I   >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC_1>(),	Event::Type::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::Type::ENTER },
				{ FSM::stateId<OR  >(),	Event::Type::ENTER },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTER },
				{ FSM::stateId<OC  >(),	Event::Type::ENTER },
				{ FSM::stateId<OC_1>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<OR  >(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC  >(),
				FSM::stateId<OC_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
				FSM::stateId<OC_2>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Restarting O") {
			machine.restart<O>();
			machine.update();

			logger.assertSequence({
				{						Event::Type::RESTART, FSM::stateId<O   >() },

				{ FSM::stateId<Apex>(),	Event::Type::UPDATE },
				{ FSM::stateId<I   >(),	Event::Type::UPDATE },

				{ FSM::stateId<I   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR_1>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::Type::EXIT },

				{ FSM::stateId<I   >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR_1>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC_1>(),	Event::Type::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::Type::ENTER },
				{ FSM::stateId<OR  >(),	Event::Type::ENTER },
				{ FSM::stateId<OR_1>(),	Event::Type::ENTER },
				{ FSM::stateId<OC  >(),	Event::Type::ENTER },
				{ FSM::stateId<OC_1>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<OR  >(),
				FSM::stateId<OR_1>(),
				FSM::stateId<OC  >(),
				FSM::stateId<OC_1>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC_2>(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		WHEN("Resuming Apex") {
			machine.resume<Apex>();
			machine.update();

			logger.assertSequence({
				{						Event::Type::RESUME, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::Type::UPDATE },
				{ FSM::stateId<I   >(),	Event::Type::UPDATE },

				{ FSM::stateId<I   >(),	Event::Type::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::Type::ENTRY_GUARD },
				{ FSM::stateId<OC_2>(),	Event::Type::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::Type::EXIT },

				{ FSM::stateId<I   >(),	Event::Type::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::Type::CONSTRUCT },
				{ FSM::stateId<OC_2>(),	Event::Type::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::Type::ENTER },
				{ FSM::stateId<OR  >(),	Event::Type::ENTER },
				{ FSM::stateId<OR_2>(),	Event::Type::ENTER },
				{ FSM::stateId<OC  >(),	Event::Type::ENTER },
				{ FSM::stateId<OC_2>(),	Event::Type::ENTER },
			});

			assertActive(machine, all, {
				FSM::stateId<O   >(),
				FSM::stateId<OR  >(),
				FSM::stateId<OR_2>(),
				FSM::stateId<OC  >(),
				FSM::stateId<OC_2>(),
			});

			assertResumable(machine, all, {
				FSM::stateId<I   >(),
			});
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	}
}

////////////////////////////////////////////////////////////////////////////////

}
