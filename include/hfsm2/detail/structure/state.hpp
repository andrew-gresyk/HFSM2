namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ {
	using Indices		= TIndices;
	static constexpr StateID	STATE_ID	= Indices::STATE_ID;
	static constexpr ShortIndex ORTHO_UNIT	= Indices::ORTHO_UNIT;

	using Args			= TArgs;
	using Head			= THead;

	using Context		= typename Args::Context;
	using Rank			= typename Args::Rank;
	using Utility		= typename Args::Utility;
	using UP			= typename Args::UP;
	using Logger		= typename Args::Logger;
	using Payload		= typename Args::Payload;

	using Request		= RequestT<Payload>;
	using RequestType	= typename Request::Type;

	using Control		= ControlT<Args>;
	using StateRegistry	= StateRegistryT<Args>;
	using StateParents	= typename StateRegistry::StateParents;

	using PlanControl	= PlanControlT<Args>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <Args>;
	using GuardControl	= GuardControlT<Args>;

	using Empty			= ::hfsm2::detail::Empty<Args>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

	template <typename TState>
	HFSM_INLINE		  TState& access()			{ return *reinterpret_cast<		 TState*>(0);	}

	template <typename TState>
	HFSM_INLINE	const TState& access() const	{ return *reinterpret_cast<const TState*>(0);	}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	template <>
	HFSM_INLINE		  Head&	  access()										{ return _head;	}

	template <>
	HFSM_INLINE const Head&	  access() const								{ return _head;	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE Parent	stateParent			 (Control& control)	{ return control._stateRegistry.stateParents[STATE_ID]; }

	HFSM_INLINE void	deepRegister		 (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl&)				{ return false;	}
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepEnter			 (PlanControl&	control);
	HFSM_INLINE void	deepReenter			 (PlanControl&	control);

	HFSM_INLINE Status	deepUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl&)				{ return false; }
	HFSM_INLINE bool	deepExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepExit			 (PlanControl&	control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wrapPlanSucceeded	 (FullControl&	control);
	HFSM_INLINE void	wrapPlanFailed		 (FullControl&	control);
	HFSM_INLINE Rank	wrapRank			 (Control& control);
	HFSM_INLINE Utility	wrapUtility			 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control&, const RequestType)					{}
	HFSM_INLINE void	deepForwardRequest	 (Control&, const RequestType)					{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control&)										{}
	HFSM_INLINE void	deepRequestRemain	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestRestart	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestResume	 (StateRegistry&)								{}
	HFSM_INLINE void	deepRequestUtilize	 (Control&)										{}
	HFSM_INLINE void	deepRequestRandomize (Control&)										{}

	HFSM_INLINE UP		deepReportChange	 (Control& control);
	HFSM_INLINE UP		deepReportUtilize	 (Control& control);
	HFSM_INLINE Rank	deepReportRank		 (Control& control);
	HFSM_INLINE Utility	deepReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepEnterRequested	(Control&)										{}
	HFSM_INLINE void   deepChangeToRequested(Control&)										{}

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE
	static constexpr bool isBare()	 { return std::is_same<Head, Empty>::value;	 }

	static constexpr LongIndex NAME_COUNT = isBare() ? 0 : 1;
#endif

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename Args::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

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

	template <typename TMethodType>
	typename std::enable_if< std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(Logger&,
		Context&,
		const Method) const
	{}

	template <typename TMethodType>
	typename std::enable_if<!std::is_same<typename MemberTraits<TMethodType>::State, Empty>::value>::type
	log(Logger& logger,
		Context& context,
		const Method method) const
	{
		logger.recordMethod(context, STATE_ID, method);
	}
#endif

	// if you see..
	// VS	 - error C2079: 'hfsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	Head _head;
	HFSM_IF_DEBUG(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
