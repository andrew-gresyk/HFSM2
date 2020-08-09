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

TEST_CASE("FSM.Resumable", "[machine]") {
	Logger logger;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		FSM::Instance machine{&logger};
		{
			logger.assertSequence({
				{ FSM::stateId<Apex>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<I   >(),	Event::ENTRY_GUARD },

				{ FSM::stateId<Apex>(),	Event::CONSTRUCT },
				{ FSM::stateId<I   >(),	Event::CONSTRUCT },

				{ FSM::stateId<Apex>(),	Event::ENTER },
				{ FSM::stateId<I   >(),	Event::ENTER },
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
				{						Event::CHANGE, FSM::stateId<OR_2>() },
				{						Event::CHANGE, FSM::stateId<OC_2>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I   >(),	Event::UPDATE },

				{ FSM::stateId<I   >(),	Event::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_2>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::EXIT },

				{ FSM::stateId<I   >(),	Event::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OC_2>(),	Event::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::ENTER },
				{ FSM::stateId<OR  >(),	Event::ENTER },
				{ FSM::stateId<OR_2>(),	Event::ENTER },
				{ FSM::stateId<OC  >(),	Event::ENTER },
				{ FSM::stateId<OC_2>(),	Event::ENTER },
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
				{						Event::RESUME, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<O   >(),	Event::UPDATE },
				{ FSM::stateId<OR  >(),	Event::UPDATE },
				{ FSM::stateId<OR_2>(),	Event::UPDATE },
				{ FSM::stateId<OC  >(),	Event::UPDATE },
				{ FSM::stateId<OC_2>(),	Event::UPDATE },

				{ FSM::stateId<O   >(),	Event::EXIT_GUARD },
				{ FSM::stateId<OR  >(),	Event::EXIT_GUARD },
				{ FSM::stateId<OR_2>(),	Event::EXIT_GUARD },
				{ FSM::stateId<OC  >(),	Event::EXIT_GUARD },
				{ FSM::stateId<OC_2>(),	Event::EXIT_GUARD },
				{ FSM::stateId<I   >(),	Event::ENTRY_GUARD },

				{ FSM::stateId<OR_2>(),	Event::EXIT },
				{ FSM::stateId<OR  >(),	Event::EXIT },
				{ FSM::stateId<OC_2>(),	Event::EXIT },
				{ FSM::stateId<OC  >(),	Event::EXIT },
				{ FSM::stateId<O   >(),	Event::EXIT },

				{ FSM::stateId<OR_2>(),	Event::DESTRUCT },
				{ FSM::stateId<OR  >(),	Event::DESTRUCT },
				{ FSM::stateId<OC_2>(),	Event::DESTRUCT },
				{ FSM::stateId<OC  >(),	Event::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::DESTRUCT },
				{ FSM::stateId<I   >(),	Event::CONSTRUCT },

				{ FSM::stateId<I   >(),	Event::ENTER },
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
				{						Event::CHANGE, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I   >(),	Event::UPDATE },

				{ FSM::stateId<I   >(),	Event::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::EXIT },

				{ FSM::stateId<I   >(),	Event::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OC_1>(),	Event::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::ENTER },
				{ FSM::stateId<OR  >(),	Event::ENTER },
				{ FSM::stateId<OR_2>(),	Event::ENTER },
				{ FSM::stateId<OC  >(),	Event::ENTER },
				{ FSM::stateId<OC_1>(),	Event::ENTER },
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
				{						Event::RESTART, FSM::stateId<O   >() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I   >(),	Event::UPDATE },

				{ FSM::stateId<I   >(),	Event::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_1>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_1>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::EXIT },

				{ FSM::stateId<I   >(),	Event::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR_1>(),	Event::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OC_1>(),	Event::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::ENTER },
				{ FSM::stateId<OR  >(),	Event::ENTER },
				{ FSM::stateId<OR_1>(),	Event::ENTER },
				{ FSM::stateId<OC  >(),	Event::ENTER },
				{ FSM::stateId<OC_1>(),	Event::ENTER },
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
				{						Event::RESUME, FSM::stateId<Apex>() },

				{ FSM::stateId<Apex>(),	Event::UPDATE },
				{ FSM::stateId<I   >(),	Event::UPDATE },

				{ FSM::stateId<I   >(),	Event::EXIT_GUARD },
				{ FSM::stateId<O   >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OR_2>(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC  >(),	Event::ENTRY_GUARD },
				{ FSM::stateId<OC_2>(),	Event::ENTRY_GUARD },

				{ FSM::stateId<I   >(),	Event::EXIT },

				{ FSM::stateId<I   >(),	Event::DESTRUCT },
				{ FSM::stateId<O   >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OR_2>(),	Event::CONSTRUCT },
				{ FSM::stateId<OC  >(),	Event::CONSTRUCT },
				{ FSM::stateId<OC_2>(),	Event::CONSTRUCT },

				{ FSM::stateId<O   >(),	Event::ENTER },
				{ FSM::stateId<OR  >(),	Event::ENTER },
				{ FSM::stateId<OR_2>(),	Event::ENTER },
				{ FSM::stateId<OC  >(),	Event::ENTER },
				{ FSM::stateId<OC_2>(),	Event::ENTER },
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
