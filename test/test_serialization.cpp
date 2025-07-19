// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#define HFSM2_ENABLE_SERIALIZATION
#define HFSM2_ENABLE_VERBOSE_DEBUG_LOG
#include "tools.hpp"

using namespace test_tools;

namespace test_serialization {

////////////////////////////////////////////////////////////////////////////////

using Config = hfsm2::Config
					::ManualActivation;

using M = hfsm2::MachineT<Config>;

//------------------------------------------------------------------------------

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

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::Info::STATE_COUNT	  == 13, "");
static_assert(FSM::Instance::Info::REGION_COUNT	  ==  6, "");
static_assert(FSM::Instance::Info::COMPO_COUNT	  ==  5, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS	  == 10, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT	  ==  1, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS	  ==  1, "");
static_assert(FSM::Instance::Info::ACTIVE_BITS	  ==  3, "");
static_assert(FSM::Instance::Info::RESUMABLE_BITS == 10, "");

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

static_assert(FSM::Instance::Info::STATE_COUNT	  == 13, "");
static_assert(FSM::Instance::Info::REGION_COUNT	  ==  6, "");
static_assert(FSM::Instance::Info::COMPO_COUNT	  ==  5, "");
static_assert(FSM::Instance::Info::COMPO_PRONGS	  == 10, "");
static_assert(FSM::Instance::Info::ORTHO_COUNT	  ==  1, "");
static_assert(FSM::Instance::Info::ORTHO_UNITS	  ==  1, "");
static_assert(FSM::Instance::Info::ACTIVE_BITS	  ==  3, "");
static_assert(FSM::Instance::Info::RESUMABLE_BITS == 10, "");

}

//------------------------------------------------------------------------------

static_assert(server::FSM::Instance::Info::STATE_COUNT	  == client::FSM::Instance::Info::STATE_COUNT	, "");
static_assert(server::FSM::Instance::Info::REGION_COUNT	  == client::FSM::Instance::Info::REGION_COUNT	, "");
static_assert(server::FSM::Instance::Info::COMPO_COUNT	  == client::FSM::Instance::Info::COMPO_COUNT	, "");
static_assert(server::FSM::Instance::Info::COMPO_PRONGS	  == client::FSM::Instance::Info::COMPO_PRONGS	, "");
static_assert(server::FSM::Instance::Info::ORTHO_COUNT	  == client::FSM::Instance::Info::ORTHO_COUNT	, "");
static_assert(server::FSM::Instance::Info::ORTHO_UNITS	  == client::FSM::Instance::Info::ORTHO_UNITS	, "");
static_assert(server::FSM::Instance::Info::ACTIVE_BITS	  == client::FSM::Instance::Info::ACTIVE_BITS	, "");
static_assert(server::FSM::Instance::Info::RESUMABLE_BITS == client::FSM::Instance::Info::RESUMABLE_BITS, "");

////////////////////////////////////////////////////////////////////////////////

void step1(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);
	replicated.load(buffer);

	logger.assertSequence({});

	assertActive(authority,  {});
	assertActive(replicated, {});

	assertResumable(authority,  {});
	assertResumable(replicated, {});
}

//------------------------------------------------------------------------------

void step2(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.enter();

	server::FSM::Instance::SerialBuffer serverBuffer;
	authority .save(serverBuffer);
	replicated.load(serverBuffer);

	logger.assertSequence({
		{ hfsm2::ROOT_ID                          , Event::Type::ENTER },
		{ client::FSM::stateId<client::C1      >(),	Event::Type::ENTER },
		{ client::FSM::stateId<client::C1_S1   >(),	Event::Type::ENTER },
	});

	assertActive(authority,  {
		hfsm2::ROOT_ID                          ,
		server::FSM::stateId<server::C1      >(),
		server::FSM::stateId<server::C1_S1   >(),
	});

	assertActive(replicated, {
		hfsm2::ROOT_ID                          ,
		client::FSM::stateId<client::C1      >(),
		client::FSM::stateId<client::C1_S1   >(),
	});

	assertResumable(authority,  {});
	assertResumable(replicated, {});
}

//------------------------------------------------------------------------------

void step3(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated)
{
	server::FSM::Instance::SerialBuffer serverBuffer;
	server::FSM::Instance::SerialBuffer clientBuffer;

	authority .save(serverBuffer);
	replicated.save(clientBuffer);

	REQUIRE		 (clientBuffer == serverBuffer);
	REQUIRE_FALSE(clientBuffer != serverBuffer);
}

//------------------------------------------------------------------------------

void step4(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.immediateChangeTo<server::O2_C1_S2>();

	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);
	replicated.load(buffer);

	logger.assertSequence({
		{ client::FSM::stateId<client::C1_S1   >(),	Event::Type::EXIT },
		{ client::FSM::stateId<client::C1      >(),	Event::Type::EXIT },

		{ client::FSM::stateId<client::O2      >(),	Event::Type::ENTER },
		{ client::FSM::stateId<client::O2_C1   >(),	Event::Type::ENTER },
		{ client::FSM::stateId<client::O2_C1_S2>(),	Event::Type::ENTER },
		{ client::FSM::stateId<client::O2_C2   >(),	Event::Type::ENTER },
		{ client::FSM::stateId<client::O2_C2_S1>(),	Event::Type::ENTER },
	});

	assertActive(authority,  {
		hfsm2::ROOT_ID                          ,
		server::FSM::stateId<server::O2      >(),
		server::FSM::stateId<server::O2_C1   >(),
		server::FSM::stateId<server::O2_C1_S2>(),
		server::FSM::stateId<server::O2_C2   >(),
		server::FSM::stateId<server::O2_C2_S1>(),
	});

	assertActive(replicated, {
		hfsm2::ROOT_ID                          ,
		client::FSM::stateId<client::O2      >(),
		client::FSM::stateId<client::O2_C1   >(),
		client::FSM::stateId<client::O2_C1_S2>(),
		client::FSM::stateId<client::O2_C2   >(),
		client::FSM::stateId<client::O2_C2_S1>(),
	});

	assertResumable(authority,  {
		server::FSM::stateId<server::C1      >(),
		server::FSM::stateId<server::C1_S1   >(),
	});

	assertResumable(replicated, {
		client::FSM::stateId<client::C1      >(),
		client::FSM::stateId<client::C1_S1   >(),
	});
}

//------------------------------------------------------------------------------

void step5(server::FSM::Instance& authority,
		   client::FSM::Instance& replicated,
		   Logger& logger)
{
	authority.exit();

	server::FSM::Instance::SerialBuffer buffer;
	authority .save(buffer);
	replicated.load(buffer);

	logger.assertSequence({
		{ client::FSM::stateId<client::O2_C1_S2>(),	Event::Type::EXIT },
		{ client::FSM::stateId<client::O2_C1   >(),	Event::Type::EXIT },
		{ client::FSM::stateId<client::O2_C2_S1>(),	Event::Type::EXIT },
		{ client::FSM::stateId<client::O2_C2   >(),	Event::Type::EXIT },
		{ client::FSM::stateId<client::O2      >(),	Event::Type::EXIT },
		{ hfsm2::ROOT_ID                          , Event::Type::EXIT },
	});

	assertActive(authority,  {});
	assertActive(replicated, {});

	assertResumable(authority,  {});
	assertResumable(replicated, {});
}

//------------------------------------------------------------------------------

TEST_CASE("FSM.Serialization") {
	Logger logger;

	{
		server::FSM::Instance authority;
		client::FSM::Instance replicated{&logger};

		step1(authority, replicated, logger);
		step2(authority, replicated, logger);
		step3(authority, replicated);
		step4(authority, replicated, logger);
		step5(authority, replicated, logger);
	}

	logger.assertSequence({});
}

////////////////////////////////////////////////////////////////////////////////

}
