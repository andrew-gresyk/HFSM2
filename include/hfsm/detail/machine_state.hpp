namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TStateID,
		  typename TContext,
		  typename TStateList,
		  typename TPayloadList,
		  typename THead>
struct _S final {
	static constexpr StateID STATE_ID = TStateID;

	using Context			= TContext;
	using Control			= ControlT<Context>;
	using StateList			= TStateList;
	using PayloadList		= TPayloadList;
	using StateRegistry2	= StateRegistry2T<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, StateList, PayloadList>;
	using Head				= THead;

	using Base				= ::hfsm::detail::Base<Context, StateList, PayloadList>;

	static constexpr LongIndex REVERSE_DEPTH = 1;
	static constexpr LongIndex DEEP_WIDTH	 = 0;
	static constexpr LongIndex STATE_COUNT	 = 1;
	static constexpr LongIndex FORK_COUNT	 = 0;
	static constexpr LongIndex PRONG_COUNT	 = 0;
	static constexpr LongIndex WIDTH		 = 1;

	_S(StateRegistry2& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardGuard	 (TransitionControl&)							{}
	inline bool deepGuard			 (TransitionControl& control);

	inline void deepEnterInitial	 (Control& control);
	inline void deepEnter			 (Control& control);

	inline bool deepUpdate			 (TransitionControl& control);

	template <typename TEvent>
	inline void deepReact			 (const TEvent& event, TransitionControl& control);

	inline void deepExit			 (Control& control);

	inline void deepForwardRequest	 (const typename Transition::Type)					{}
	inline void deepRequestRemain	 ()												{}
	inline void deepRequestRestart	 ()												{}
	inline void deepRequestResume	 ()												{}
	inline void deepChangeToRequested(Control&)										{}

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Base>::value;		 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  LongIndex& index,
					  MachineStructure& structure) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	template <typename>
	struct MemberTraits;

	template <typename TReturn, typename TState, typename... TArgs>
	struct MemberTraits<TReturn(TState::*)(TArgs...)> {
		using State = TState;
	};

	template <typename TMethodType, LoggerInterface::Method>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Base>::value>::type
	log(LoggerInterface&) const {}

	template <typename TMethodType, LoggerInterface::Method TMethodId>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Base>::value>::type
	log(LoggerInterface& logger) const {
		logger.recordMethod(STATE_ID, TMethodId);
	}
#endif

	Head _head;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_state.inl"
