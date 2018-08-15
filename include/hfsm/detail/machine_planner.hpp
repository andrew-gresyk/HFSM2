namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TArgs,
		  typename THead>
struct _P final
	: _S<TStateID, TArgs, THead>
{
	static constexpr auto STATE_ID = TStateID;

	using Args				= TArgs;
	using Head				= THead;
	using State				= _S<STATE_ID, Args, Head>;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateRegistry		= typename State::StateRegistry;
	using ControlOrigin		= typename State::ControlOrigin;
	using PlanControl		= typename State::PlanControl;
	using Transition		= typename State::Transition;
	using FullControl		= typename State::FullControl;

	using StateBase			= typename State::StateBase;

	using State::State;

	inline bool	  deepGuard (FullControl& control);
	inline void	  deepEnter (PlanControl& control);
	inline Status deepUpdate(FullControl& control);
	inline void	  deepExit  (PlanControl& control);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_planner.inl"
