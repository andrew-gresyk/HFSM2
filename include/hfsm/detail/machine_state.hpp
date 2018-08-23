namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TArgs,
		  typename THead>
struct _S {
	static constexpr StateID STATE_ID = TStateID;

	using Args				= TArgs;
	using Head				= THead;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using Control			= ControlT			   <Context, StateList, Args::FORK_COUNT>;
	using ControlOrigin		= ControlOriginT	   <Context, StateList, Args::FORK_COUNT>;
	using PlanControl		= PlanControlT		   <Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT		   <PayloadList>;
	using TransitionControl	= TransitionControlT   <Context, StateList, Args::FORK_COUNT, PayloadList>;
	using FullControl		= FullControlT		   <Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using StateBase			= ::hfsm::detail::State<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	_S(StateParents& stateParents,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void	  deepForwardGuard		(FullControl&)									{}
	inline bool	  deepGuard				(FullControl& control);

	inline void	  deepEnterInitial		(PlanControl& control);
	inline void	  deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void	  deepReact				(const TEvent& event,
										 FullControl& control);

	inline void	  deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(const typename Transition::Type)				{}
	inline void   deepRequestRemain		()												{}
	inline void   deepRequestRestart	()												{}
	inline void   deepRequestResume		()												{}
	inline void   deepChangeToRequested	(Control&)										{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, StateBase>::value;		 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... Ts>
	struct MemberTraits<TReturn(TState::*)(Ts...)> {
		using State = TState;
	};

	template <typename TMethodType, LoggerInterface::Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, StateBase>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, LoggerInterface::Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, StateBase>::value>::type
	log(LoggerInterface& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	Head _head;
	HSFM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_state.inl"
