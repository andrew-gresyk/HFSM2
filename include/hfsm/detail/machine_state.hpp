namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename TH>
struct M<TContext, TMaxSubstitutions>::_S {
	using Head = TH;

	enum : unsigned {
		ReverseDepth = 1,
		DeepWidth	 = 0,
		StateCount	 = 1,
		ForkCount	 = 0,
		ProngCount	 = 0,
		Width		 = 1,
	};

	_S(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute	(Control&,		   Context&,		 LoggerInterface* const)		{}
	inline bool deepSubstitute			(Control& control, Context& context, LoggerInterface* const logger);

	inline void deepEnterInitial		(				   Context& context, LoggerInterface* const logger);
	inline void deepEnter				(				   Context& context, LoggerInterface* const logger);

	inline bool deepUpdateAndTransition	(Control& control, Context& context, LoggerInterface* const logger);
	inline void deepUpdate				(				   Context& context, LoggerInterface* const logger);

	template <typename TEvent>
	inline void deepReact				(const TEvent& event,
										 Control& control, Context& context, LoggerInterface* const logger);

	inline void deepLeave				(				   Context& context, LoggerInterface* const logger);

	inline void deepForwardRequest(const enum Transition::Type)												{}
	inline void deepRequestRemain()																			{}
	inline void deepRequestRestart()																		{}
	inline void deepRequestResume()																			{}
	inline void deepChangeToRequested	(				   Context&,		 LoggerInterface* const)		{}

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()		{ return std::is_same<Head, Base>::value; }

	enum : unsigned {
		NameCount	 = isBare() ? 0 : 1,
	};
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static const char* name();

	void deepGetNames(const unsigned parent,
					  const enum StateInfo::RegionType region,
					  const unsigned depth,
					  StateInfos& stateInfos) const;

	void deepIsActive(const bool isActive,
					  unsigned& index,
					  MachineStructure& structure) const;
#endif

#ifdef HFSM_ENABLE_LOG_INTERFACE
	static const char* fullName();

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
		logger.record(typeid(Head), fullName(), TMethodId, methodName(TMethodId));
	}
#endif

	Head _head;

	HSFM_IF_DEBUG(const TypeInfo _type = TypeInfo::get<Head>());
};

////////////////////////////////////////////////////////////////////////////////

}

#include "machine_state_methods.inl"
