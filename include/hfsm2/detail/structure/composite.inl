﻿namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
Short
C_<TN, TA, SG, TH, TS...>::resolveRandom(Control& control,
										 const Utility(& options)[Info::WIDTH],
										 const Utility sum,
										 const Rank(& ranks)[Info::WIDTH],
										 const Rank top) const noexcept
{
	const Utility random = control._rng.next();
	HFSM2_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (Short i = 0; i < count(ranks); ++i)
		if (ranks[i] == top) {
			HFSM2_ASSERT(options[i] >= 0.0f);

			if (cursor >= options[i])
				cursor -= options[i];
			else {
				HFSM2_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, i, random);

				return i;
			}
		}

	HFSM2_BREAK();
	return INVALID_SHORT;
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRegister(Registry& registry,
										const Parent parent) noexcept
{
	registry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(registry, parent);
	_subStates.wideRegister(registry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepForwardEntryGuard(GuardControl& control) noexcept {
	const Short active	  = compoActive   (control);
	const Short requested = compoRequested(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT)
		return _subStates.wideForwardEntryGuard(control, active);
	else
		return _subStates.wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepEntryGuard(GuardControl& control) noexcept {
	const Short requested = compoRequested(control);
	HFSM2_ASSERT(requested != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepConstruct(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active	   == INVALID_SHORT);
	HFSM2_ASSERT(requested != INVALID_SHORT);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT;

	requested = INVALID_SHORT;

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepEnter(PlanControl& control) noexcept {
	const Short& active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepReenter(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active	   != INVALID_SHORT &&
				 requested != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);

	if (active == requested)
		_subStates.wideReenter(control, active);
	else {
		_subStates.wideExit	  (control, active);

		active	  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT;

		_subStates.wideEnter  (control, active);
	}

	requested = INVALID_SHORT;
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
Status
C_<TN, TA, SG, TH, TS...>::deepUpdate(FullControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control, active);

		if (subStatus.outerTransition)
			return Status{Status::Result::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
template <typename TEvent>
Status
C_<TN, TA, SG, TH, TS...>::deepReact(FullControl& control,
									 const TEvent& event) noexcept
{
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepReact(control, event)) {
		ControlLock lock{control};

		_subStates.wideReact(control, event, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideReact(control, event, active);

		if (subStatus.outerTransition)
			return subStatus;

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

	#ifdef HFSM2_ENABLE_PLANS
		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	#else
		return subStatus;
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepForwardExitGuard(GuardControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT)
		return _subStates.wideForwardExitGuard(control, active);
	else
		return _subStates.wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
bool
C_<TN, TA, SG, TH, TS...>::deepExitGuard(GuardControl& control) noexcept {
	const Short active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepExit(PlanControl& control) noexcept {
	Short& active = compoActive(control);
	HFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepDestruct(PlanControl& control) noexcept {
	Short& active	 = compoActive   (control);
	Short& resumable = compoResumable(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	resumable = active;
	active	  = INVALID_SHORT;

#ifdef HFSM2_ENABLE_PLANS
	auto plan = control.plan(REGION_ID);
	plan.clear();
#endif
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepForwardActive(Control& control,
											 const Request request) noexcept
{
	HFSM2_ASSERT(control._registry.isActive(HEAD_ID));

	const Short requested = compoRequested(control);

	if (requested == INVALID_SHORT) {
		const Short active = compoActive(control);

		_subStates.wideForwardActive (control, request, active);
	} else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepForwardRequest(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short requested = compoRequested(control);

	if (requested != INVALID_SHORT)
		_subStates.wideForwardRequest(control, request, requested);
	else
		deepRequest					 (control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequest(Control& control,
									   const Request request) noexcept
{
	switch (request.type) {
	case TransitionType::CHANGE:
		deepRequestChange	(control, request);
		break;

	case TransitionType::RESTART:
		deepRequestRestart	(control, request);
		break;

	case TransitionType::RESUME:
		deepRequestResume	(control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case TransitionType::UTILIZE:
		deepRequestUtilize	(control, request);
		break;

	case TransitionType::RANDOMIZE:
		deepRequestRandomize(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------

#ifndef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChange(Control& control,
											 const Request request) noexcept
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite  (control, request);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable  (control, request);
		break;

#ifdef HFSM2_ENABLE_UTILITY_THEORY

	case Strategy::Utilitarian:
		deepRequestChangeUtilitarian(control, request);
		break;

	case Strategy::RandomUtil:
		deepRequestChangeRandom		(control, request);
		break;

#endif

	default:
		HFSM2_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeComposite(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestChangeComposite(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeResumable(Control& control,
													  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	_subStates.wideRequestChangeResumable(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeUtilitarian(Control& control,
														const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = _subStates.wideReportChangeUtilitarian(control);
	HFSM2_ASSERT(s.prong != INVALID_SHORT);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestChangeRandom(Control& control,
												   const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);
}

#endif

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestRestart(Control& control,
											  const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Short& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestRestart(control, request);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestResume(Control& control,
											 const Request request) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	_subStates.wideRequestResume(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM2_ENABLE_UTILITY_THEORY

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestUtilize(Control& control,
											  const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	const UP s = _subStates.wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepRequestRandomize(Control& control,
												const Request HFSM2_IF_TRANSITION_HISTORY(request)) noexcept
{
	HFSM2_IF_TRANSITION_HISTORY(control.pinLastTransition(HEAD_ID, request.index));

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_UTILITY_THEORY
#ifndef HFSM2_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChange(Control& control) noexcept {
	switch (STRATEGY) {
	case Strategy::Composite:
		return deepReportChangeComposite  (control);

	case Strategy::Resumable:
		return deepReportChangeResumable  (control);

	case Strategy::Utilitarian:
		return deepReportChangeUtilitarian(control);

	case Strategy::RandomUtil:
		return deepReportChangeRandom	  (control);

	default:
		HFSM2_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeComposite(Control& control) noexcept {
	Short& requested = compoRequested(control);
	requested = 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeResumable(Control& control) noexcept {
	const Short  resumable = compoResumable(control);
		  Short& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT) ?
		resumable : 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeUtilitarian(Control& control) noexcept {
	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeUtilitarian(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportChangeRandom(Control& control) noexcept {
	const UP h = _headState.deepReportChange(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::UP
C_<TN, TA, SG, TH, TS...>::deepReportUtilize(Control& control) noexcept {
	const UP h = _headState.deepReportUtilize(control);
	const UP s = _subStates.wideReportUtilize(control);

	Short& requested = compoRequested(control);
	requested = s.prong;

	HFSM2_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::Rank
C_<TN, TA, SG, TH, TS...>::deepReportRank(Control& control) noexcept {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
typename TA::Utility
C_<TN, TA, SG, TH, TS...>::deepReportRandomize(Control& control) noexcept {
	const Utility h = _headState.wrapUtility(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	Short& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM2_ASSERT(requested < Info::WIDTH);

	return h * options[requested];
}

#endif

//------------------------------------------------------------------------------
// COMMON

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepChangeToRequested(PlanControl& control) noexcept {
	Short& active	 = compoActive	 (control);
	Short& resumable = compoResumable(control);
	Short& requested = compoRequested(control);

	HFSM2_ASSERT(active != INVALID_SHORT);

	if (requested == INVALID_SHORT)
		_subStates.wideChangeToRequested(control, active);
	else if (requested != active) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else if (compoRemain(control)) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		requested = INVALID_SHORT;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else {
		requested = INVALID_SHORT;

		// reconstruction done in S_::reenter()
		_subStates.wideReenter	(control, active);
	}
}

// COMMON
//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_SERIALIZATION

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepSaveActive(const Registry& registry,
										  WriteStream& stream) const noexcept
{
	const Short active	  = compoActive   (registry);
	const Short resumable = compoResumable(registry);

	stream.template write<WIDTH_BITS>(active);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	_subStates.wideSaveActive(registry,stream, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepSaveResumable(const Registry& registry,
											 WriteStream& stream) const noexcept
{
	const Short resumable = compoResumable(registry);

	if (resumable != INVALID_SHORT) {
		stream.template write<1>(1);
		stream.template write<WIDTH_BITS>(resumable);
	} else
		stream.template write<1>(0);

	_subStates.wideSaveResumable(registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepLoadRequested(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);
	Short& requested = compoRequested(registry);

	requested = stream.template read<WIDTH_BITS>();
	HFSM2_ASSERT(requested < WIDTH);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	_subStates.wideLoadRequested(registry, stream, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepLoadResumable(Registry& registry,
											 ReadStream& stream) const noexcept
{
	Short& resumable = compoResumable(registry);

	if (stream.template read<1>()) {
		resumable = stream.template read<WIDTH_BITS>();
		HFSM2_ASSERT(resumable < WIDTH);
	} else
		resumable = INVALID_SHORT;

	_subStates.wideLoadResumable(registry, stream);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

template <typename TN, typename TA, Strategy SG, typename TH, typename... TS>
void
C_<TN, TA, SG, TH, TS...>::deepGetNames(const Long parent,
										const RegionType regionType,
										const Short depth,
										StructureStateInfos& _stateInfos) const noexcept
{
	_headState.deepGetNames(parent,					 regionType,								depth,	   _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, StructureStateInfo::RegionType::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
