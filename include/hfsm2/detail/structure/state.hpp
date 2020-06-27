namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ final {
	static constexpr auto STATE_ID	 = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
#endif

#ifdef HFSM2_ENABLE_LOG_INTERFACE
	using Logger		= typename TArgs::Logger;
#endif

	using Control		= ControlT<TArgs>;
	using Registry		= RegistryT<TArgs>;
	using StateParents	= typename Registry::StateParents;

	using PlanControl	= PlanControlT<TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;

	using Head			= THead;
	using HeadBox		= Boxify<Head, TArgs>;

	//----------------------------------------------------------------------

#ifdef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

	template <typename T>
	struct Accessor {
		HFSM2_INLINE static		  T&	   get(		 S_&  )			{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);		}
		HFSM2_INLINE static const T&	   get(const S_&  )			{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);		}
	};

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	template <>
	struct Accessor<Head> {
		HFSM2_INLINE static		  Head& get(	  S_& s)			{ return s._headBox.get();								}
		HFSM2_INLINE static const Head& get(const S_& s)			{ return s._headBox.get();								}
	};

	template <typename T>
	HFSM2_INLINE	   T& access()									{ return Accessor<T>::get(*this);						}

	template <typename T>
	HFSM2_INLINE const T& access() const							{ return Accessor<T>::get(*this);						}
#endif

	//----------------------------------------------------------------------

	HFSM2_INLINE Parent	 stateParent		  (Control& control)	{ return control._registry.stateParents[STATE_ID];		}

	HFSM2_INLINE void	 deepRegister		  (Registry& registry, const Parent parent);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE Rank	 wrapRank			  (Control&  control);
	HFSM2_INLINE Utility wrapUtility		  (Control&  control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE bool	 deepForwardEntryGuard(GuardControl&)												{ return false;	}
	HFSM2_INLINE bool	 deepEntryGuard		  (GuardControl& control);

	HFSM2_INLINE void	 deepConstruct		  (PlanControl&  control);

	HFSM2_INLINE void	 deepEnter			  (PlanControl&  control);
	HFSM2_INLINE void	 deepReenter		  (PlanControl&  control);

	HFSM2_INLINE Status	 deepUpdate			  (FullControl&  control);

	template <typename TEvent>
	HFSM2_INLINE Status	 deepReact			  (FullControl&	 control, const TEvent& event);

	HFSM2_INLINE bool	 deepForwardExitGuard (GuardControl&)												{ return false; }
	HFSM2_INLINE bool	 deepExitGuard		  (GuardControl& control);

	HFSM2_INLINE void	 deepExit			  (PlanControl&	 control);

	HFSM2_INLINE void	 deepDestruct		  (PlanControl&  control);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_PLANS
	HFSM2_INLINE void	 wrapPlanSucceeded	  (FullControl&	control);
	HFSM2_INLINE void	 wrapPlanFailed		  (FullControl&	control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepForwardActive	  (Control&, const Request::Type)												{}
	HFSM2_INLINE void	 deepForwardRequest	  (Control&, const Request::Type)												{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	 deepRequestChange	  (Control&)																	{}
	HFSM2_INLINE void	 deepRequestRemain	  (Registry&)																	{}
	HFSM2_INLINE void	 deepRequestRestart	  (Registry&)																	{}
	HFSM2_INLINE void	 deepRequestResume	  (Registry&)																	{}

#ifdef HFSM2_ENABLE_UTILITY_THEORY
	HFSM2_INLINE void	 deepRequestUtilize	  (Control&)																	{}
	HFSM2_INLINE void	 deepRequestRandomize (Control&)																	{}

	HFSM2_INLINE UP		 deepReportChange	  (Control& control);
	HFSM2_INLINE UP		 deepReportUtilize	  (Control& control);
	HFSM2_INLINE Rank	 deepReportRank		  (Control& control);
	HFSM2_INLINE Utility deepReportRandomize  (Control& control);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE void	deepChangeToRequested(Control&)																		{}

	//----------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_INLINE void	 deepSaveActive	  (const Registry&, WriteStream&) const												{}
	HFSM2_INLINE void	 deepSaveResumable(const Registry&, WriteStream&) const												{}

	HFSM2_INLINE void	 deepLoadRequested(		 Registry&, ReadStream& ) const												{}
	HFSM2_INLINE void	 deepLoadResumable(		 Registry&, ReadStream& ) const												{}
#endif

	//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT
	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static const char* name();

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if defined _DEBUG || defined HFSM2_ENABLE_STRUCTURE_REPORT || defined HFSM2_ENABLE_LOG_INTERFACE

	static constexpr LongIndex NAME_COUNT = HeadBox::isBare() ? 0 : 1;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_LOG_INTERFACE

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
