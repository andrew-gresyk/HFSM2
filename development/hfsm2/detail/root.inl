namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
R_<TG, TA>::R_(Context& context
			 HFSM2_IF_UTILITY_THEORY(, RNG& rng)
			 HFSM2_IF_LOG_INTERFACE(, Logger* const logger)) noexcept
	: _context{context}
	HFSM2_IF_UTILITY_THEORY(, _rng{rng})
	HFSM2_IF_LOG_INTERFACE(, _logger{logger})
{
	_apex.deepRegister(_registry, Parent{});

	HFSM2_IF_STRUCTURE_REPORT(getStateNames());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(20)
R_<TG, TA>::~R_() noexcept {
	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::update() noexcept {
	HFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());

	if (_requests.count())
		processTransitions(currentTransitions);

	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::react(const TEvent& event) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	FullControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));

	TransitionSets currentTransitions;
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());

	if (_requests.count())
		processTransitions(currentTransitions);

	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::changeTo(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::CHANGE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::restart(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESTART});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::resume(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESUME});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::utilize(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::UTILIZE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::randomize(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RANDOMIZE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::schedule(const StateID stateId) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::reset() noexcept {
	HFSM2_ASSERT(_registry.isActive());

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit(control);

	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions.clear());

	_registry.clear();
	// TODO: clear _requests
	// TODO: clear _rng					// HFSM2_IF_UTILITY_THEORY()
	// TODO: clear _planData			// HFSM2_IF_PLANS()
	// TODO: clear _activityHistory		// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});
	_apex.deepEnter(control);
}

//------------------------------------------------------------------------------

#if HFSM2_SERIALIZATION_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::save(SerialBuffer& _buffer) const noexcept {
	HFSM2_ASSERT(_registry.isActive());

	WriteStream stream{_buffer};

	// TODO: save _registry
	// TODO: save _requests
	// TODO: save _rng						// HFSM2_IF_UTILITY_THEORY()
	// TODO: save _planData					// HFSM2_IF_PLANS()
	// TODO: save _previousTransitions		// HFSM2_IF_TRANSITION_HISTORY()
	// TODO: save _activityHistory			// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepSaveActive(_registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::load(const SerialBuffer& buffer) noexcept {
	HFSM2_ASSERT(_registry.isActive());

	_requests.clear();

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit(control);

	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions.clear());

	_registry.clear();
	_requests.clear();

	ReadStream stream{buffer};

	// TODO: load _registry
	// TODO: load _requests
	// TODO: load _rng					// HFSM2_IF_UTILITY_THEORY()
	// TODO: load _planData				// HFSM2_IF_PLANS()
	// TODO: load _previousTransitions	// HFSM2_IF_TRANSITION_HISTORY()
	// TODO: load _activityHistory		// HFSM2_IF_STRUCTURE_REPORT()

	_apex.deepLoadRequested(_registry, stream);

	_apex.deepEnter(control);
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransitions(const Transition* const transitions,
							  const Short count) noexcept
{
	HFSM2_ASSERT(transitions);
	HFSM2_ASSERT(_registry.isActive());

	_transitionTargets.clear();
	_previousTransitions.clear();

	if (HFSM2_CHECKED(transitions && count)) {
		PlanControl control{_context
						  , _registry
						  , _requests
						  HFSM2_IF_UTILITY_THEORY(, _rng)
						  HFSM2_IF_PLANS(, _planData)
						  , _transitionTargets
						  , _previousTransitions
						  HFSM2_IF_LOG_INTERFACE(, _logger)};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_previousTransitions.emplace(transitions[i]);

			_apex.deepChangeToRequested(control);

			_registry.clearRequests();

			HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::replayTransitions(const ArrayT<Transition, NCount>& transitions) noexcept {
	if (transitions.count())
		return replayTransitions(&transitions[0],
								 transitions.count());
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
const typename R_<TG, TA>::Transition*
R_<TG, TA>::lastTransitionTo(const StateID stateId) const noexcept {
	HFSM2_ASSERT(_registry.isActive());

	if (HFSM2_CHECKED(stateId < StateList::SIZE)) {
		const Short index = _transitionTargets[stateId];

		if (index < _previousTransitions.count())
			return &_previousTransitions[index];
	}

	return nullptr;
}

#endif

//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::initialEnter() noexcept {
	HFSM2_ASSERT(!_registry.isActive());
	HFSM2_ASSERT(_requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
	HFSM2_IF_TRANSITION_HISTORY(HFSM2_ASSERT(_previousTransitions.count() == 0));

	RegistryBackUp undo;

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepRequestChange(control, {TransitionType::RESTART, INVALID_SHORT});

	TransitionSets currentTransitions;
	TransitionSet  pendingTransitions;

	cancelledByEntryGuards(currentTransitions,
						   pendingTransitions);

	for (Long i = 0;
		 i < SUBSTITUTION_LIMIT && _requests.count();
		 ++i)
	{
		backup(_registry, undo);

		if (HFSM2_CHECKED(applyRequests(control))) {
			pendingTransitions = _requests;
			_requests.clear();

			if (cancelledByEntryGuards(currentTransitions,
									   pendingTransitions))
			{
				HFSM2_BREAK();

				restore(_registry, undo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_requests.clear();
	}
	HFSM2_ASSERT(_requests.count() == 0);
	HFSM2_IF_TRANSITION_HISTORY(_previousTransitions = currentTransitions);

	_apex.deepEnter(control);

	_registry.clearRequests();

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::finalExit() noexcept {
	HFSM2_ASSERT(_registry.isActive());

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::processTransitions(TransitionSets& currentTransitions) noexcept {
	HFSM2_ASSERT(_registry.isActive());
	HFSM2_ASSERT(_requests.count());

	RegistryBackUp registryUndo;

	PlanControl control{_context
					  , _registry
					  , _requests
					  HFSM2_IF_UTILITY_THEORY(, _rng)
					  HFSM2_IF_PLANS(, _planData)
					  HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
					  HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
					  HFSM2_IF_LOG_INTERFACE(, _logger)};

	TransitionSet pendingTransitions;

	for (Long i = 0;
		i < SUBSTITUTION_LIMIT && _requests.count();
		++i)
	{
		backup(_registry, registryUndo);

		if (applyRequests(control)) {
			pendingTransitions = _requests;
			_requests.clear();

			if (cancelledByGuards(currentTransitions, pendingTransitions)) {
				HFSM2_IF_TRANSITION_HISTORY(_transitionTargets.clear());
				restore(_registry, registryUndo);
			} else
				currentTransitions += pendingTransitions;

			pendingTransitions.clear();
		} else
			_requests.clear();
	}
	HFSM2_ASSERT(_requests.count() == 0);

	if (currentTransitions.count())
		_apex.deepChangeToRequested(control);

	_registry.clearRequests();

	HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
	HFSM2_IF_STRUCTURE_REPORT(udpateActivity());
}

// COMMON
//------------------------------------------------------------------------------

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequest(Control& control,
						 const Transition& request,
						 const Short index) noexcept
{
	switch (request.type) {
	case TransitionType::CHANGE:
	case TransitionType::RESTART:
	case TransitionType::RESUME:

#if HFSM2_UTILITY_THEORY_AVAILABLE()
	case TransitionType::UTILIZE:
	case TransitionType::RANDOMIZE:
#endif

		if (_registry.requestImmediate(request))
			_apex.deepForwardActive(control, {request.type, index});
		else
			_apex.deepRequest	   (control, {request.type, index});

		return true;

	case TransitionType::SCHEDULE:
		_registry.requestScheduled(request.destination);

		return false;

	default:
		HFSM2_BREAK();

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequests(Control& control) noexcept {
	bool changesMade = false;

	for (Short i = 0; i < _requests.count(); ++i)
		changesMade |= applyRequest(control, _requests[i], i);

	return changesMade;
}

//------------------------------------------------------------------------------
// COMMON

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByEntryGuards(const TransitionSets& currentTransitions,
								   const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _requests
							, currentTransitions
							, pendingTransitions
							HFSM2_IF_UTILITY_THEORY(, _rng)
							HFSM2_IF_PLANS(, _planData)
							HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
							HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
							HFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepEntryGuard(guardControl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::cancelledByGuards(const TransitionSets& currentTransitions,
							  const TransitionSet&  pendingTransitions) noexcept
{
	GuardControl guardControl{_context
							, _registry
							, _requests
							, currentTransitions
							, pendingTransitions
							HFSM2_IF_UTILITY_THEORY(, _rng)
							HFSM2_IF_PLANS(, _planData)
							HFSM2_IF_TRANSITION_HISTORY(, _transitionTargets)
							HFSM2_IF_TRANSITION_HISTORY(, _previousTransitions)
							HFSM2_IF_LOG_INTERFACE(, _logger)};

	return _apex.deepForwardExitGuard(guardControl) ||
		   _apex.deepForwardEntryGuard(guardControl);
}

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
bool
R_<TG, TA>::applyRequests(Control& control,
						  const Transition* const transitions,
						  const Short count) noexcept
{
	if (HFSM2_CHECKED(transitions && count)) {
		bool changesMade = false;

		for (Short i = 0; i < count; ++i)
			changesMade |= applyRequest(control, transitions[i], i);

		return changesMade;
	} else
		return false;
}

#endif

//------------------------------------------------------------------------------

#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::getStateNames() noexcept {
	_stateInfos.clear();
	_apex.deepGetNames((Long) -1, StructureStateInfo::RegionType::COMPOSITE, 0, _stateInfos);

	Long margin = (Long) -1;
	for (Long s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const Long mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (auto d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (auto r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.regionType == StructureStateInfo::RegionType::COMPOSITE ? L'│' : L'║';
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

	_structure.clear();
	for (Long s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const Long space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure.emplace(StructureEntry{false, &prefix[margin * 2], state.name});
			_activityHistory.emplace((int8_t) 0);
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (Long c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const Long mark = space + 1;

			switch (nextPrefix[mark]) {
			case L'├':
				nextPrefix[mark] = state.depth == margin ? L'┌' : L'┬';
				break;
			case L'╟':
				nextPrefix[mark] = state.depth == margin ? L'╓' : L'╥';
				break;
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
HFSM2_CONSTEXPR(14)
void
R_<TG, TA>::udpateActivity() noexcept {
	for (Long s = 0, i = 0; s < _stateInfos.count(); ++s)
		if (_stateInfos[s].name[0] != L'\0') {
			_structure[i].isActive = isActive(s);

			auto& activity = _activityHistory[i];

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

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const Transition* const transitions,
																											   const Short count) noexcept
{
	HFSM2_ASSERT(!_registry.isActive());
	HFSM2_ASSERT(_requests.count() == 0);

	_transitionTargets.clear();
	HFSM2_ASSERT(_previousTransitions.count() == 0);

	if (HFSM2_CHECKED(transitions && count)) {
		PlanControl control{_context
						  , _registry
						  , _requests
						  HFSM2_IF_UTILITY_THEORY(, _rng)
						  HFSM2_IF_PLANS(, _planData)
						  , _transitionTargets
						  , _previousTransitions
						  HFSM2_IF_LOG_INTERFACE(, _logger)};

		if (HFSM2_CHECKED(applyRequests(control, transitions, count))) {
			for (Short i = 0; i < count; ++i)
				_previousTransitions.emplace(transitions[i]);

			_apex.deepEnter(control);

			_registry.clearRequests();

			HFSM2_IF_PLANS(HFSM2_IF_ASSERT(_planData.verifyPlans()));
			HFSM2_IF_STRUCTURE_REPORT(udpateActivity());

			return true;
		}
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
template <Long NCount>
HFSM2_CONSTEXPR(14)
bool
RV_<G_<NFT, TC, Manual HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::replayEnter(const ArrayT<Transition, NCount>& transitions)  noexcept {
	if (transitions.count())
		return replayEnter(&transitions[0],
						   transitions.count());
	else
		return false;
}

#endif

////////////////////////////////////////////////////////////////////////////////
// COMMON

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																										  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::CHANGE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::changeWith(const StateID  stateId,
																											   Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::CHANGE, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// COMMON
//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::restartWith(const StateID  stateId,
																										   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESTART, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::restartWith(const StateID  stateId,
																												Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESTART, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::resumeWith(const StateID  stateId,
																										  const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESUME, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::resumeWith(const StateID  stateId,
																											   Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RESUME, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::utilizeWith(const StateID  stateId,
																				 const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::UTILIZE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::utilizeWith(const StateID  stateId,
																					  Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::UTILIZE, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::randomizeWith(const StateID  stateId,
																				   const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RANDOMIZE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV, typename TR, typename TU, typename TG, Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV, TR, TU, TG, NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::randomizeWith(const StateID  stateId,
																						Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::RANDOMIZE, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

#endif

//------------------------------------------------------------------------------

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::scheduleWith(const StateID  stateId,
																											const Payload& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <FeatureTag NFT, typename TC, typename TV HFSM2_IF_UTILITY_THEORY(, typename TR, typename TU, typename TG), Long NSL HFSM2_IF_PLANS(, Long NTC), typename TP, typename TA>
HFSM2_CONSTEXPR(14)
void
RP_<G_<NFT, TC, TV HFSM2_IF_UTILITY_THEORY(, TR, TU, TG), NSL HFSM2_IF_PLANS(, NTC), TP>, TA>::scheduleWith(const StateID  stateId,
																												 Payload&& payload) noexcept
{
	HFSM2_ASSERT(_registry.isActive());

	_requests.emplace(Transition{stateId, TransitionType::SCHEDULE, move(payload)});

	HFSM2_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
