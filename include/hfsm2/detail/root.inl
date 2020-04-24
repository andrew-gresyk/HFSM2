namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TA_>
R_<TG_, TA_>::R_(Context& context,
				 RNG& rng
				 HFSM_IF_LOGGER(, Logger* const logger))
	: _context{context}
	, _rng{rng}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
R_<TG_, TA_>::~R_() {
	PlanControl control{_context,
						_rng,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};

	_apex.deepExit	  (control);
	_apex.deepDestruct(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::update() {
	FullControl control(_context,
						_rng,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepUpdate(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
template <typename TEvent>
void
R_<TG_, TA_>::react(const TEvent& event) {
	FullControl control{_context,
						_rng,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepReact(control, event);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::changeTo(const StateID stateId) {
	_requests.append(Request{Request::Type::CHANGE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::CHANGE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::restart(const StateID stateId) {
	_requests.append(Request{Request::Type::RESTART, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::RESTART, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::resume(const StateID stateId) {
	_requests.append(Request{Request::Type::RESUME, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::RESUME, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::utilize(const StateID stateId) {
	_requests.append(Request{Request::Type::UTILIZE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::UTILIZE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::randomize(const StateID stateId) {
	_requests.append(Request{Request::Type::RANDOMIZE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::RANDOMIZE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::schedule(const StateID stateId) {
	_requests.append(Request{Request::Type::SCHEDULE, stateId});

	HFSM_LOG_TRANSITION(_context, INVALID_STATE_ID, Transition::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::initialEnter() {
	Control control{_context,
					_rng,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr)};

	AllForks undoRequested = _stateRegistry.requested;

	_apex.deepRequestChange(control);

	Requests lastRequests = _requests;
	_requests.clear();

	if (cancelledByEntryGuards(lastRequests))
		_stateRegistry.requested = undoRequested;

	for (LongIndex i = 0;
		 i < SUBSTITUTION_LIMIT && _requests.count();
		 ++i)
	{
		undoRequested = _stateRegistry.requested;

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByEntryGuards(lastRequests))
				_stateRegistry.requested = undoRequested;
		}

		_requests.clear();
	}
	HFSM_ASSERT(_requests.count() == 0);

	{
		PlanControl planControl{_context,
								_rng,
								_stateRegistry,
								_planData,
								HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepConstruct(planControl);
		_apex.deepEnter	   (planControl);

		_stateRegistry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	AllForks undoRequested;
	Requests lastRequests;

	Control control(_context,
					_rng,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr));

	for (LongIndex i = 0;
		i < SUBSTITUTION_LIMIT && _requests.count();
		++i)
	{
		undoRequested = _stateRegistry.requested;

		if (applyRequests(control)) {
			lastRequests = _requests;
			_requests.clear();

			if (cancelledByGuards(lastRequests))
				_stateRegistry.requested = undoRequested;
		} else
			_requests.clear();
	}

	{
		PlanControl planControl{_context,
								_rng,
								_stateRegistry,
								_planData,
								HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepChangeToRequested(planControl);
		_stateRegistry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
bool
R_<TG_, TA_>::applyRequests(Control& control) {
	bool changesMade = false;

	for (const Request& request : _requests) {
		HFSM_IF_STRUCTURE(_lastTransitions.append(TransitionInfo{request, Method::UPDATE}));

		switch (request.type) {
		case Request::CHANGE:
		case Request::RESTART:
		case Request::RESUME:
		case Request::UTILIZE:
		case Request::RANDOMIZE:
			if (_stateRegistry.requestImmediate(request))
				_apex.deepForwardActive(control, request.type);
			else
				_apex.deepRequest	   (control, request.type);

			changesMade = true;
			break;

		case Request::SCHEDULE:
			_stateRegistry.requestScheduled(request.stateId);
			break;

		default:
			HFSM_BREAK();
		}
	}

	return changesMade;
}

//------------------------------------------------------------------------------

template <typename TG_, typename TA_>
bool
R_<TG_, TA_>::cancelledByEntryGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context,
							  _rng,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
		HFSM_LOGGER_OR(_logger, nullptr)};

	if (_apex.deepEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
bool
R_<TG_, TA_>::cancelledByGuards(const Requests& pendingRequests) {
	GuardControl guardControl{_context,
							  _rng,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
							  HFSM_LOGGER_OR(_logger, nullptr)};

	if (_apex.deepForwardExitGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::EXIT_GUARD));

		return true;
	} else if (_apex.deepForwardEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::getStateNames() {
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

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::udpateActivity() {
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TA_>
void
R_<TG_, TA_>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_lastTransitions.append(TransitionInfo{request, method});
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
