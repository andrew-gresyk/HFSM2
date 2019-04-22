namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TA, typename TH>
struct RegisterT {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;
	using StateList		= typename TA::StateList;

	static constexpr StateID STATE_ID = NS;

	static HFSM_INLINE
	void
	execute(StateParents& stateParents,
			const Parent parent)
	{
		static constexpr auto HEAD_ID = StateList::template index<TH>();
		StaticAssertEquality<STATE_ID, HEAD_ID>();

		stateParents[STATE_ID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA>
struct RegisterT<NS, TA, Empty<TA>> {
	using StateParents	= StaticArray<Parent, TA::STATE_COUNT>;

	static HFSM_INLINE
	void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepRegister(StateRegistry& stateRegistry,
							 const Parent parent)
{
	using Register = RegisterT<STATE_ID, TA, Head>;
	Register::execute(stateRegistry.stateParents, parent);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepEntryGuard(GuardControl& control,
							   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::entryGuard, Method::ENTRY_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreEntryGuard(control.context());
	_head.entryGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnter(PlanControl& control,
						  const ShortIndex /*prong*/)
{
	HFSM_ASSERT(!control.planData().hasSucceeded(STATE_ID));
	HFSM_ASSERT(!control.planData().hasFailed   (STATE_ID));

	HFSM_LOG_STATE_METHOD(&Head::enter, Method::ENTER);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
Status
_S<NS, TA, TH>::deepUpdate(FullControl& control,
						   const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::update, Method::UPDATE);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
template <typename TEvent>
void
_S<NS, TA, TH>::deepReact(FullControl& control,
						  const TEvent& event,
						  const ShortIndex /*prong*/)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, FullControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, Method::REACT);

	ScopedOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());

	(_head.*reaction)(event, control);		//_head.react(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepExitGuard(GuardControl& control,
							  const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exitGuard, Method::EXIT_GUARD);

	ScopedOrigin origin{control, STATE_ID};

	const bool cancelledBefore = control.cancelled();

	_head.widePreExitGuard(control.context());
	_head.exitGuard(control);

	return !cancelledBefore && control.cancelled();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepExit(PlanControl& control,
						 const ShortIndex /*prong*/)
{
	HFSM_LOG_STATE_METHOD(&Head::exit, Method::EXIT);

	ScopedOrigin origin{control, STATE_ID};

	_head.exit(control);
	_head.widePostExit(control.context());

	control.planData().setSuccessful(STATE_ID, false);
	control.planData().setFailed	(STATE_ID, false);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::wrapPlanSucceeded(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planSucceeded, Method::PLAN_SUCCEEDED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planSucceeded(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::wrapPlanFailed(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::planFailed, Method::PLAN_FAILED);

	ScopedOrigin origin{control, STATE_ID};

	_head.planFailed(control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
UProng
_S<NS, TA, TH>::wrapUtility(Control& control) {
	HFSM_LOG_STATE_METHOD(&Head::utility, Method::UTILITY);

	const float utility = _head.utility(static_cast<const Control&>(control));
	const float compliment = 1.0f - utility;

	const float greater = 0.0f > compliment ?
		0.0f : compliment;

	const float clamped = greater < 1.0f ?
		greater : 1.0f;

	const Parent parent = control._stateRegistry.stateParents[STATE_ID];

	return {clamped, parent.prong};
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, typename TA, typename TH>
const char*
_S<NS, TA, TH>::name() {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
	}
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepGetNames(const LongIndex parent,
							  const RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
