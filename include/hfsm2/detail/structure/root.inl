namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::_R(Context& context
						HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	HFSM_IF_LOGGER(, _logger{logger})
{
	_apex.deepRegister(_stateRegistry, Parent{});

	HFSM_IF_STRUCTURE(getStateNames());

	for (auto& payload : _requestPayloads)
		payload.reset();

	initialEnter();

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::~_R() {
	PlanControl control{_context,
						_stateRegistry,
						_planData,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HFSM_IF_ASSERT(_planData.verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::update() {
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

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TEvent>
void
_R<TC, TG, TPL, TA>::react(const TEvent& event) {
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

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId) {
	const Request request(Request::Type::CHANGE, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::CHANGE, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::restart(const StateID stateId) {
	const Request request(Request::Type::RESTART, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId) {
	const Request request(Request::Type::RESUME, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::utilize(const StateID stateId) {
	const Request request(Request::Type::UTILIZE, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::UTILIZE, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId) {
	const Request request(Request::Type::SCHEDULE, stateId);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId,
							  TPayload* const payload)
{
	const Request request(Request::Type::CHANGE, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::CHANGE, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::restart(const StateID stateId,
							 TPayload* const payload)
{
	const Request request(Request::Type::RESTART, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId,
							TPayload* const payload)
{
	const Request request(Request::Type::RESUME, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::utilize(const StateID stateId,
							 TPayload* const payload)
{
	const Request request(Request::Type::UTILIZE, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::UTILIZE, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId,
							  TPayload* const payload)
{
	const Request request(Request::Type::SCHEDULE, stateId, payload);
	_requests << request;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_VERBOSE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resetStateData(const StateID stateId) {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_requestPayloads[stateId].reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::setStateData(const StateID stateId,
								  TPayload* const payload)
{
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_requestPayloads[stateId] = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::isStateDataSet(const StateID stateId) const {
	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		return !!_requestPayloads[stateId];
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TG, TPL, TA>::getStateData(const StateID stateId) const {
	using Payload = TPayload;

	HFSM_ASSERT(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		const auto& payload = _requestPayloads[stateId];

		return payload.template get<Payload>();
	} else
		return nullptr;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::initialEnter() {
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
		 i < MAX_SUBSTITUTIONS && _requests.count();
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
		_stateRegistry.clearOrthoRequested();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::processTransitions() {
	HFSM_ASSERT(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	AllForks undoRequested;
	_stateRegistry.requested.clear();

	Requests lastRequests;

	Control control(_context,
					_stateRegistry,
					_planData,
					HFSM_LOGGER_OR(_logger, nullptr));

	for (LongIndex i = 0;
		i < MAX_SUBSTITUTIONS && _requests.count();
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
		_stateRegistry.clearOrthoRequested();

		HFSM_IF_ASSERT(_planData.verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::applyRequests(Control& control) {
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

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::cancelledByEntryGuards(const Requests& pendingRequests) {
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

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::cancelledByGuards(const Requests& pendingRequests) {
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

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::getStateNames() {
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

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::udpateActivity() {
	for (LongIndex s = 0, i = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];

		if (state.name[0] != L'\0') {
			_structure[i].isActive = isActive(s);

			auto& activity = _activityHistory[i];

			if (_structure[i].isActive) {
				if (activity > 0)
					activity = activity < INT8_MAX ? activity + 1 : activity;
				else
					activity = +1;
			} else {
				if (activity > 0)
					activity = -1;
				else
					activity = activity > INT8_MIN ? activity - 1 : activity;
			}

			++i;
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::recordRequestsAs(const Method method) {
	for (const auto& request : _requests)
		_lastTransitions << TransitionInfo(request, method);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
