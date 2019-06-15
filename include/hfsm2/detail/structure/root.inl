namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG, typename TA>
_R<TG, TA>::_R(Context& context
			   HFSM_IF_LOGGER(, Logger* const logger))
	: _context{context}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	initialEnter();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
_R<TG, TA>::~_R() {
	PlanControl control{_context,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::update() {
	FullControl control(_context,
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

template <typename TG, typename TA>
template <typename TEvent>
void
_R<TG, TA>::react(const TEvent& event) {
	FullControl control(_context,
						_stateRegistry,
						_planData,
						_requests,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(control, event);

	HFSM_IF_ASSERT(_planData.verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::changeTo(const StateID stateId) {
	const Request request{Request::Type::CHANGE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::CHANGE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::changeTo(const StateID stateId,
					 const Payload& payload)
{
	const Request request{Request::Type::CHANGE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::CHANGE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::restart(const StateID stateId) {
	const Request request{Request::Type::RESTART, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESTART, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::restart(const StateID stateId,
					const Payload& payload)
{
	const Request request{Request::Type::RESTART, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESTART, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::resume(const StateID stateId) {
	const Request request{Request::Type::RESUME, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESUME, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::resume(const StateID stateId,
				   const Payload& payload)
{
	const Request request{Request::Type::RESUME, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::RESUME, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::utilize(const StateID stateId) {
	const Request request{Request::Type::UTILIZE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::UTILIZE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::utilize(const StateID stateId,
					const Payload& payload)
{
	const Request request{Request::Type::UTILIZE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::UTILIZE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::schedule(const StateID stateId) {
	const Request request{Request::Type::SCHEDULE, stateId};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::schedule(const StateID stateId,
					 const Payload& payload)
{
	const Request request{Request::Type::SCHEDULE, stateId, payload};
	_requests << request;

	HFSM_LOG_TRANSITION(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::resetStateData(const StateID stateId) {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_payloadsSet.reset(stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::setStateData(const StateID stateId,
						 const Payload& payload)
{
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		_payloads[stateId] = payload;
		_payloadsSet.set(stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
_R<TG, TA>::isStateDataSet(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	return stateId < Payloads::CAPACITY ?
		_payloadsSet.get(stateId) : false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
const typename _R<TG, TA>::Payload*
_R<TG, TA>::getStateData(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	return stateId < Payloads::CAPACITY && _payloadsSet.get(stateId) ?
		&_payloads[stateId] : nullptr;
}

//------------------------------------------------------------------------------

template <typename TG, typename TA>
void
_R<TG, TA>::initialEnter() {
	Control control(_context,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr));

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
								_stateRegistry,
								_planData,
								HFSM_LOGGER_OR(_logger, nullptr)};

		_apex.deepEnterRequested(planControl);
		_stateRegistry.clearRequests();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
void
_R<TG, TA>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	AllForks undoRequested;
	Requests lastRequests;

	Control control(_context,
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

template <typename TG, typename TA>
bool
_R<TG, TA>::applyRequests(Control& control) {
	bool changesMade = false;

	for (const Request& request : _requests) {
		HFSM_IF_STRUCTURE(_lastTransitions << TransitionInfo(request, Method::UPDATE));

		switch (request.type) {
		case Request::CHANGE:
		case Request::RESTART:
		case Request::RESUME:
		case Request::UTILIZE:
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

template <typename TG, typename TA>
bool
_R<TG, TA>::cancelledByEntryGuards(const Requests& pendingRequests) {
	GuardControl guardControl(_context,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
							  HFSM_LOGGER_OR(_logger, nullptr));

	if (_apex.deepEntryGuard(guardControl)) {
		HFSM_IF_STRUCTURE(recordRequestsAs(Method::ENTRY_GUARD));

		return true;
	} else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG, typename TA>
bool
_R<TG, TA>::cancelledByGuards(const Requests& pendingRequests) {
	GuardControl guardControl(_context,
							  _stateRegistry,
							  _planData,
							  _requests,
							  pendingRequests,
							  HFSM_LOGGER_OR(_logger, nullptr));

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

template <typename TG, typename TA>
void
_R<TG, TA>::getStateNames() {
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
			_structure << StructureEntry { false, &prefix[margin * 2], state.name };
			_activityHistory << (char) 0;
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
_R<TG, TA>::udpateActivity() {
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

template <typename TG, typename TA>
void
_R<TG, TA>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_lastTransitions << TransitionInfo(request, method);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
