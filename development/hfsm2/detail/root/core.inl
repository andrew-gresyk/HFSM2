namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(Context& context_
				  HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
				  HFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{context_}
	HFSM2_IF_UTILITY_THEORY(, rng{rng_})
	HFSM2_IF_LOG_INTERFACE(, logger{logger_})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(PureContext&& context_
				  HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
				  HFSM2_IF_LOG_INTERFACE(, Logger* const logger_)) noexcept
	: context{move(context_)}
	HFSM2_IF_UTILITY_THEORY(, rng	{rng_	})
	HFSM2_IF_LOG_INTERFACE (, logger{logger_})
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(const CoreT& other) noexcept
	: context {other.context }
	, registry{other.registry}
	, requests{other.requests}
	HFSM2_IF_PLANS			   (, planData			 {other.planData		   })
	HFSM2_IF_TRANSITION_HISTORY(, transitionTargets  {other.transitionTargets  })
	HFSM2_IF_TRANSITION_HISTORY(, previousTransitions{other.previousTransitions})
	HFSM2_IF_UTILITY_THEORY	   (, rng				 {other.rng				   })
	HFSM2_IF_LOG_INTERFACE	   (, logger			 {other.logger			   })
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
CoreT<TArgs>::CoreT(CoreT&& other) noexcept
	: context {move(other.context )}
	, registry{move(other.registry)}
	, requests{move(other.requests)}
	HFSM2_IF_PLANS			   (, planData			 {move(other.planData			)})
	HFSM2_IF_TRANSITION_HISTORY(, transitionTargets  {move(other.transitionTargets	)})
	HFSM2_IF_TRANSITION_HISTORY(, previousTransitions{move(other.previousTransitions)})
	HFSM2_IF_UTILITY_THEORY	   (, rng				 {move(other.rng				)})
	HFSM2_IF_LOG_INTERFACE	   (, logger			 {move(other.logger				)})
{}

////////////////////////////////////////////////////////////////////////////////

}
}
