namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
	using Logger		= typename TArgs::Logger;

	using Control		= ControlT<TArgs>;
	using StateRegistry	= StateRegistryT<TArgs>;
	using StateParents	= typename StateRegistry::StateParents;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;
	using HeadBox		= Boxify<Head, TArgs>;

	//----------------------------------------------------------------------

#ifdef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

	template <typename T>
	struct Accessor {
		HFSM_INLINE static		 T&	   get(		 S_&  )			{ HFSM_BREAK(); return *reinterpret_cast<T*>(0);		}
		HFSM_INLINE static const T&	   get(const S_&  )			{ HFSM_BREAK(); return *reinterpret_cast<T*>(0);		}
	};

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <>
	struct Accessor<Head> {
		HFSM_INLINE static		 Head& get(		 S_& s)			{ return s._headBox.get();								}
		HFSM_INLINE static const Head& get(const S_& s)			{ return s._headBox.get();								}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename T>
	HFSM_INLINE		  T& access()								{ return Accessor<T>::get(*this);						}

	template <typename T>
	HFSM_INLINE const T& access() const							{ return Accessor<T>::get(*this);						}

#endif

	//----------------------------------------------------------------------

	HFSM_INLINE Parent	stateParent			 (Control& control)	{ return control._stateRegistry.stateParents[STATE_ID]; }

	HFSM_INLINE void	deepRegister		 (StateRegistry& stateRegistry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE bool	deepForwardEntryGuard(GuardControl&)					{ return false;	}
	HFSM_INLINE bool	deepEntryGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepEnter			 (PlanControl&	control);
	HFSM_INLINE void	deepReenter			 (PlanControl&	control);

	HFSM_INLINE Status	deepUpdate			 (FullControl&	control);

	template <typename TEvent>
	HFSM_INLINE Status	deepReact			 (FullControl&	control, const TEvent& event);

	HFSM_INLINE bool	deepForwardExitGuard (GuardControl&)					{ return false; }
	HFSM_INLINE bool	deepExitGuard		 (GuardControl&	control);

	HFSM_INLINE void	deepExit			 (PlanControl&	control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	wrapPlanSucceeded	 (FullControl&	control);
	HFSM_INLINE void	wrapPlanFailed		 (FullControl&	control);
	HFSM_INLINE Rank	wrapRank			 (Control& control);
	HFSM_INLINE Utility	wrapUtility			 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepForwardActive	 (Control&, const Request::Type)					{}
	HFSM_INLINE void	deepForwardRequest	 (Control&, const Request::Type)					{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void	deepRequestChange	 (Control&)											{}
	HFSM_INLINE void	deepRequestRemain	 (StateRegistry&)									{}
	HFSM_INLINE void	deepRequestRestart	 (StateRegistry&)									{}
	HFSM_INLINE void	deepRequestResume	 (StateRegistry&)									{}
	HFSM_INLINE void	deepRequestUtilize	 (Control&)											{}
	HFSM_INLINE void	deepRequestRandomize (Control&)											{}

	HFSM_INLINE UP		deepReportChange	 (Control& control);
	HFSM_INLINE UP		deepReportUtilize	 (Control& control);
	HFSM_INLINE Rank	deepReportRank		 (Control& control);
	HFSM_INLINE Utility	deepReportRandomize	 (Control& control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM_INLINE void   deepEnterRequested	(Control&)											{}
	HFSM_INLINE void   deepChangeToRequested(Control&)											{}

	//----------------------------------------------------------------------

#if defined _DEBUG || defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE

	static constexpr LongIndex NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;

#endif

	//----------------------------------------------------------------------

#ifdef HFSM_ENABLE_LOG_INTERFACE

	template <typename>
	struct Traits_;

	template <typename TR_, typename TH_, typename... TAs_>
	struct Traits_<TR_(TH_::*)(TAs_...)> {
		using Host = TH_;
	};

	template <typename TM_>
	using Host_			= typename Traits_<TM_>::Host;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	using Empty			= EmptyT<TArgs>;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TMethodType>
	typename std::enable_if<
				 std::is_same<
					 Host_<TMethodType>,
					 Empty
				 >::value
			 >::type
	log(Logger&,
		Context&,
		const Method) const
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TMethodType>
	typename std::enable_if<
				 !std::is_same<
					 Host_<TMethodType>,
					 Empty
				 >::value
			 >::type
	log(Logger& logger,
		Context& context,
		const Method method) const
	{
		logger.recordMethod(context, STATE_ID, method);
	}

#endif

	//----------------------------------------------------------------------

	// TODO: account for boxing
	//
	// if you see..
	// VS	 - error C2079: 'hfsm2::detail::S_<...>::_head' uses undefined struct 'Blah'
	// Clang - error : field has incomplete type 'hfsm2::detail::S_<...>::Head' (aka 'Blah')
	//
	// .. add definition for the state 'Blah'
	HeadBox _headBox;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
