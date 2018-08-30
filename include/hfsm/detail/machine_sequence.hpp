namespace hfsm2 {
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
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;

	using Args			= TArgs;
	using Head			= THead;
	using Composite		= _C<HEAD_ID, COMPO_INDEX, ORTHO_INDEX, Args, Head, TSubStates...>;

	using Control		= ControlT	  <Args>;
	using ControlOrigin	= typename Control::Origin;
	using ControlRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using Plan			= typename Control::Plan;

	using HeadState		= typename Composite::HeadState;
	using SubStateList	= typename Composite::SubStateList;

	static constexpr ShortIndex REGION_SIZE	= SubStateList::SIZE;

	using Composite::Composite;

	inline void	  deepGuard (FullControl& control);

	inline Status deepUpdate(FullControl& control);

	template <typename TEvent>
	inline void   deepReact	(const TEvent& event,
							 FullControl& control);

	inline void	  deepExit  (Control& control);

	using Composite::_headState;
	using Composite::_subStates;

	bool _success = false;	// TODO: use BitArray for orthogonal
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_sequence.inl"
