namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext,
		  typename TPayloadList,
		  typename THead>
struct _S final {
	using Context			= TContext;
	using Control			= ControlT<Context>;
	using PayloadList		= TPayloadList;
	using StateRegistryBase	= StateRegistryBaseT<PayloadList>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionControl	= TransitionControlT<Context, PayloadList>;
	using Head				= THead;
	using StateList			= TypeListT<Head>;
	HSFM_IF_DEBUG(StateList stateList);

	using Base				= ::hfsm::detail::Base<Context, PayloadList>;

	enum : LongIndex {
		REVERSE_DEPTH = 1,
		DEEP_WIDTH	  = 0,
		STATE_COUNT	  = 1,
		FORK_COUNT	  = 0,
		PRONG_COUNT	  = 0,
		WIDTH		  = 1,
	};

	_S(StateRegistryBase& stateRegistry,
	   const Parent parent,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepRegister		 (StateRegistryBase& stateRegistry, const Parent parent);

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

	enum : LongIndex {
		NAME_COUNT	 = isBare() ? 0 : 1,
	};
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const enum StructureStateInfo::RegionType region,
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
		logger.recordMethod(typeid(Head), TMethodId);
	}
#endif

	Head _head;

	HSFM_IF_DEBUG(const std::type_index _type = typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_state.inl"
