namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

struct None {};

#endif

////////////////////////////////////////////////////////////////////////////////

template <typename TIndices,
		  typename TArgs,
		  typename THead>
struct S_ final
	: THead
{
	static constexpr auto STATE_ID	 = TIndices::STATE_ID;

	using Context		= typename TArgs::Context;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using Rank			= typename TArgs::Rank;
	using Utility		= typename TArgs::Utility;
	using UP			= typename TArgs::UP;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
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

	//----------------------------------------------------------------------

#if HFSM2_EXPLICIT_MEMBER_SPECIALIZATION_AVAILABLE()

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wnull-dereference"
#endif

	template <typename T>
	struct Accessor {
		HFSM2_CONSTEXPR(11) static			T& get(		 S_&  )					  noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}
		HFSM2_CONSTEXPR(11) static const	T& get(const S_&  )					  noexcept	{ HFSM2_BREAK(); return *reinterpret_cast<T*>(0);	}
	};

#ifdef __clang__
	#pragma clang diagnostic pop
#endif

	template <>
	struct Accessor<Head> {
		HFSM2_CONSTEXPR(11) static		 Head& get(		 S_& s)					  noexcept	{ return s;											}
		HFSM2_CONSTEXPR(11) static const Head& get(const S_& s)					  noexcept	{ return s;											}
	};

	template <typename T>
	HFSM2_CONSTEXPR(14)		  T& access()										  noexcept	{ return Accessor<T>::get(*this);					}

	template <typename T>
	HFSM2_CONSTEXPR(11) const T& access()									const noexcept	{ return Accessor<T>::get(*this);					}

#endif

	//----------------------------------------------------------------------

	HFSM2_CONSTEXPR(14) Parent	stateParent			  (Control&  control)		  noexcept	{ return control._registry.stateParents[STATE_ID];	}

	HFSM2_CONSTEXPR(14) void	deepRegister		  (Registry& registry,
													   const Parent parent)		  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) Rank	wrapRank			  (Control&  control)		  noexcept;
	HFSM2_CONSTEXPR(14) Utility wrapUtility			  (Control&  control)		  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) bool	deepForwardEntryGuard(GuardControl&		   )	  noexcept	{ return false;	}
	HFSM2_CONSTEXPR(14) bool	deepEntryGuard		 (GuardControl&	control)	  noexcept;

	HFSM2_CONSTEXPR(14) void	deepEnter			 (PlanControl&	control)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepReenter			 (PlanControl&	control)	  noexcept;

	HFSM2_CONSTEXPR(14) Status	deepUpdate			 (FullControl&	control)	  noexcept;

	template <typename TEvent>
	HFSM2_CONSTEXPR(14) Status	deepReact			 (FullControl&	control,
													  const TEvent&	event)		  noexcept;

	HFSM2_CONSTEXPR(14) bool	deepForwardExitGuard (GuardControl&		   )	  noexcept	{ return false; }
	HFSM2_CONSTEXPR(14) bool	deepExitGuard		 (GuardControl&	control)	  noexcept;

	HFSM2_CONSTEXPR(14) void	deepExit			 (PlanControl&	control)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_PLANS_AVAILABLE()
	HFSM2_CONSTEXPR(14) void	wrapPlanSucceeded	 (FullControl&	control)	  noexcept;
	HFSM2_CONSTEXPR(14) void	wrapPlanFailed		 (FullControl&	control)	  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepForwardActive	 (Control&,			const Request		 )	  noexcept	{}
	HFSM2_CONSTEXPR(14) void	deepForwardRequest	 (Control& control, const Request request)	  noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepRequestChange	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestRestart	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestResume	 (Control& control, const Request request)	  noexcept;

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	HFSM2_CONSTEXPR(14) void	deepRequestUtilize	 (Control& control, const Request request)	  noexcept;
	HFSM2_CONSTEXPR(14) void	deepRequestRandomize (Control& control, const Request request)	  noexcept;

	HFSM2_CONSTEXPR(14) UP		deepReportChange	 (Control& control)			  noexcept;
	HFSM2_CONSTEXPR(14) UP		deepReportUtilize	 (Control& control)			  noexcept;
	HFSM2_CONSTEXPR(14) Rank	deepReportRank		 (Control& control)			  noexcept;
	HFSM2_CONSTEXPR(14) Utility deepReportRandomize  (Control& control)			  noexcept;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14) void	deepChangeToRequested(Control&)					  noexcept	{}

	//----------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()
	using WriteStream	= typename TArgs::WriteStream;
	using ReadStream	= typename TArgs::ReadStream;

	HFSM2_CONSTEXPR(14) void	 deepSaveActive	  (const Registry&, WriteStream&)	const noexcept	{}
	HFSM2_CONSTEXPR(14) void	 deepSaveResumable(const Registry&, WriteStream&)	const noexcept	{}

	HFSM2_CONSTEXPR(14) void	 deepLoadRequested(		 Registry&, ReadStream& )	const noexcept	{}
	HFSM2_CONSTEXPR(14) void	 deepLoadResumable(		 Registry&, ReadStream& )	const noexcept	{}
#endif

	//----------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	using StructureStateInfos = typename TArgs::StructureStateInfos;
	using RegionType		  = typename StructureStateInfo::RegionType;

	static
	HFSM2_CONSTEXPR(NO)	const char* name() noexcept;

	HFSM2_CONSTEXPR(14)	void deepGetNames(const Long parent,
										  const RegionType region,
										  const Short depth,
										  StructureStateInfos& stateInfos)			const noexcept;
#endif

	//----------------------------------------------------------------------

#if HFSM2_DEBUG_STATE_TYPE_AVAILABLE() || HFSM2_STRUCTURE_REPORT_AVAILABLE() || HFSM2_LOG_INTERFACE_AVAILABLE()

	using Empty			= EmptyT<TArgs>;

	static HFSM2_CONSTEXPR(11) bool isBare()  noexcept	{ return std::is_base_of<Head, Empty>::value;	}

	HFSM2_IF_TYPEINDEX(const std::type_index TYPE = isBare() ? typeid(None) : typeid(Head));

	static constexpr Long NAME_COUNT = isBare() ? 0 : 1;

#endif

	//----------------------------------------------------------------------

#if HFSM2_LOG_INTERFACE_AVAILABLE()

	template <typename TReturn, typename THost, typename... TParams>
	HFSM2_CONSTEXPR(11)
	void log(TReturn (THost::*)(TParams...),
			 Logger& logger,
			 Context& context,
			 const Method method)											const noexcept
	{
		logger.recordMethod(context, STATE_ID, method);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	template <typename TReturn, typename... TParams>
	HFSM2_CONSTEXPR(11)
	void log(TReturn (Empty::*)(TParams...),
			 Logger&,
			 Context&,
			 const Method)													const noexcept
	{}

#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state.inl"
