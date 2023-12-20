namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
class GuardControlT final
	: public FullControlT<TArgs>
{
	template <typename, typename, typename>
	friend struct S_;

	template <typename, typename>
	friend class R_;

	using FullControl	= FullControlT<TArgs>;

	using typename FullControl::Core;
	using typename FullControl::Context;

	using typename FullControl::Registry;
	using typename FullControl::TransitionSet;
	using typename FullControl::TransitionSets;

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	using typename FullControl::TransitionTargets;
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	using typename FullControl::RNG;
#endif

#if HFSM2_PLANS_AVAILABLE()
	using typename FullControl::PlanData;
#endif

#if HFSM2_LOG_INTERFACE_AVAILABLE()
	using typename FullControl::Logger;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)	GuardControlT(Core& core,
									  const TransitionSets& currentTransitions,
									  const TransitionSet& pendingTransitions) noexcept
		: FullControl{core, currentTransitions}
		, _pendingTransitions{pendingTransitions}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	using FullControl::context;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated or deactivated
	/// @param `stateId` State identifier
	/// @return State pending activation/deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingChange(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingChange(stateId_);					}

	/// @brief Check if a state is going to be activated or deactivated
	/// @tparam `TState` State type
	/// @return State pending activation/deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingChange()						  const noexcept	{ return isPendingChange(FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be activated
	/// @param `stateId` State identifier
	/// @return State pending activation status
	HFSM2_CONSTEXPR(11)	bool isPendingEnter (const StateID stateId_)  const noexcept	{ return _core.registry.isPendingEnter (stateId_);					}

	/// @brief Check if a state is going to be activated
	/// @tparam `TState` State type
	/// @return State pending activation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingEnter ()						  const noexcept	{ return isPendingEnter (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	/// @brief Check if a state is going to be deactivated
	/// @param `stateId` State identifier
	/// @return State pending deactivation status
	HFSM2_CONSTEXPR(11)	bool isPendingExit	(const StateID stateId_)  const noexcept	{ return _core.registry.isPendingExit  (stateId_);					}

	/// @brief Check if a state is going to be deactivated
	/// @tparam `TState` State type
	/// @return State pending deactivation status
	template <typename TState>
	HFSM2_CONSTEXPR(11)	bool isPendingExit  ()						  const noexcept	{ return isPendingExit  (FullControl::template stateId<TState>());	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// COMMON

	/// @brief Get pending transition requests
	/// @return Array of pending transition requests
	HFSM2_CONSTEXPR(11)	const TransitionSet&  pendingTransitions()	  const noexcept	{ return _pendingTransitions;										}

	/// @brief Cancel pending transition requests
	///   (can be used to substitute a transition into the current state with a different one)
	HFSM2_CONSTEXPR(14)	void cancelPendingTransitions()						noexcept;

private:
	using FullControl::_core;
	using FullControl::_originId;

	const TransitionSet& _pendingTransitions;

	bool _cancelled = false;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "control_4.inl"
