namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

#pragma region Utility

template <typename TC, unsigned TMS>
M<TC, TMS>::Fork::Fork(const Index index,
					   const TypeInfo HSFM_DEBUG_ONLY(type_))
	: self(index)
#ifdef _DEBUG
	, type(type_)
#endif
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
M<TC, TMS>::_R<TA>::_R(Context& context)
	: _context(context)
	, _apex(_stateRegistry, Parent(), _stateParents, _forkParents, _forkPointers)
{
	_apex.deepEnterInitial(_context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TA>
M<TC, TMS>::_R<TA>::~_R() {
	_apex.deepLeave(_context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::update() {
	Control control(_requests);
	_apex.deepUpdateAndTransition(control, _context);

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
	_apex.deepReact(event, control, _context);

	processTransitions();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::processTransitions() {
	for (unsigned i = 0; i < MaxSubstitutions && _requests.count(); ++i) {
		unsigned changeCount = 0;

		for (const auto& request : _requests)
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
		_requests.clear();

		if (changeCount > 0) {
			Control substitutionControl(_requests);
			_apex.deepForwardSubstitute(substitutionControl, _context);
		}
	}

	_apex.deepChangeToRequested(_context);
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
M<TC, TMS>::_R<TA>::requestImmediate(const Transition request) {
	const unsigned state = id(request);

	for (auto parent = _stateParents[state]; parent; parent = _forkParents[parent.fork]) {
		auto& fork = *_forkPointers[parent.fork];

		HSFM_DEBUG_ONLY(fork.requestedType = parent.prongType);
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

	HSFM_ASSERT_ONLY(const auto forksParent = _stateParents[fork.self]);
	HSFM_ASSERT_ONLY(const auto& forksFork = *_forkPointers[forksParent.fork]);
	assert(forksFork.active == INVALID_INDEX);

	HSFM_DEBUG_ONLY(fork.resumableType = parent.prongType);
	fork.resumable = parent.prong;
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

}

#include "detail/machine_state.inl"
#include "detail/machine_composite.inl"
#include "detail/machine_orthogonal.inl"
