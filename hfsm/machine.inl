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
M<TC, TMS>::_B<TI, TR...>::widePreSubstitute(Context& context) const {
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
M<TC, TMS>::_B<TI, TR...>::widePreTransition(Context& context) const {
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
M<TC, TMS>::_B<TI>::widePreSubstitute(Context& context) const {
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
M<TC, TMS>::_B<TI>::widePreTransition(Context& context) const {
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

#pragma region State

template <typename TC, unsigned TMS>
template <typename T>
M<TC, TMS>::_S<T>::_S(StateRegistry& stateRegistry,
					  const Parent parent,
					  Parents& stateParents,
					  Parents& /*forkParents*/,
					  ForkPointers& /*forkPointers*/)
{
	const auto id = stateRegistry.add(TypeInfo::get<T>());
	stateParents[id] = parent;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
bool
M<TC, TMS>::_S<T>::deepSubstitute(Control& control,
								  Context& context) const
{
	const unsigned requestCountBefore = control.requestCount();

	_client.widePreSubstitute(context);
	_client.substitute(control, context);

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepEnterInitial(Context& context) {
	deepEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepEnter(Context& context) {
	_client.widePreEnter(context);
	_client.enter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
bool
M<TC, TMS>::_S<T>::deepUpdateAndTransition(Control& control,
										   Context& context)
{
	_client.widePreUpdate(context);
	_client.update(context);

	const unsigned requestCountBefore = control.requestCount();

	_client.widePreTransition(context);
	_client.transition(control, context);

	return requestCountBefore < control.requestCount();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepUpdate(Context& context) {
	_client.widePreUpdate(context);
	_client.update(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepLeave(Context& context) {
	_client.leave(context);
	_client.widePostLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
template <typename TEvent>
void
M<TC, TMS>::_S<T>::deepReact(const TEvent& event,
							 Control& control,
							 Context& context)
{
	_client.widePreReact(event, context);
	_client.react(event, control, context);
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

#pragma region Composite::Substate

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
												  const Index fork,
												  Parents& stateParents,
												  Parents& forkParents,
												  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, ProngIndex, TypeInfo::get<T>(), TypeInfo::get<TI>()),
			  stateParents,
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry, fork, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
																	Control& control,
																	Context& context) const
{
	if (prong == ProngIndex)
		initial.deepForwardSubstitute(control, context);
	else
		remaining.wideForwardSubstitute(prong, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(const unsigned prong,
															 Control& control,
															 Context& context) const
{
	if (prong == ProngIndex)
		initial.deepSubstitute(control, context);
	else
		remaining.wideSubstitute(prong, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context) {
	initial.deepEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnter(const unsigned prong,
														Context& context)
{
	if (prong == ProngIndex)
		initial.deepEnter(context);
	else
		remaining.wideEnter(prong, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(const unsigned prong,
																	  Control& control,
																	  Context& context)
{
	return prong == ProngIndex ?
		initial.deepUpdateAndTransition(control, context) :
		remaining.wideUpdateAndTransition(prong, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(const unsigned prong,
														 Context& context)
{
	if (prong == ProngIndex)
		initial.deepUpdate(context);
	else
		remaining.wideUpdate(prong, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideReact(const unsigned prong,
														const TEvent& event,
														Control& control,
														Context& context)
{
	if (prong == ProngIndex)
		initial.deepReact(event, control, context);
	else
		remaining.wideReact(prong, event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideLeave(const unsigned prong,
														Context& context)
{
	if (prong == ProngIndex)
		initial.deepLeave(context);
	else
		remaining.wideLeave(prong, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardRequest(const unsigned prong,
																 const enum Transition::Type transition)
{
	if (prong == ProngIndex)
		initial.deepForwardRequest(transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideRequestResume(const unsigned prong) {
	if (prong == ProngIndex)
		initial.deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(const unsigned prong,
																	Context& context)
{
	if (prong == ProngIndex)
		initial.deepChangeToRequested(context);
	else
		remaining.wideChangeToRequested(prong, context);
}

#pragma endregion

//==============================================================================

#pragma region Composite::Substate

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::Sub(StateRegistry& stateRegistry,
										   const Index fork,
										   Parents& stateParents,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, ProngIndex, TypeInfo::get<T>(), TypeInfo::get<TI>()),
			  stateParents,
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_ASSERT_ONLY(prong),
															 Control& control,
															 Context& context) const
{
	assert(prong == ProngIndex);

	initial.deepForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideSubstitute(const unsigned HSFM_ASSERT_ONLY(prong),
													  Control& control,
													  Context& context) const
{
	assert(prong == ProngIndex);

	initial.deepSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context) {
	initial.deepEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnter(const unsigned HSFM_ASSERT_ONLY(prong),
												 Context& context)
{
	assert(prong == ProngIndex);

	initial.deepEnter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(const unsigned HSFM_ASSERT_ONLY(prong),
															   Control& control,
															   Context& context)
{
	assert(prong == ProngIndex);

	return initial.deepUpdateAndTransition(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdate(const unsigned HSFM_ASSERT_ONLY(prong),
												  Context& context)
{
	assert(prong == ProngIndex);

	initial.deepUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideReact(const unsigned HSFM_ASSERT_ONLY(prong),
												 const TEvent& event,
												 Control& control,
												 Context& context)
{
	assert(prong == ProngIndex);

	initial.deepReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideLeave(const unsigned HSFM_ASSERT_ONLY(prong),
												 Context& context)
{
	assert(prong == ProngIndex);

	initial.deepLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardRequest(const unsigned HSFM_ASSERT_ONLY(prong),
														  const enum Transition::Type transition)
{
	assert(prong == ProngIndex);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestResume(const unsigned HSFM_ASSERT_ONLY(prong)) {
	assert(prong == ProngIndex);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideChangeToRequested(const unsigned HSFM_ASSERT_ONLY(prong),
															 Context& context)
{
	assert(prong == ProngIndex);

	initial.deepChangeToRequested(context);
}

#pragma endregion

//==============================================================================

#pragma region Composite

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_C<T, TS...>::_C(StateRegistry& stateRegistry,
							 const Parent parent,
							 Parents& stateParents,
							 Parents& forkParents,
							 ForkPointers& forkPointers)
	: ForkT<T>(static_cast<Index>(forkPointers << this), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, Fork::self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context) const
{
	assert(Fork::requested != INVALID_INDEX);

	if (Fork::requested == Fork::active)
		_subStates.wideForwardSubstitute(Fork::requested, control, context);
	else
		_subStates.wideSubstitute(Fork::requested, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepSubstitute(Control& control,
										 Context& context) const
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::requested != INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(Fork::requested, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnterInitial(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX &&
		   Fork::requested == INVALID_INDEX);

	HSFM_DEBUG_ONLY(Fork::activeType = TypeInfo::get<typename SubStates::Initial::Client>());
	Fork::active = 0;

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepEnter(Context& context) {
	assert(Fork::active == INVALID_INDEX &&
		   Fork::requested != INVALID_INDEX);

	HSFM_DEBUG_ONLY(Fork::activeType = Fork::requestedType);
	Fork::active = Fork::requested;

	HSFM_DEBUG_ONLY(Fork::requestedType.clear());
	Fork::requested = INVALID_INDEX;

	_state.deepEnter(context);
	_subStates.wideEnter(Fork::active, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
bool
M<TC, TMS>::_C<T, TS...>::deepUpdateAndTransition(Control& control,
												  Context& context)
{
	assert(Fork::active != INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context)) {
		_subStates.wideUpdate(Fork::active, context);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(Fork::active, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepUpdate(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	_state.deepUpdate(context);
	_subStates.wideUpdate(Fork::active, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::deepReact(const TEvent& event,
									Control& control,
									Context& context)
{
	assert(Fork::active != INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(Fork::active, event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepLeave(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	_subStates.wideLeave(Fork::active, context);
	_state.deepLeave(context);

	HSFM_DEBUG_ONLY(Fork::resumableType = Fork::activeType);
	Fork::resumable = Fork::active;

	HSFM_DEBUG_ONLY(Fork::activeType.clear());
	Fork::active = INVALID_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	if (Fork::requested != INVALID_INDEX)
		_subStates.wideForwardRequest(Fork::requested, transition);
	else
		switch (transition) {
		case Transition::Remain:
			deepRequestRemain();
			break;

		case Transition::Restart:
			deepRequestRestart();
			break;

		case Transition::Resume:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestRemain() {
	if (Fork::active == INVALID_INDEX) {
		HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		Fork::requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestRestart() {
	HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
	Fork::requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepRequestResume() {
	if (Fork::resumable != INVALID_INDEX) {
		HSFM_DEBUG_ONLY(Fork::requestedType = Fork::resumableType);
		Fork::requested = Fork::resumable;
	} else {
		HSFM_DEBUG_ONLY(Fork::requestedType = TypeInfo::get<typename SubStates::Initial::Client>());
		Fork::requested = 0;
	}

	_subStates.wideRequestResume(Fork::requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_C<T, TS...>::deepChangeToRequested(Context& context) {
	assert(Fork::active != INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX) {
		if (Fork::requested == Fork::active)
			_subStates.wideChangeToRequested(Fork::requested, context);
		else {
			_subStates.wideLeave(Fork::active, context);

			HSFM_DEBUG_ONLY(Fork::resumableType = Fork::activeType);
			Fork::resumable = Fork::active;

			HSFM_DEBUG_ONLY(Fork::activeType = Fork::requestedType);
			Fork::active = Fork::requested;

			HSFM_DEBUG_ONLY(Fork::requestedType.clear());
			Fork::requested = INVALID_INDEX;

			_subStates.wideEnter(Fork::active, context);
		}
	}
}

#pragma endregion

////////////////////////////////////////////////////////////////////////////////

#pragma region Orthogonal::Substate

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
											  const Index fork,
											  Parents& stateParents,
											  Parents& forkParents,
											  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, ProngIndex, TypeInfo::get<T>(), TypeInfo::get<typename Initial::Client>()),
			  stateParents,
			  forkParents,
			  forkPointers)
	, remaining(stateRegistry, fork, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
																	Control& control,
																	Context& context) const
{
	if (prong == ProngIndex)
		initial.deepForwardSubstitute(control, context);
	else
		remaining.wideForwardSubstitute(prong, control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(Control& control,
																	Context& context) const
{
	initial.deepForwardSubstitute(control, context);
	remaining.wideForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(Control& control,
															 Context& context) const
{
	initial.deepSubstitute(control, context);
	remaining.wideSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context) {
	initial.deepEnterInitial(context);
	remaining.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnter(Context& context) {
	initial.deepEnter(context);
	remaining.wideEnter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(Control& control,
																	  Context& context)
{
	return initial.deepUpdateAndTransition(control, context)
		|| remaining.wideUpdateAndTransition(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(Context& context) {
	initial.deepUpdate(context);
	remaining.wideUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideReact(const TEvent& event,
														Control& control,
														Context& context)
{
	initial.deepReact(event, control, context);
	remaining.wideReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideLeave(Context& context) {
	initial.deepLeave(context);
	remaining.wideLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardRequest(const unsigned prong,
																 const enum Transition::Type transition)
{
	if (prong == ProngIndex) {
		initial.deepForwardRequest(transition);
		remaining.wideForwardRequest(prong, Transition::Remain);
	} else {
		initial.deepForwardRequest(Transition::Remain);
		remaining.wideForwardRequest(prong, transition);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
	remaining.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
	remaining.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideRequestResume() {
	initial.deepRequestResume();
	remaining.wideRequestResume();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(Context& context) {
	initial.deepChangeToRequested(context);
	remaining.wideChangeToRequested(context);
}

#pragma endregion

//==============================================================================

#pragma region Orthogonal::Substate

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::Sub(StateRegistry& stateRegistry,
										   const Index fork,
										   Parents& stateParents,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, ProngIndex, TypeInfo::get<T>(), TypeInfo::get<typename Initial::Client>()),
			  stateParents,
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_ASSERT_ONLY(prong),
															 Control& control,
															 Context& context) const
{
	assert(prong == ProngIndex);

	initial.deepForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(Control& control,
															 Context& context) const
{
	initial.deepForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideSubstitute(Control& control,
													  Context& context) const
{
	initial.deepSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context) {
	initial.deepEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnter(Context& context) {
	initial.deepEnter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(Control& control,
															   Context& context)
{
	return initial.deepUpdateAndTransition(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdate(Context& context) {
	initial.deepUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideReact(const TEvent& event,
												 Control& control,
												 Context& context)
{
	initial.deepReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideLeave(Context& context) {
	initial.deepLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardRequest(const unsigned prong,
														  const enum Transition::Type transition)
{
	assert(prong <= ProngIndex);

	if (prong == ProngIndex)
		initial.deepForwardRequest(transition);
	else
		initial.deepForwardRequest(Transition::Remain);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideRequestResume() {
	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideChangeToRequested(Context& context) {
	initial.deepChangeToRequested(context);
}

#pragma endregion

//==============================================================================

#pragma region Orthogonal

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
M<TC, TMS>::_O<T, TS...>::_O(StateRegistry& stateRegistry,
							 const Parent parent,
							 Parents& stateParents,
							 Parents& forkParents,
							 ForkPointers& forkPointers)
	: ForkT<T>(static_cast<Index>(forkPointers << this), parent, forkParents)
	, _state(stateRegistry, parent, stateParents, forkParents, forkPointers)
	, _subStates(stateRegistry, Fork::self, stateParents, forkParents, forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepForwardSubstitute(Control& control,
												Context& context) const
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX)
		_subStates.wideForwardSubstitute(Fork::requested, control, context);
	else
		_subStates.wideForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepSubstitute(Control& control,
										 Context& context) const
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (!_state.deepSubstitute(control, context))
		_subStates.wideSubstitute(control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnterInitial(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX &&
		   Fork::requested == INVALID_INDEX);

	_state.deepEnter(context);
	_subStates.wideEnterInitial(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepEnter(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_state.deepEnter(context);
	_subStates.wideEnter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
bool
M<TC, TMS>::_O<T, TS...>::deepUpdateAndTransition(Control& control,
												  Context& context)
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (_state.deepUpdateAndTransition(control, context)) {
		_subStates.wideUpdate(context);

		return true;
	} else
		return _subStates.wideUpdateAndTransition(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepUpdate(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_state.deepUpdate(context);
	_subStates.wideUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::deepReact(const TEvent& event,
									Control& control,
									Context& context)
{
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_state.deepReact(event, control, context);
	_subStates.wideReact(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepLeave(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideLeave(context);
	_state.deepLeave(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepForwardRequest(const enum Transition::Type transition) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	if (Fork::requested != INVALID_INDEX)
		_subStates.wideForwardRequest(Fork::requested, transition);
	else
		switch (transition) {
		case Transition::Remain:
			deepRequestRemain();
			break;

		case Transition::Restart:
			deepRequestRestart();
			break;

		case Transition::Resume:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestRemain() {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestRestart() {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepRequestResume() {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
void
M<TC, TMS>::_O<T, TS...>::deepChangeToRequested(Context& context) {
	assert(Fork::active    == INVALID_INDEX &&
		   Fork::resumable == INVALID_INDEX);

	_subStates.wideChangeToRequested(context);
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

	processRequests();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
template <typename TEvent>
void
M<TC, TMS>::_R<TA>::react(const TEvent& event) {
	Control control(_requests);
	_apex.deepReact(event, control, _context);

	processRequests();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TA>
void
M<TC, TMS>::_R<TA>::processRequests() {
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

		if (fork.active == INVALID_INDEX)
			continue;
		else
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

		if (fork.active == INVALID_INDEX)
			continue;
		else
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
