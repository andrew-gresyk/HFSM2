namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::_R(Context& context
						 HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	, _apex{_registry.stateParents, Parent{}, _registry.forkParents, _registry.forkPointers}
	HFSM_IF_LOGGER(, _logger{logger})
{
	HFSM_IF_STRUCTURE(getStateNames());

	for (auto& payload : _transitionPayloads)
		payload.reset();

	{
		PlanControl control{_context,
							_registry,
							_tasks,
							_stateTasks,
							HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepEnterInitial(control);

		HSFM_IF_DEBUG(verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
_R<TC, TG, TPL, TA>::~_R() {
	PlanControl control{_context,
						_registry,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);

	HSFM_IF_DEBUG(verifyPlans());
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::update() {
	FullControl control(_context,
						_registry,
						_requests,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepUpdate(control);

	HSFM_IF_DEBUG(verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TEvent>
void
_R<TC, TG, TPL, TA>::react(const TEvent& event) {
	FullControl control(_context,
						_registry,
						_requests,
						_tasks,
						_stateTasks,
						HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(event, control);

	HSFM_IF_DEBUG(verifyPlans());

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId) {
	const Transition transition(Transition::Type::RESTART, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId) {
	const Transition transition(Transition::Type::RESUME, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId) {
	const Transition transition(Transition::Type::SCHEDULE, stateId);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::changeTo(const StateID stateId,
							  TPayload* const payload)
{
	const Transition transition(Transition::Type::RESTART, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::resume(const StateID stateId,
							TPayload* const payload)
{
	const Transition transition(Transition::Type::RESUME, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::schedule(const StateID stateId,
							  TPayload* const payload)
{
	const Transition transition(Transition::Type::SCHEDULE, stateId, payload);
	_requests << transition;

#if defined HFSM_ENABLE_LOG_INTERFACE || defined HFSM_FORCE_DEBUG_LOG
	if (_logger)
		_logger->recordTransition(INVALID_STATE_ID, LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::resetStateData(const StateID stateId) {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_transitionPayloads[stateId].reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
void
_R<TC, TG, TPL, TA>::setStateData(const StateID stateId,
								  TPayload* const payload)
{
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		_transitionPayloads[stateId] = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
bool
_R<TC, TG, TPL, TA>::isStateDataSet(const StateID stateId) const {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY)
		return !!_transitionPayloads[stateId];
	else
		return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TG, TPL, TA>::getStateData(const StateID stateId) const {
	assert(stateId < Payloads::CAPACITY);

	if (stateId < Payloads::CAPACITY) {
		const auto& payload = _transitionPayloads[stateId];

		return payload.template get<TPayload>();
	} else
		return nullptr;
}

//------------------------------------------------------------------------------

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::processTransitions() {
	assert(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (LongIndex i = 0;
		i < MAX_SUBSTITUTIONS && _requests.count();
		++i)
	{
		unsigned changeCount = 0;

		for (const auto& request : _requests) {
			HFSM_IF_STRUCTURE(_lastTransitions << TransitionInfo(request, Method::UPDATE));

			switch (request.type) {
			case Transition::RESTART:
			case Transition::RESUME:
				requestImmediate(request);

				++changeCount;
				break;

			case Transition::SCHEDULE:
				requestScheduled(request);
				break;

			default:
				assert(false);
			}
		}
		_requests.clear();

		if (changeCount > 0) {
			FullControl substitutionControl(_context,
											_registry,
											_requests,
											_tasks,
											_stateTasks,
											HFSM_LOGGER_OR(_logger, nullptr));
			_apex.deepForwardGuard(substitutionControl);

			HSFM_IF_DEBUG(verifyPlans());

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << TransitionInfo(request, Method::GUARD);
		#endif
		}
	}

	{
		PlanControl control{_context,
							_registry,
							_tasks,
							_stateTasks,
							HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepChangeToRequested(control);

		HSFM_IF_DEBUG(verifyPlans());
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::requestImmediate(const Transition request) {
	assert(STATE_COUNT > request.stateId);

	for (auto parent = _registry.stateParents[request.stateId]; parent; parent = _registry.forkParents[parent.fork]) {
		auto& fork = *_registry.forkPointers[parent.fork];

		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::requestScheduled(const Transition request) {
	assert(STATE_COUNT > request.stateId);

	const auto parent = _registry.stateParents[request.stateId];
	auto& fork = *_registry.forkPointers[parent.fork];

	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef _DEBUG

template <typename TC, typename TG, typename TPL, typename TA>
void
_R<TC, TG, TPL, TA>::verifyPlans() const {
	LongIndex planCount = 0;
	for (StateID id = 0; id < STATE_COUNT; ++id)
		planCount += verifyPlan(id);

	assert(_tasks.count() == planCount);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TG, typename TPL, typename TA>
LongIndex
_R<TC, TG, TPL, TA>::verifyPlan(const StateID stateId) const {
	LongIndex length = 0;
	const TaskIndices& indices = _stateTasks[stateId];

	if (indices.first != INVALID_LONG_INDEX) {
		assert(indices.last != INVALID_LONG_INDEX);

		for (auto c = indices.first, d = c; ; ) {
			++length;
			const TaskTransition& task = _tasks[c];

			if (c != indices.last) {
				assert(task.next != INVALID_LONG_INDEX);
				c = task.next;

				// loop check
				if (d != INVALID_LONG_INDEX) {
					d = _tasks[d].next;

					if (d != INVALID_LONG_INDEX) {
						d = _tasks[d].next;
					}

					if (d != INVALID_LONG_INDEX)
						assert(c != d);
				}
			} else {
				assert(task.next == INVALID_LONG_INDEX);

				break;
			}
		};
	} else
		assert(indices.last == INVALID_LONG_INDEX);

	return length;
}

#endif

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
	for (LongIndex i = 0; i < _structure.count(); ++i) {
		_structure[i].isActive = isActive(i);

		auto& activity = _activityHistory[i];

		if (_structure[i].isActive) {
			if (activity > 0)
				activity = activity < INT8_MIN ? activity + 1 : activity;
			else
				activity = +1;
		} else {
			if (activity > 0)
				activity = -1;
			else
				activity = activity > INT8_MIN ? activity - 1 : activity;
		}
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
