namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

struct None final {};

#endif

////////////////////////////////////////////////////////////////////////////////

template <
	typename TIndices
  , typename TArgs
>
struct S_<TIndices, TArgs, EmptyT<TArgs>>
	: EmptyT<TArgs>
{
	static constexpr StateID STATE_ID = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger		= typename TArgs::Logger;
#endif

	using ConstControl	= ConstControlT<TArgs>;
	using ScopedCOrigin	= typename ConstControl::Origin;

	using Control		= ControlT	   <TArgs>;
	using Registry		= RegistryT	   <TArgs>;
	using StateParents	= typename Registry::StateParents;

	using PlanControl	= PlanControlT <TArgs>;
	using ScopedOrigin	= typename PlanControl::Origin;

	using FullControl	= FullControlT <TArgs>;
	using GuardControl	= GuardControlT<TArgs>;
	using EventControl	= EventControlT<TArgs>;

	using Empty			= EmptyT<TArgs>;
	using Head			= Empty;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Parent		 stateParent		  (		Control& control)								noexcept	{ return control._core.registry.stateParents[STATE_ID];	}

	HFSM2_CONSTEXPR(14)	void		 deepRegister		  (	   Registry&		, const Parent		)			noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Prong		 wrapSelect			  (		Control& control)								noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank		 wrapRank			  (		Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility		 wrapUtility		  (		Control& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool		 deepForwardEntryGuard(GuardControl&		)								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool		 deepEntryGuard		  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void		 deepEnter			  ( PlanControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void		 deepReenter		  ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	TaskStatus	 deepPreUpdate		  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepUpdate			  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepPostUpdate		  ( FullControl& control)								noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepPreReact		  (EventControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepReact			  (EventControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepPostReact		  (EventControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void		 deepQuery			  (ConstControl& control,		TEvent& event)	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	TaskStatus	 deepUpdatePlans	  ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool		 deepForwardExitGuard (GuardControl&		)								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool		 deepExitGuard		  (GuardControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	void		 deepExit			  ( PlanControl& control)								noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void		 wrapPlanSucceeded	  ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void		 wrapPlanFailed		  ( FullControl& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		 deepForwardActive	  (	   Control&,		 const Request		  )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void		 deepForwardRequest	  (	   Control& control, const Request request)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		 deepRequestChange	  (	    Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void		 deepRequestRestart	  (		Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void		 deepRequestResume	  (		Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void		 deepRequestSelect	  (	    Control& control, const Request request)		noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void		 deepRequestUtilize	  (		Control& control, const Request request)		noexcept;
	HFSM2_CONSTEXPR(14)	void		 deepRequestRandomize (		Control& control, const Request request)		noexcept;

	HFSM2_CONSTEXPR(14)	UP			 deepReportChange	  (	    Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	UP			 deepReportUtilize	  (	    Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Rank		 deepReportRank		  (		Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility		 deepReportRandomize  (	    Control& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		 deepChangeToRequested(	    Control&		)								noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_CONSTEXPR(14)	void		 deepSaveActive		  (const Registry&, WriteStream&)				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void		 deepSaveResumable	  (const Registry&, WriteStream&)				  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void		 deepLoadRequested	  (		 Registry&,  ReadStream&)				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void		 deepLoadResumable	  (		 Registry&,  ReadStream&)				  const noexcept	{}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename TArgs::StructureStateInfos;

	static
	HFSM2_CONSTEXPR(NO)	const char* name() noexcept;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

	HFSM2_IF_TYPEINDEX(const std::type_index TYPE = typeid(None));

	static constexpr Long NAME_COUNT = 0;

#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	template <typename TReturn, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (Empty::*)(TParams...) const,
			 Logger&,
			 const Context&,
			 const Method)																			  const noexcept
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state_2.inl"
