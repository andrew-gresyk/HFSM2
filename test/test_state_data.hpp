#include "shared.hpp"

namespace test_state_data {

//------------------------------------------------------------------------------

using M = hfsm2::MachineT<hfsm2::Config::PayloadT<bool>>;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::OrthogonalRoot<S(Apex),
				S(Shotgun),
				M::Composite<S(Top),
					S(Origin),
					S(Destination)
				>
			>;

#undef S

static_assert(FSM::regionId<Apex>()		  == 0, "");
static_assert(FSM::regionId<Top>()		  == 1, "");

static_assert(FSM::stateId<Apex>()		  == 0, "");
static_assert(FSM::stateId<Shotgun>()	  == 1, "");
static_assert(FSM::stateId<Top>()		  == 2, "");
static_assert(FSM::stateId<Origin>()	  == 3, "");
static_assert(FSM::stateId<Destination>() == 4, "");

//------------------------------------------------------------------------------

struct Apex			: FSM::State {};
struct Shotgun		: FSM::State {};
struct Top			: FSM::State {};
struct Origin		: FSM::State {};
struct Destination	: FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 5, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS == 1, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 2, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS == 1, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   == 1, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}
