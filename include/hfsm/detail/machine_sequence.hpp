namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _Q
	: _C<NHeadIndex, NCompoIndex, NOrthoIndex, TArgs, THead, TSubStates...>
{
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;

	using Args				= TArgs;
	using Head				= THead;
	using Composite			= _C<HEAD_ID, COMPO_INDEX, ORTHO_INDEX, Args, Head, TSubStates...>;
	using HeadState			= typename Composite::HeadState;

	using ControlLock		= typename Composite::ControlLock;
	using ControlRegion		= typename Composite::ControlRegion;
	using PlanControl		= typename Composite::PlanControl;
	using FullControl		= typename Composite::FullControl;

	using Plan				= typename PlanControl::Plan;

	using SubStateList		= typename Composite::SubStateList;

	using ControlOrigin		= ControlOriginT<Args>;

	using Composite::Composite;

	inline void	  deepGuard (FullControl& control);

	inline Status deepUpdate(FullControl& control);

	template <typename TEvent>
	inline void   deepReact	(const TEvent& event,
							 FullControl& control);

	inline void	  deepExit  (PlanControl& control);

	using Composite::_headState;
	using Composite::_subStates;

	bool _success = false;	// TODO: use BitArray for orthogonal
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_sequence.inl"
