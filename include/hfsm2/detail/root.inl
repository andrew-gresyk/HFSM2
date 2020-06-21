namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
R_<TG, TA>::R_(Context& context
			   HFSM_IF_UTILITY_THEORY(, RNG& rng)
			   HFSM_IF_LOG_INTERFACE(, Logger* const logger))
	: _context{context}
	HFSM_IF_UTILITY_THEORY(, _rng{rng})
	HFSM_IF_LOG_INTERFACE(, _logger{logger})
{
	_apex.deepRegister(_registry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
R_<TG, TA>::~R_() {
	PlanControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::update() {
	FullControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						, _requests
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepUpdate(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
template <typename TEvent>
void
R_<TG, TA>::react(const TEvent& event) {
	FullControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						, _requests
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepReact(control, event);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::changeTo(const StateID stateId) {
	_requests.append(Request{Request::Type::CHANGE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::restart(const StateID stateId) {
	_requests.append(Request{Request::Type::RESTART, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::resume(const StateID stateId) {
	_requests.append(Request{Request::Type::RESUME, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef HFSM_ENABLE_UTILITY_THEORY

template <typename TG, typename TA>
void
R_<TG, TA>::utilize(const StateID stateId) {
	_requests.append(Request{Request::Type::UTILIZE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::randomize(const StateID stateId) {
	_requests.append(Request{Request::Type::RANDOMIZE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::RANDOMIZE, stateId);
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::schedule(const StateID stateId) {
	_requests.append(Request{Request::Type::SCHEDULE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, TransitionType::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::reset() {
	PlanControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	   (control);
	_apex.deepDestruct (control);

	_registry.reset();

	_apex.deepRequestChange(control);
	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_SERIALIZATION

template <typename TG, typename TA>
void
R_<TG, TA>::save(SerialBuffer& _buffer) const {
	WriteStream stream{_buffer};

	_apex.deepSaveActive(_registry, stream);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::load(const SerialBuffer& buffer) {
	PlanControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepExit	   (control);
	_apex.deepDestruct (control);

	ReadStream stream{buffer};
	_apex.deepLoadRequested(_registry, stream);

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_TRANSITION_HISTORY

template <typename TG, typename TA>
void
R_<TG, TA>::replayTransitions(const Transition* const transitions,
							  const uint64_t count)
{
	if (HFSM_CHECKED(transitions && count)) {
		HFSM_IF_TRANSITION_HISTORY(_transitionHistory.clear());

		PlanControl control{_context
							HFSM_IF_UTILITY_THEORY(, _rng)
							, _registry
							, _planData
							HFSM_IF_LOG_INTERFACE(, _logger)};

		if (applyRequests(control, transitions, count)) {
			_apex.deepChangeToRequested(control);

			_registry.clearRequests();

			HFSM_IF_ASSERT(_planData.verifyPlans());
		}

		HFSM_IF_STRUCTURE(udpateActivity());
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
R_<TG, TA>::initialEnter() {
	HFSM_ASSERT(_requests.count() == 0);
	HFSM_IF_TRANSITION_HISTORY(HFSM_ASSERT(_transitionHistory.count() == 0));

	RegistryBackUp undo;
	HFSM_IF_TRANSITION_HISTORY(TransitionHistory undoTransitionHistory);

	Requests lastRequests;

	PlanControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						HFSM_IF_LOG_INTERFACE(, _logger)};

	_apex.deepRequestChange(control);

	cancelledByEntryGuards(_requests);

	for (LongIndex i = 0;
		 i < SUBSTITUTION_LIMIT && _requests.count();
		 ++i)
	{
		backup(_registry, undo);
		HFSM_IF_TRANSITION_HISTORY(undoTransitionHistory = _transitionHistory);

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByEntryGuards(lastRequests)) {
				restore(_registry, undo);
				HFSM_IF_TRANSITION_HISTORY(_transitionHistory = undoTransitionHistory);
			}
		} else
			_requests.clear();
	}
	HFSM_ASSERT(_requests.count() == 0);

	_apex.deepConstruct(control);
	_apex.deepEnter	   (control);

	_registry.clearRequests();

	HFSM_IF_ASSERT(_planData.verifyPlans());

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_TRANSITION_HISTORY(_transitionHistory.clear());

	RegistryBackUp undo;
	HFSM_IF_TRANSITION_HISTORY(TransitionHistory undoTransitionHistory);

	Requests lastRequests;

	PlanControl control{_context
						HFSM_IF_UTILITY_THEORY(, _rng)
						, _registry
						, _planData
						HFSM_IF_LOG_INTERFACE(, _logger)};

	bool changesMade = false;

	for (LongIndex i = 0;
		i < SUBSTITUTION_LIMIT && _requests.count();
		++i)
	{
		backup(_registry, undo);
		HFSM_IF_TRANSITION_HISTORY(undoTransitionHistory = _transitionHistory);

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByGuards(lastRequests)) {
				restore(_registry, undo);
				HFSM_IF_TRANSITION_HISTORY(_transitionHistory = undoTransitionHistory);
			} else
				changesMade = true;
		} else
			_requests.clear();
	}

	if (changesMade) {
		_apex.deepChangeToRequested(control);

		_registry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequest(Control& control,
						 const Request& request)
{
	HFSM_IF_TRANSITION_HISTORY(_transitionHistory.append(Transition{request, Method::NONE}));

	switch (request.type) {
	case Request::CHANGE:
	case Request::RESTART:
	case Request::RESUME:

#ifdef HFSM_ENABLE_UTILITY_THEORY
	case Request::UTILIZE:
	case Request::RANDOMIZE:
#endif

		if (_registry.requestImmediate(request))
			_apex.deepForwardActive(control, request.type);
		else
			_apex.deepRequest	   (control, request.type);

		return true;

	case Request::SCHEDULE:
		_registry.requestScheduled(request.stateId);

		return false;

	default:
		HFSM_BREAK();

		return false;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequests(Control& control) {
	bool changesMade = false;

	for (const Request& request : _requests)
		changesMade |= applyRequest(control, request);

	return changesMade;
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByEntryGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context
							  HFSM_IF_UTILITY_THEORY(, _rng)
							  , _registry
							  , _planData
							  , _requests
							  , pendingRequests
							  HFSM_IF_LOG_INTERFACE(, _logger)};

	if (_apex.deepEntryGuard(guardControl)) {
		HFSM_IF_TRANSITION_HISTORY(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
R_<TG, TA>::cancelledByGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context
							  HFSM_IF_UTILITY_THEORY(, _rng)
							  , _registry
							  , _planData
							  , _requests
							  , pendingRequests
							  HFSM_IF_LOG_INTERFACE(, _logger)};

	if (_apex.deepForwardExitGuard(guardControl)) {
		HFSM_IF_TRANSITION_HISTORY(recordRequestsAs(Method::EXIT_GUARD));

		return true;
	} else if (_apex.deepForwardEntryGuard(guardControl)) {
		HFSM_IF_TRANSITION_HISTORY(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_TRANSITION_HISTORY

template <typename TG, typename TA>
bool
R_<TG, TA>::applyRequests(Control& control,
						  const Transition* const transitions,
						  const uint64_t count)
{
	if (HFSM_CHECKED(transitions && count)) {
		bool changesMade = false;

		for (uint64_t i = 0; i < count; ++i)
			changesMade |= applyRequest(control, transitions[i].request());

		return changesMade;
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
R_<TG, TA>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_transitionHistory.append(Transition{request, method});
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TG, typename TA>
void
R_<TG, TA>::getStateNames() {
	_stateInfos.clear();
	_apex.deepGetNames((LongIndex) -1, StructureStateInfo::COMPOSITE, 0, _stateInfos);

	LongIndex margin = (LongIndex) -1;
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const LongIndex mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.region == StructureStateInfo::COMPOSITE ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (auto d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (auto r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.region == StructureStateInfo::COMPOSITE ? L'│' : L'║';
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
	for (LongIndex s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const LongIndex space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure.append(StructureEntry{false, &prefix[margin * 2], state.name});
			_activityHistory.append((char) 0);
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (LongIndex c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const LongIndex mark = space + 1;

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
void
R_<TG, TA>::udpateActivity() {
	for (LongIndex s = 0, i = 0; s < _stateInfos.count(); ++s)
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

}
}
