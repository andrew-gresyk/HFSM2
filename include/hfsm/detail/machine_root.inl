namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
_R<TC, TPL, TMS, TA>::_R(Context& context
						 HFSM_IF_LOGGER(, LoggerInterface* const logger))
	: _context{context}
	, _apex{_stateRegistry2, Parent{}, _forkParents, _forkPointers}
	HFSM_IF_LOGGER(, _logger{logger})
{
	HFSM_IF_STRUCTURE(getStateNames());

	{
		Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepEnterInitial(control);
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
_R<TC, TPL, TMS, TA>::~_R() {
	Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
	_apex.deepExit(control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::update() {
	TransitionControl control(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));

	if (_apex.deepUpdate(control))
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TEvent>
void
_R<TC, TPL, TMS, TA>::react(const TEvent& event) {
	TransitionControl control(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));
	_apex.deepReact(event, control);

	if (_requests.count())
		processTransitions();

	_requests.clear();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::changeTo(const StateID stateId)	{
	const Transition transition(Transition::Type::RESTART, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resume(const StateID stateId) {
	const Transition transition(Transition::Type::RESUME, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::schedule(const StateID stateId) {
	const Transition transition(Transition::Type::SCHEDULE, stateId);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, stateId);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::changeTo(const StateID stateId,
							   TPayload* const payload)
{
	const Transition transition(Transition::Type::RESTART, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESTART, stateId);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::resume(const StateID stateId,
							 TPayload* const payload)
{
	const Transition transition(Transition::Type::RESUME, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::RESUME, state);
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::schedule(const StateID stateId,
							   TPayload* const payload)
{
	const Transition transition(Transition::Type::SCHEDULE, stateId, payload);
	_requests << transition;

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (_logger)
		_logger->recordTransition(LoggerInterface::Transition::SCHEDULE, state);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::resetStateData(const StateID stateId) {
	auto& stateInfo = _stateRegistry2[stateId];

	stateInfo.payload.reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
void
_R<TC, TPL, TMS, TA>::setStateData(const StateID stateId,
								   TPayload* const payload)
{
	auto& stateInfo = _stateRegistry2[stateId];

	stateInfo.payload = payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isStateDataSet(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	return !!stateInfo.payload;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename TPayload>
TPayload*
_R<TC, TPL, TMS, TA>::getStateData(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];
	const auto& payload = stateInfo.payload;

	return payload.template get<TPayload>();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isActive(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.active;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename T>
bool
_R<TC, TPL, TMS, TA>::isActive() const {
	constexpr auto id = stateId<T>();
	static_assert(id != INVALID_STATE_ID, "State not in FSM");

	return isActive(id);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
bool
_R<TC, TPL, TMS, TA>::isResumable(const StateID stateId) const {
	const auto& stateInfo = _stateRegistry2[stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_SHORT_INDEX)
			return parent.prong == fork.resumable;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
template <typename T>
bool
_R<TC, TPL, TMS, TA>::isResumable() const {
	constexpr auto id = stateId<T>();
	static_assert(id != INVALID_STATE_ID, "State not in FSM");

	return isResumable(id);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::processTransitions() {
	assert(_requests.count());

	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (unsigned i = 0;
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
			TransitionControl substitutionControl(_context, _requests, HFSM_LOGGER_OR(_logger, nullptr));
			_apex.deepForwardGuard(substitutionControl);

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << TransitionInfo(request, Method::GUARD);
		#endif
		}
	}

	{
		Control control{_context, HFSM_LOGGER_OR(_logger, nullptr)};
		_apex.deepChangeToRequested(control);
	}

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::requestImmediate(const Transition request) {
	const auto& stateInfo = _stateRegistry2[request.stateId];

	for (auto parent = stateInfo.parent; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::requestScheduled(const Transition request) {
	const auto& stateInfo = _stateRegistry2[request.stateId];

	const auto parent = stateInfo.parent;
	auto& fork = *_forkPointers[parent.fork];

	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::getStateNames() {
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

template <typename TC, typename TPL, ShortIndex TMS, typename TA>
void
_R<TC, TPL, TMS, TA>::udpateActivity() {
	for (auto& item : _structure)
		item.isActive = false;

	LongIndex index = 0;
	_apex.deepIsActive(true, index, _structure);

	for (LongIndex i = 0; i < _structure.count(); ++i) {
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
