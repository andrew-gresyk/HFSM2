namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
ShortIndex
C_<TN_, TA_, TG_, TH_, TS_...>::resolveRandom(Control& control,
											  const Utility(& options)[Info::WIDTH],
											  const Utility sum,
											  const Rank(& ranks)[Info::WIDTH],
											  const Rank top)
{
	const Utility random = control._rng.next();
	HFSM_ASSERT(0.0f <= random && random < 1.0f);

	Utility cursor = random * sum;

	for (ShortIndex i = 0; i < count(ranks); ++i)
		if (ranks[i] == top) {
			HFSM_ASSERT(options[i] >= 0.0f);

			if (cursor >= options[i])
				cursor -= options[i];
			else {
				HFSM_LOG_RANDOM_RESOLUTION(control.context(), HEAD_ID, i, random);

				return i;
			}
		}

	HFSM_BREAK();
	return INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRegister(StateRegistry& stateRegistry,
											 const Parent parent)
{
	stateRegistry.compoParents[COMPO_INDEX] = parent;

	_headState.deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry, Parent{COMPO_ID});
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
bool
C_<TN_, TA_, TG_, TH_, TS_...>::deepForwardEntryGuard(GuardControl& control) {
	const ShortIndex active	   = compoActive   (control);
	const ShortIndex requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (requested == INVALID_SHORT_INDEX)
		return _subStates.wideForwardEntryGuard(control, active);
	else
		return _subStates.wideEntryGuard	   (control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
bool
C_<TN_, TA_, TG_, TH_, TS_...>::deepEntryGuard(GuardControl& control) {
	const ShortIndex requested = compoRequested(control);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepEntryGuard(control) ||
		   _subStates.wideEntryGuard(control, requested);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepConstruct(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  == INVALID_SHORT_INDEX);
	HFSM_ASSERT(requested != INVALID_SHORT_INDEX);

	active	  = requested;

	if (requested == resumable)
		resumable = INVALID_SHORT_INDEX;

	requested = INVALID_SHORT_INDEX;

	_headState.deepConstruct(control);
	_subStates.wideConstruct(control, active);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepEnter(PlanControl& control) {
	const ShortIndex& active = compoActive(control);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepEnter(control);
	_subStates.wideEnter(control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepReenter(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active	  != INVALID_SHORT_INDEX &&
				requested != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	_headState.deepReenter(control);

	if (active == requested)
		_subStates.wideReenter(control, active);
	else {
		_subStates.wideExit	  (control, active);

		active	  = requested;

		if (requested == resumable)
			resumable = INVALID_SHORT_INDEX;

		_subStates.wideEnter  (control, active);
	}

	requested = INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
Status
C_<TN_, TA_, TG_, TH_, TS_...>::deepUpdate(FullControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion outer{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (const Status headStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(control, active);

		return headStatus;
	} else {
		const Status subStatus = _subStates.wideUpdate(control, active);

		if (subStatus.outerTransition)
			return Status{Status::NONE, true};

		ScopedRegion inner{control, REGION_ID, HEAD_ID, REGION_SIZE};

		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
template <typename TEvent>
Status
C_<TN_, TA_, TG_, TH_, TS_...>::deepReact(FullControl& control,
										  const TEvent& event)
{
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

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

		return subStatus && control._planData.planExists.template get<REGION_ID>() ?
			control.updatePlan(_headState, subStatus) : subStatus;
	}
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
bool
C_<TN_, TA_, TG_, TH_, TS_...>::deepForwardExitGuard(GuardControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	if (compoRequested(control) == INVALID_SHORT_INDEX)
		return _subStates.wideForwardExitGuard(control, active);
	else
		return _subStates.wideExitGuard		  (control, active);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
bool
C_<TN_, TA_, TG_, TH_, TS_...>::deepExitGuard(GuardControl& control) {
	const ShortIndex active = compoActive(control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	ScopedRegion region{control, REGION_ID, HEAD_ID, REGION_SIZE};

	return _headState.deepExitGuard(control) ||
		   _subStates.wideExitGuard(control, active);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepExit(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.wideExit(control, active);
	_headState.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepDestruct(PlanControl& control) {
	ShortIndex& active	  = compoActive   (control);
	ShortIndex& resumable = compoResumable(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	_subStates.wideDestruct(control, active);
	_headState.deepDestruct(control);

	resumable = active;
	active	  = INVALID_SHORT_INDEX;

	auto plan = control.plan(REGION_ID);
	plan.clear();
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepForwardActive(Control& control,
												  const Request::Type request)
{
	HFSM_ASSERT(control._stateRegistry.isActive(HEAD_ID));

	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX) {
		const ShortIndex active = compoActive(control);

		_subStates.wideForwardActive (control, request, active);
	} else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepForwardRequest(Control& control,
												   const Request::Type request)
{
	const ShortIndex requested = compoRequested(control);

	if (requested == INVALID_SHORT_INDEX)
		deepRequest					 (control, request);
	else
		_subStates.wideForwardRequest(control, request, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequest(Control& control,
											const Request::Type request)
{
	switch (request) {
	case Request::REMAIN:
		deepRequestRemain	(control._stateRegistry);
		break;

	case Request::CHANGE:
		deepRequestChange	(control);
		break;

	case Request::RESTART:
		deepRequestRestart	(control._stateRegistry);
		break;

	case Request::RESUME:
		deepRequestResume	(control._stateRegistry);
		break;

	case Request::UTILIZE:
		deepRequestUtilize	(control);
		break;

	case Request::RANDOMIZE:
		deepRequestRandomize(control);
		break;

	default:
		HFSM_BREAK();
	}
}

//------------------------------------------------------------------------------

#ifndef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestChange(Control& control)
{
	switch (STRATEGY) {
	case Strategy::Composite:
		deepRequestChangeComposite  (control);
		break;

	case Strategy::Resumable:
		deepRequestChangeResumable  (control);
		break;

	case Strategy::Utilitarian:
		deepRequestChangeUtilitarian(control);
		break;

	case Strategy::RandomUtil:
		deepRequestChangeRandom		(control);
		break;

	default:
		HFSM_BREAK();
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);

	requested = 0;

	_subStates.wideRequestChangeComposite(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.wideRequestChangeResumable(control, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestChangeUtilitarian(Control& control) {
	const UP s = _subStates.wideReportChangeUtilitarian(control);
	HFSM_ASSERT(s.prong != INVALID_SHORT_INDEX);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestChangeRandom(Control& control) {
	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestRemain(StateRegistry& stateRegistry) {
	const ShortIndex  active	= compoActive   (stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	if (active == INVALID_SHORT_INDEX)
		requested = 0;

	_subStates.wideRequestRemain(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestRestart(StateRegistry& stateRegistry) {
	ShortIndex& requested = compoRequested(stateRegistry);

	requested = 0;

	_subStates.wideRequestRestart(stateRegistry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestResume(StateRegistry& stateRegistry) {
	const ShortIndex  resumable = compoResumable(stateRegistry);
		  ShortIndex& requested = compoRequested(stateRegistry);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	_subStates.wideRequestResume(stateRegistry, requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestUtilize(Control& control) {
	const UP s = _subStates.wideReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepRequestRandomize(Control& control) {
	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);
}

//------------------------------------------------------------------------------

#ifndef HFSM_EXPLICIT_MEMBER_SPECIALIZATION

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportChange(Control& control) {
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
		HFSM_BREAK();
		return {};
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportChangeComposite(Control& control) {
	ShortIndex& requested = compoRequested(control);
	requested = 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeComposite(control);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportChangeResumable(Control& control) {
	const ShortIndex  resumable = compoResumable(control);
		  ShortIndex& requested = compoRequested(control);

	requested = (resumable != INVALID_SHORT_INDEX) ?
		resumable : 0;

	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeResumable(control, requested);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportChangeUtilitarian(Control& control) {
	const UP h = _headState.deepReportChange(control);
	const UP s = _subStates.wideReportChangeUtilitarian(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportChangeRandom(Control& control) {
	const UP h = _headState.deepReportChange(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const UP sum = _subStates.wideReportChangeRandom(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum.utility, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);

	return {
		h.utility * options[requested],
		h.prong
	};
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::UP
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportUtilize(Control& control) {
	const UP h = _headState.deepReportUtilize(control);
	const UP s = _subStates.wideReportUtilize(control);

	ShortIndex& requested = compoRequested(control);
	requested = s.prong;

	HFSM_LOG_UTILITY_RESOLUTION(control.context(), HEAD_ID, requested, s.utility);

	return {
		h.utility * s.utility,
		h.prong
	};
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::Rank
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportRank(Control& control) {
	return _headState.wrapRank(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
typename TA_::Utility
C_<TN_, TA_, TG_, TH_, TS_...>::deepReportRandomize(Control& control) {
	const Utility h = _headState.wrapUtility(control);

	Rank ranks[Info::WIDTH];
	Rank top = _subStates.wideReportRank(control, ranks);

	Utility options[Info::WIDTH];
	const Utility sum = _subStates.wideReportRandomize(control, options, ranks, top);

	ShortIndex& requested = compoRequested(control);
	requested = resolveRandom(control, options, sum, ranks, top);
	HFSM_ASSERT(requested < Info::WIDTH);

	return h * options[requested];
}

//------------------------------------------------------------------------------

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepChangeToRequested(PlanControl& control) {
	ShortIndex& active	  = compoActive	  (control);
	ShortIndex& resumable = compoResumable(control);
	ShortIndex& requested = compoRequested(control);

	HFSM_ASSERT(active != INVALID_SHORT_INDEX);

	if (requested == INVALID_SHORT_INDEX)
		_subStates.wideChangeToRequested(control, active);
	else if (requested != active) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		resumable = active;
		active	  = requested;
		requested = INVALID_SHORT_INDEX;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else if (compoRemain(control)) {
		_subStates.wideExit		(control, active);
		_subStates.wideDestruct	(control, active);

		requested = INVALID_SHORT_INDEX;

		_subStates.wideConstruct(control, active);
		_subStates.wideEnter	(control, active);
	} else {
		requested = INVALID_SHORT_INDEX;

		// TODO: _subStates.wideReconstruct();

		_subStates.wideReenter	(control, active);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TN_, typename TA_, Strategy TG_, typename TH_, typename... TS_>
void
C_<TN_, TA_, TG_, TH_, TS_...>::deepGetNames(const LongIndex parent,
											 const RegionType region,
											 const ShortIndex depth,
											 StructureStateInfos& _stateInfos) const
{
	_headState.deepGetNames(parent,					 region,						depth,	   _stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, StructureStateInfo::COMPOSITE, depth + 1, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
