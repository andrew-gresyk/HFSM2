namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
struct CoreT {
	using Context			= typename TArgs::Context;
	using PureContext		= typename TArgs::PureContext;

	using Registry			= RegistryT<TArgs>;

	using Payload			= typename TArgs::Payload;
	using Transition		= TransitionT<Payload>;
	using TransitionSet		= ArrayT<Transition, TArgs::COMPO_REGIONS>;
	using TransitionSets	= ArrayT<Transition, TArgs::COMPO_REGIONS * TArgs::SUBSTITUTION_LIMIT>;

#if HFSM2_PLANS_AVAILABLE()
	using PlanData			= PlanDataT<TArgs>;
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using TransitionTargets	= StaticArrayT<Short, TArgs::STATE_COUNT>;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using RNG				= typename TArgs::RNG;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using Logger			= typename TArgs::Logger;
#endif

	HFSM2_CONSTEXPR(14)	explicit CoreT(Context& context_
									 HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
									 HFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14)	explicit CoreT(PureContext&& context_
									 HFSM2_IF_UTILITY_THEORY(, RNG& rng_)
									 HFSM2_IF_LOG_INTERFACE(, Logger* const logger_ = nullptr))		noexcept;

	HFSM2_CONSTEXPR(14) CoreT(const CoreT&  other)													noexcept;
	HFSM2_CONSTEXPR(14) CoreT(		CoreT&& other)													noexcept;

	Context context;
	Registry registry;
	TransitionSet requests;
	HFSM2_IF_PLANS(PlanData planData);
	HFSM2_IF_TRANSITION_HISTORY(TransitionTargets transitionTargets);
	HFSM2_IF_TRANSITION_HISTORY(TransitionSets previousTransitions);
	HFSM2_IF_UTILITY_THEORY(RNG& rng);
	HFSM2_IF_LOG_INTERFACE(Logger* logger);
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "core.inl"
