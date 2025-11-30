namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
R_<TG_, TA_>::R_(Context& context
			   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			   HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{context
		  HFSM2_IF_UTILITY_THEORY(, rng)
		  HFSM2_IF_LOG_INTERFACE(, logger)}
{
	_apex.deepRegister(_core.registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
R_<TG_, TA_>::R_(PureContext&& context
			   HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			   HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _core{move(context)
		  HFSM2_IF_UTILITY_THEORY(, rng)
		  HFSM2_IF_LOG_INTERFACE(, logger)}
{
	_apex.deepRegister(_core.registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(20)
R_<TG_, TA_>::~R_() noexcept {
	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::update() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	FullControl control{_core, emptyTransitions};

	_apex. deepPreUpdate(control);
	_apex.	  deepUpdate(control);
	_apex.deepPostUpdate(control);

#if HFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearStatuses();
#endif

	processRequest();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::react(const TEvent& event) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	EventControl control{_core, emptyTransitions};

	_apex. deepPreReact(control, event);

	control._consumed = false;
	_apex.	  deepReact(control, event);

	control._consumed = false;
	_apex.deepPostReact(control, event);

#if HFSM2_PLANS_AVAILABLE()
	_apex.deepUpdatePlans(control);
	_core.planData.clearStatuses();
#endif

	processRequest();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::query(TEvent& event) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	ConstControl control{_core};

	_apex.deepQuery(control, event);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::succeed(const StateID stateId_) noexcept {
	if (HFSM2_CHECKED(ROOT_ID < stateId_ && stateId_ < STATE_COUNT)) {
		_core.planData.tasksSuccesses.set(stateId_);

		HFSM2_LOG_TASK_STATUS(_core.context, INVALID_REGION_ID, stateId_, StatusEvent::SUCCEEDED);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::fail(const StateID stateId_) noexcept {
	if (HFSM2_CHECKED(ROOT_ID < stateId_ && stateId_ < STATE_COUNT)) {
		_core.planData.tasksFailures.set(stateId_);

		HFSM2_LOG_TASK_STATUS(_core.context, INVALID_REGION_ID, stateId_, StatusEvent::FAILED);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::changeTo(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::CHANGE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::CHANGE, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::restart(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESTART});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESTART, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::resume(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RESUME});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RESUME, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::select(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SELECT});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SELECT, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::utilize(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::UTILIZE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId_);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::randomize(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::RANDOMIZE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId_);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::schedule(const StateID stateId_) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.requests.emplace(Transition{stateId_, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(_core.context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId_);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateChangeTo(const StateID stateId_) noexcept {
	changeTo(stateId_);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateRestart(const StateID stateId_) noexcept {
	restart(stateId_);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateResume(const StateID stateId_) noexcept {
	resume(stateId_);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateSelect(const StateID stateId_) noexcept {
	select(stateId_);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateUtilize(const StateID stateId_) noexcept {
	utilize(stateId_);

	processRequest();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::immediateRandomize(const StateID stateId_) noexcept {
	randomize(stateId_);

	processRequest();
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::reset() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets currentTransitions;
	PlanControl control{_core, currentTransitions};

	_apex.deepExit(control);

	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions.clear());

	_core.registry.clear();
	// TODO: clear _core.requests
	// TODO: clear _core.rng			// HFSM2_IF_UTILITY_THEORY()
	// TODO: clear _core.planData		// HFSM2_IF_PLANS()
	// TODO: clear _activityHistory		// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});
	_apex.deepEnter(control);
}

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::replayTransitions(const Transition* const transitions,
								const Short count) noexcept
{
	HFSM2_ASSERT(transitions);
	HFSM2_ASSERT(_core.registry.isActive());

	_core.transitionTargets  .clear();
	_core.previousTransitions.clear();

	if (HFSM2_CHECKED(transitions && count)) {
		TransitionSets currentTransitions;
		PlanControl control{_core, currentTransitions};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_core.previousTransitions.emplace(transitions[i]);

			_apex.deepChangeToRequested(control);

			_core.registry.clearRequests();

			HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::replayTransitions(const DynamicArrayT<Transition, NCount>& transitions) noexcept {
	if (transitions.count())
		return replayTransitions(&transitions[0],
								 transitions.count());
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
const typename R_<TG_, TA_>::Transition*
R_<TG_, TA_>::lastTransitionTo(const StateID stateId_) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	if (HFSM2_CHECKED(stateId_ < StateList::SIZE)) {
		const Short index = _core.transitionTargets[stateId_];

		if (index < _core.previousTransitions.count())
			return &_core.previousTransitions[index];
	}

	return nullptr;
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::initialEnter() noexcept {
	HFSM2_ASSERT(!_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(HFSM2_ASSERT(_core.previousTransitions.count() == 0));

	TransitionSets currentTransitions;
	TransitionSet  pendingTransitions;

	PlanControl control{_core, currentTransitions};

	_apex.deepRequestChange(control, {TransitionType::CHANGE, INVALID_SHORT});

	approvedByEntryGuards(currentTransitions,
						  pendingTransitions);

	RegistryBackUp backup;
	_core.registry.backup(backup);

	for (Long s = 0;
		 s < SUBSTITUTION_LIMIT && _core.requests.count();
		 ++s)
	{
		for (Short i = 0; i < _core.requests.count(); ++i) {
			const Transition& request = _core.requests[i];

			if (HFSM2_CHECKED(request.destination < STATE_COUNT))
				applyRequest(control, request, i);
		}

		if (_core.registry != backup) {
			pendingTransitions = _core.requests;
			_core.requests.clear();

			if (approvedByEntryGuards(currentTransitions,
									  pendingTransitions))
			{
				currentTransitions += pendingTransitions;
				_core.registry.backup(backup);
			}
			else {
				HFSM2_BREAK();

				_core.registry.restore(backup);
			}

			pendingTransitions.clear();
		}
		else
			_core.requests.clear();
	}
	HFSM2_ASSERT(_core.requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions = currentTransitions);

	_apex.deepEnter(control);

	_core.registry.clearRequests();

	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::finalExit() noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepExit(control);

	_core.registry.clear();
	_core.requests.clear();

#if HFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: _core.rng.clear();
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.transitionTargets  .clear();
	_core.previousTransitions.clear();
#endif

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processRequest() noexcept {
	HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());

	if (_core.requests.count()) {
		processTransitions(currentTransitions);

		HFSM2_IF_ASSERT(HFSM2_IF_PLANS(_core.planData.verifyPlans()));
		HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
	}

	HFSM2_IF_TRANSITION_HISTORY(_core.previousTransitions = currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::processTransitions(TransitionSets& currentTransitions) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());
	HFSM2_ASSERT(_core.requests.count());

	TransitionSet pendingTransitions;

	PlanControl control{_core, currentTransitions};

	RegistryBackUp backup;
	_core.registry.backup(backup);

	for (Long s = 0;
		 s < SUBSTITUTION_LIMIT && _core.requests.count();
		 ++s)
	{
		for (Short i = 0; i < _core.requests.count(); ++i) {
			const Transition& request = _core.requests[i];

			if (HFSM2_CHECKED(request.destination < STATE_COUNT))
				applyRequest(control, request, i);
		}

		if (_core.registry != backup) {
			pendingTransitions = _core.requests;
			_core.requests.clear();

			if (approvedByGuards(currentTransitions,
								 pendingTransitions))
			{
				currentTransitions += pendingTransitions;
				_core.registry.backup(backup);
			}
			else {
				HFSM2_IF_TRANSITION_HISTORY(_core.transitionTargets.clear());
				_core.registry.restore(backup);
			}

			pendingTransitions.clear();
		}
		else
			_core.requests.clear();
	}
	HFSM2_ASSERT(_core.requests.count() == 0);

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_core.registry.clearRequests();
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::applyRequest(Control& control,
						   const Transition& request,
						   const Short index) noexcept
{
	switch (request.type) {
	case TransitionType::CHANGE:
	case TransitionType::RESTART:
	case TransitionType::RESUME:
	case TransitionType::SELECT:

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case TransitionType::UTILIZE:
	case TransitionType::RANDOMIZE:
#endif

		if (request.destination == 0)
			_apex.deepRequest      (control, {request.type, index});
		else {
			_core.registry.requestImmediate(request);
			_apex.deepForwardActive(control, {request.type, index});
		}
		break;

	case TransitionType::SCHEDULE:
		_core.registry.requestScheduled(request.destination);
		break;

	default:
		HFSM2_BREAK();
	}
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::approvedByEntryGuards(const TransitionSets& currentTransitions,
									const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_core,
							  currentTransitions,
							  pendingTransitions};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::approvedByGuards(const TransitionSets& currentTransitions,
							   const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_core,
							  currentTransitions,
							  pendingTransitions};

	return _apex.deepForwardExitGuard(guardControl) &&
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::save(WriteStream& stream) const noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_apex.deepSaveActive(_core.registry, stream);

	// TODO: save(stream, _core.requests);

#if HFSM2_PLANS_AVAILABLE()
	// TODO: save(stream, _core.planData);
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: save(stream, _core.rng);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	// TODO: save(stream, _core.transitionTargets);
	// TODO: save(stream, _core.previousTransitions);
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: save(stream, _activityHistory);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::load(ReadStream& stream) noexcept {
	HFSM2_ASSERT(_core.registry.isActive());

	_core.registry.clearRequests();
	_core.registry.compoResumable.clear();
	_apex.deepLoadRequested(_core.registry, stream);

	_core.requests.clear();
	// TODO: load(stream, _core.requests);

#if HFSM2_PLANS_AVAILABLE()
	_core.planData.clear();
	// TODO: load(stream, _core.planData);
#endif

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	// TODO: load(stream, _core.rng);
#endif

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()
	_core.transitionTargets  .clear();
	_core.previousTransitions.clear();
#endif

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()
	// TODO: load(stream, _activityHistory);
#endif

	TransitionSets emptyTransitions;
	PlanControl control{_core, emptyTransitions};

	_apex.deepChangeToRequested(control);

	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
bool
R_<TG_, TA_>::applyRequests(Control& control,
							const Transition* const transitions,
							const Short count) noexcept
{
	if (HFSM2_CHECKED(transitions && count)) {
		RegistryBackUp backup;
		_core.registry.backup(backup);

		for (Short i = 0; i < count; ++i)
			applyRequest(control, transitions[i], i);

		return _core.registry != backup;
	}
	else
		return false;
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::getStateNames() noexcept {
	StructureStateInfos stateInfos;
	_apex.deepGetNames(static_cast<Long>(-1), RegionType::COMPOSITE, 0, stateInfos);

	Long margin = static_cast<Long>(-1);
	for (Long s = 0; s < stateInfos.count(); ++s) {
		const StructureStateInfo& stateInfo = stateInfos[s];
		Prefix& prefix = _prefixes[s];

		if (margin > stateInfo.depth && stateInfo.name[0] != '\0')
			margin = stateInfo.depth;

		if (stateInfo.depth == 0)
			prefix[0] = L'\0';
		else {
			const Long mark = stateInfo.depth * 2 - 1;

			prefix[mark + 0] = stateInfo.regionType == RegionType::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (Long d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (Long r = s; r > stateInfo.parent; --r) {
				Prefix& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = stateInfo.regionType == RegionType::COMPOSITE ? L'│' : L'║';
					break;
				case L'└':
					prefixAbove[mark] = L'├';
					break;
				case L'╙':
					prefixAbove[mark] = L'╟';
					break;
				}
			}
		}
	}
	if (margin > 0)
		margin -= 1;

	for (Long s = 0; s < stateInfos.count(); ++s) {
		const StructureStateInfo& stateInfo = stateInfos[s];
		Prefix& prefix = _prefixes[s];
		const Long space = stateInfo.depth * 2;

		if (stateInfo.name[0] != L'\0') {
			_structure[s] = StructureEntry{false, &prefix[margin * 2], stateInfo.name};
			_activityHistory[s] = static_cast<int8_t>(0);
		}
		else
		if (s + 1 < stateInfos.count()) {
			Prefix& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (Long c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const Long mark = space + 1;

			switch (nextPrefix[mark]) {
			case L'├':
				nextPrefix[mark] = stateInfo.depth == margin ? L'┌' : L'┬';
				break;
			case L'╟':
				nextPrefix[mark] = stateInfo.depth == margin ? L'╓' : L'╥';
				break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
HFSM2_CONSTEXPR(14)
void
R_<TG_, TA_>::udpateActivity() noexcept {
	for (Long s = 0, i = 0; s < STATE_COUNT; ++s) {
		_structure[i].isActive = isActive(s);

		typename ActivityHistory::Item& activity = _activityHistory[i];

		if (_structure[i].isActive) {
			if (activity < 0)
				activity = +1;
			else
				activity = activity < INT8_MAX ? activity + 1 : activity;
		} else {
			if (activity > 0)
				activity = -1;
			else
				activity = activity > INT8_MIN ? activity - 1 : activity;
		}

		++i;
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
