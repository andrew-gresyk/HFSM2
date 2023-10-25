namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

struct None final {};

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_
	: THead
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

	using Head			= THead;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Parent		stateParent			 (	   Control& control)								noexcept	{ return control._core.registry.stateParents[STATE_ID];	}

	HFSM2_CONSTEXPR(14)	void		deepRegister		 (	  Registry& registry, const Parent parent)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Short		wrapSelect			 (	   Control& control)								noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	Rank		wrapRank			 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility		wrapUtility			 (	   Control& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	bool		deepForwardEntryGuard(GuardControl&		   )								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool		deepEntryGuard		 (GuardControl&	control)								noexcept;

	HFSM2_CONSTEXPR(14)	void		deepEnter			 ( PlanControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void		deepReenter			 ( PlanControl& control)								noexcept;

	HFSM2_CONSTEXPR(14)	TaskStatus	deepPreUpdate		 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	deepUpdate			 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPostUpdate		 ( FullControl& control)								noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPreReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepReact			 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	TaskStatus	deepPostReact		 ( FullControl& control, const TEvent&	event)			noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14)	void		deepQuery			 (ConstControl& control,	   TEvent&  event)	  const noexcept;

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	TaskStatus	deepUpdatePlans		 ( FullControl& control)								noexcept;
#endif

	HFSM2_CONSTEXPR(14)	bool		deepForwardExitGuard (GuardControl&		   )								noexcept	{ return false;		}
	HFSM2_CONSTEXPR(14)	bool		deepExitGuard		 (GuardControl&	control)								noexcept;

	HFSM2_CONSTEXPR(14)	void		deepExit			 ( PlanControl& control)								noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void		wrapPlanSucceeded	 ( FullControl& control)								noexcept;
	HFSM2_CONSTEXPR(14)	void		wrapPlanFailed		 ( FullControl& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepForwardActive	 (	   Control&,		 const Request		  )			noexcept	{}
	HFSM2_CONSTEXPR(14)	void		deepForwardRequest	 (	   Control& control, const Request request)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepRequestChange	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestRestart	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestResume	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestSelect	 (	   Control& control, const Request request)			noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14)	void		deepRequestUtilize	 (	   Control& control, const Request request)			noexcept;
	HFSM2_CONSTEXPR(14)	void		deepRequestRandomize (	   Control& control, const Request request)			noexcept;

	HFSM2_CONSTEXPR(14)	UP			deepReportChange	 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	UP			deepReportUtilize	 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Rank		deepReportRank		 (	   Control& control)								noexcept;
	HFSM2_CONSTEXPR(14)	Utility		deepReportRandomize  (	   Control& control)								noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void		deepChangeToRequested(	   Control&)										noexcept	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_CONSTEXPR(14)	void		deepSaveActive		 (const Registry&, WriteStream&)				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void		deepSaveResumable	 (const Registry&, WriteStream&)				  const noexcept	{}

	HFSM2_CONSTEXPR(14)	void		deepLoadRequested	 (		Registry&,  ReadStream&)				  const noexcept	{}
	HFSM2_CONSTEXPR(14)	void		deepLoadResumable	 (		Registry&,  ReadStream&)				  const noexcept	{}
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static
	HFSM2_CONSTEXPR(NO)	const char* name() noexcept;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)							  const noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

	using Empty			= EmptyT<TArgs>;

	static HFSM2_CONSTEXPR(11)	bool isBare()																noexcept	{ return IsSame<Head, Empty>;	}

	HFSM2_IF_TYPEINDEX(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));

	static constexpr Long NAME_COUNT = isBare() ? 0 : 1;

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename THost, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	template <typename TReturn, typename THost, typename... TParams>
	HFSM2_CONSTEXPR(14)
	void log(TReturn (THost::*)(TParams...) const,
			 Logger& logger,
			 const Context& context,
			 const Method method)																	  const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

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

#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
