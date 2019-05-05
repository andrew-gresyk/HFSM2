#include "shared.hpp"

namespace test_state_data {

//------------------------------------------------------------------------------

struct Context {};

using M = hfsm2::MachineT<Context, bool>;

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

static_assert(FSM::Instance::STATE_COUNT == 5, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_COUNT == 1, "COMPO_COUNT");
static_assert(FSM::Instance::ORTHO_COUNT == 1, "ORTHO_COUNT");
static_assert(FSM::Instance::ORTHO_UNITS == 1, "ORTHO_UNITS");
static_assert(FSM::Instance::PRONG_COUNT == 2, "PRONG_COUNT");

////////////////////////////////////////////////////////////////////////////////

}
