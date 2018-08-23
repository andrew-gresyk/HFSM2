namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _Q
	: _C<NHeadID, NCompoID, NOrthoID, TArgs, THead, TSubStates...>
{
	static constexpr StateID HEAD_ID  = NHeadID;
	static constexpr StateID COMPO_ID = NCompoID;
	static constexpr StateID ORTHO_ID = NOrthoID;

	using Args				= TArgs;
	using Head				= THead;
	using Composite			= _C<HEAD_ID, COMPO_ID, ORTHO_ID, Args, Head, TSubStates...>;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using PlanControl		= typename Composite::PlanControl;
	using Plan				= typename PlanControl::Plan;
	using ControlLock		= typename Composite::ControlLock;
	using ControlRegion		= typename Composite::ControlRegion;
	using FullControl		= typename Composite::FullControl;
	using HeadState			= typename Composite::State;

	using SubStateList		= typename Composite::SubStateList;

	using ControlOrigin		= ControlOriginT<Context, StateList, Args::FORK_COUNT>;

	using Composite::Composite;

	inline void	  deepGuard (FullControl& control);

	inline Status deepUpdate(FullControl& control);

	template <typename TEvent>
	inline void   deepReact	(const TEvent& event,
							 FullControl& control);

	inline void	  deepExit  (PlanControl& control);

	using Composite::_fork;
	using Composite::_state;
	using Composite::_subStates;

	bool _success = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_sequence.inl"
