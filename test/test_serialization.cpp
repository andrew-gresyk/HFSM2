#define HFSM2_ENABLE_SERIALIZATION
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

namespace test_serialization {

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
struct C1_C2_S2 : FSM::State {};
struct O2		: FSM::State {};
struct O2_C1	: FSM::State {};
struct O2_C1_S1	: FSM::State {};
struct O2_C1_S2	: FSM::State {};
struct O2_C2	: FSM::State {};
struct O2_C2_S1	: FSM::State {};
struct O2_C2_S2	: FSM::State {};

//------------------------------------------------------------------------------

static_assert(FSM::Instance::Info::STATE_COUNT	  == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT	  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS  ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS	  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS  ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS	  ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::Info::ACTIVE_BITS	  ==  3, "");
static_assert(FSM::Instance::Info::RESUMABLE_BITS == 10, "");

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

static_assert(FSM::Instance::Info::STATE_COUNT	  == 13, "STATE_COUNT");
static_assert(FSM::Instance::Info::REGION_COUNT	  ==  6, "REGION_COUNT");
static_assert(FSM::Instance::Info::COMPO_REGIONS  ==  5, "COMPO_REGIONS");
static_assert(FSM::Instance::Info::COMPO_PRONGS	  == 10, "COMPO_PRONGS");
static_assert(FSM::Instance::Info::ORTHO_REGIONS  ==  1, "ORTHO_REGIONS");
static_assert(FSM::Instance::Info::ORTHO_UNITS	  ==  1, "ORTHO_UNITS");
static_assert(FSM::Instance::Info::ACTIVE_BITS	  ==  3, "");
static_assert(FSM::Instance::Info::RESUMABLE_BITS == 10, "");

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

static_assert(server::FSM::Instance::Info::STATE_COUNT	  == client::FSM::Instance::Info::STATE_COUNT	, "STATE_COUNT");
static_assert(server::FSM::Instance::Info::REGION_COUNT	  == client::FSM::Instance::Info::REGION_COUNT	, "REGION_COUNT");
static_assert(server::FSM::Instance::Info::COMPO_REGIONS  == client::FSM::Instance::Info::COMPO_REGIONS	, "COMPO_REGIONS");
static_assert(server::FSM::Instance::Info::COMPO_PRONGS	  == client::FSM::Instance::Info::COMPO_PRONGS	, "COMPO_PRONGS");
static_assert(server::FSM::Instance::Info::ORTHO_REGIONS  == client::FSM::Instance::Info::ORTHO_REGIONS	, "ORTHO_REGIONS");
static_assert(server::FSM::Instance::Info::ORTHO_UNITS	  == client::FSM::Instance::Info::ORTHO_UNITS	, "ORTHO_UNITS");
static_assert(server::FSM::Instance::Info::ACTIVE_BITS	  == client::FSM::Instance::Info::ACTIVE_BITS	, "ACTIVE_BITS");
static_assert(server::FSM::Instance::Info::RESUMABLE_BITS == client::FSM::Instance::Info::RESUMABLE_BITS, "RESUMABLE_BITS");

////////////////////////////////////////////////////////////////////////////////

TEST_CASE("FSM.Serialization", "[machine]") {
	Logger logger;

	server::FSM::Instance::SerialBuffer buffer;

	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		server::FSM::Instance authority;
		client::FSM::Instance replicated{&logger};
		{
			logger.assertSequence({
				{ hfsm2::StateID{0},						Event::ENTRY_GUARD },
				{ client::FSM::stateId<client::C1      >(),	Event::ENTRY_GUARD },
				{ client::FSM::stateId<client::C1_S1   >(),	Event::ENTRY_GUARD },

				{ hfsm2::StateID{0},						Event::CONSTRUCT },
				{ client::FSM::stateId<client::C1      >(),	Event::CONSTRUCT },
				{ client::FSM::stateId<client::C1_S1   >(),	Event::CONSTRUCT },

				{ hfsm2::StateID{0}, 						Event::ENTER },
				{ client::FSM::stateId<client::C1      >(),	Event::ENTER },
				{ client::FSM::stateId<client::C1_S1   >(),	Event::ENTER },
			});

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
		}
		authority.save(buffer);

		replicated.load(buffer);
		{
			logger.assertSequence({
				{ client::FSM::stateId<client::C1_S1   >(),	Event::EXIT },
				{ client::FSM::stateId<client::C1      >(),	Event::EXIT },
				{ hfsm2::StateID{0},						Event::EXIT },

				{ client::FSM::stateId<client::C1_S1   >(),	Event::DESTRUCT },
				{ client::FSM::stateId<client::C1      >(),	Event::DESTRUCT },
				{ hfsm2::StateID{0},						Event::DESTRUCT },

				{ hfsm2::StateID{0}, 						Event::CONSTRUCT },
				{ client::FSM::stateId<client::O2      >(),	Event::CONSTRUCT },
				{ client::FSM::stateId<client::O2_C1   >(),	Event::CONSTRUCT },
				{ client::FSM::stateId<client::O2_C1_S2>(),	Event::CONSTRUCT },
				{ client::FSM::stateId<client::O2_C2   >(),	Event::CONSTRUCT },
				{ client::FSM::stateId<client::O2_C2_S1>(),	Event::CONSTRUCT },

				{ hfsm2::StateID{0}, 						Event::ENTER },
				{ client::FSM::stateId<client::O2      >(),	Event::ENTER },
				{ client::FSM::stateId<client::O2_C1   >(),	Event::ENTER },
				{ client::FSM::stateId<client::O2_C1_S2>(),	Event::ENTER },
				{ client::FSM::stateId<client::O2_C2   >(),	Event::ENTER },
				{ client::FSM::stateId<client::O2_C2_S1>(),	Event::ENTER },
			});

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
	}
}

////////////////////////////////////////////////////////////////////////////////

}
