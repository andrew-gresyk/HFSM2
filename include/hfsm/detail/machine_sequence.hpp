namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID THeadID,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _Q
	: _C<THeadID, TArgs, THead, TSubStates...>
{
	static constexpr StateID HEAD_ID = THeadID;

	using Args				= TArgs;
	using Head				= THead;
	using Composite			= _C<HEAD_ID, Args, Head, TSubStates...>;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using PlanControl		= typename Composite::PlanControl;
	using FullControl		= typename Composite::FullControl;
	using HeadState			= typename Composite::State;

	using Composite::Composite;

	inline void	  deepGuard (FullControl& control);

	inline Status deepUpdate(FullControl& control);

	template <typename TEvent>
	inline void   deepReact	(const TEvent& event,
							 FullControl& control);

	inline void	  deepExit  (PlanControl& control);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_sequence.inl"
