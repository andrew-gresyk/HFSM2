namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

#pragma region Utility

template <typename TC, unsigned TMS>
M<TC, TMS>::Fork::Fork(const Index index,
					   const TypeInfo HSFM_IF_DEBUG(type_))
	: self(index)
	HSFM_IF_DEBUG(, type(type_))
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <unsigned TCapacity>
unsigned
M<TC, TMS>::StateRegistryT<TCapacity>::add(const TypeInfo stateType) {
	const unsigned index = _typeToIndex.count();

	HSFM_CHECKED(_typeToIndex.insert(*stateType, index));

	return index;
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

#pragma region Injections

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreSubstitute(Context& context) {
	TI::preSubstitute(context);
	_B<TR...>::widePreSubstitute(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreEnter(Context& context) {
	TI::preEnter(context);
	_B<TR...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreUpdate(Context& context) {
	TI::preUpdate(context);
	_B<TR...>::widePreUpdate(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePreTransition(Context& context) {
	TI::preTransition(context);
	_B<TR...>::widePreTransition(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_B<TI, TR...>::widePreReact(const TEvent& event,
										Context& context)
{
	TI::preReact(event, context);
	_B<TR...>::widePreReact(event, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI, typename... TR>
void
M<TC, TMS>::_B<TI, TR...>::widePostLeave(Context& context) {
	TI::postLeave(context);
	_B<TR...>::widePostLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreSubstitute(Context& context) {
	TI::preSubstitute(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreEnter(Context& context) {
	TI::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreUpdate(Context& context) {
	TI::preUpdate(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePreTransition(Context& context) {
	TI::preTransition(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
template <typename TEvent>
void
M<TC, TMS>::_B<TI>::widePreReact(const TEvent& event,
								 Context& context)
{
	TI::preReact(event, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TI>
void
M<TC, TMS>::_B<TI>::widePostLeave(Context& context) {
	TI::postLeave(context);
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

#pragma region Root

template <typename TC, unsigned TMS>
template <typename TA>
M<TC, TMS>::_R<TA>::_R(Context& context
					   HFSM_IF_LOGGER(, LoggerInterface* const logger = nullptr))
	: _context(context)
	, _apex(_stateRegistry, Parent(), _stateParents, _forkParents, _forkPointers)
	HFSM_IF_LOGGER(, _logger(logger))
{
	HFSM_IF_STRUCTURE(getStateNames());

	_apex.deepEnterInitial(_context, HFSM_LOGGER_OR(_logger, nullptr));

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
M<TC, TMS>::_R<TA>::~_R() {
	_apex.deepLeave(_context, HFSM_LOGGER_OR(_logger, nullptr));
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::update() {
	Control control(_requests);
	_apex.deepUpdateAndTransition(control, _context, HFSM_LOGGER_OR(_logger, nullptr));

	if (_requests.count())
		processTransitions();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
template <typename TEvent>
void
M<TC, TMS>::_R<TA>::react(const TEvent& event) {
	Control control(_requests);
	_apex.deepReact(event, control, _context, HFSM_LOGGER_OR(_logger, nullptr));

	if (_requests.count())
		processTransitions();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
template <typename T>
bool
M<TC, TMS>::_R<TA>::isActive() {
	using Type = T;

	const auto stateType = TypeInfo::get<Type>();
	const auto state = _stateRegistry[*stateType];

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_INDEX)
			return parent.prong == fork.active;
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
template <typename T>
bool
M<TC, TMS>::_R<TA>::isResumable() {
	using Type = T;

	const auto stateType = TypeInfo::get<Type>();
	const auto state = _stateRegistry[*stateType];

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		const auto& fork = *_forkPointers[parent.fork];

		if (fork.active != INVALID_INDEX)
			return parent.prong == fork.resumable;
	}

	return false;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::processTransitions() {
	HFSM_IF_STRUCTURE(_lastTransitions.clear());

	for (unsigned i = 0;
		i < MaxSubstitutions && _requests.count();
		++i)
	{
		unsigned changeCount = 0;

		for (const auto& request : _requests) {
			HFSM_IF_STRUCTURE(_lastTransitions << DebugTransitionInfo(request, DebugTransitionInfo::Update));

			switch (request.type) {
			case Transition::Restart:
			case Transition::Resume:
				requestImmediate(request);

				++changeCount;
				break;

			case Transition::Schedule:
				requestScheduled(request);
				break;

			default:
				assert(false);
			}
		}
		_requests.clear();

		if (changeCount > 0) {
			Control substitutionControl(_requests);
			_apex.deepForwardSubstitute(substitutionControl, _context, HFSM_LOGGER_OR(_logger, nullptr));

		#ifdef HFSM_ENABLE_STRUCTURE_REPORT
			for (const auto& request : _requests)
				_lastTransitions << DebugTransitionInfo(request, DebugTransitionInfo::Substitute);
		#endif
		}
	}

	_apex.deepChangeToRequested(_context, HFSM_LOGGER_OR(_logger, nullptr));

	HFSM_IF_STRUCTURE(udpateActivity());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::requestImmediate(const Transition request) {
	const unsigned state = id(request);

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		HSFM_IF_DEBUG(fork.requestedType = parent.prongType);
		fork.requested = parent.prong;
	}

	_apex.deepForwardRequest(request.type);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::requestScheduled(const Transition request) {
	const unsigned state = id(request);

	const auto parent = _stateParents[state];
	auto& fork = *_forkPointers[parent.fork];

	HSFM_IF_ASSERT(const auto forksParent = _stateParents[fork.self]);
	HSFM_IF_ASSERT(const auto& forksFork = *_forkPointers[forksParent.fork]);
	assert(forksFork.active == INVALID_INDEX);

	HSFM_IF_DEBUG(fork.resumableType = parent.prongType);
	fork.resumable = parent.prong;
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::getStateNames() {
	_stateInfos.clear();
	_apex.deepGetNames((unsigned) -1, StateInfo::Composite, 0, _stateInfos);

	unsigned margin = (unsigned) -1;
	for (unsigned s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix      = _prefixes[s];

		if (margin > state.depth && state.name[0] != '\0')
			margin = state.depth;

		if (state.depth == 0)
			prefix[0] = L'\0';
		else {
			const auto mark = state.depth * 2 - 1;

			prefix[mark + 0] = state.region == StateInfo::Composite ? L'└' : L'╙';
			prefix[mark + 1] = L' ';
			prefix[mark + 2] = L'\0';

			for (unsigned d = mark; d > 0; --d)
				prefix[d - 1] = L' ';

			for (unsigned r = s; r > state.parent; --r) {
				auto& prefixAbove = _prefixes[r - 1];

				switch (prefixAbove[mark]) {
				case L' ':
					prefixAbove[mark] = state.region == StateInfo::Composite ? L'│' : L'║';
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
	for (unsigned s = 0; s < _stateInfos.count(); ++s) {
		const auto& state = _stateInfos[s];
		auto& prefix = _prefixes[s];
		const auto space = state.depth * 2;

		if (state.name[0] != L'\0') {
			_structure << StructureEntry { false, &prefix[margin * 2], state.name };
			_activityHistory << (char) 0;
		} else if (s + 1 < _stateInfos.count()) {
			auto& nextPrefix = _prefixes[s + 1];

			if (s > 0)
				for (unsigned c = 0; c <= space; ++c)
					nextPrefix[c] = prefix[c];

			const auto mark = space + 1;

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

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::udpateActivity() {
	for (auto& item : _structure)
		item.isActive = false;

	unsigned index = 0;
	_apex.deepIsActive(true, index, _structure);

	for (unsigned i = 0; i < _structure.count(); ++i) {
		auto& activity = _activityHistory[i];

		if (_structure[i].isActive) {
			if (activity > 0)
				activity = activity < std::numeric_limits<char>::max() ? activity + 1 : activity;
			else
				activity = +1;
		} else {
			if (activity > 0)
				activity = -1;
			else
				activity = activity > std::numeric_limits<char>::min() ? activity - 1 : activity;
		}
	}
}

#endif

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

}

#include "machine_state.hpp"
#include "machine_composite.hpp"
#include "machine_orthogonal.hpp"
