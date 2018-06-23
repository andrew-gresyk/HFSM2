namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::Sub(StateRegistry& stateRegistry,
										   const Index fork,
										   Parents& stateParents,
										   Parents& forkParents,
										   ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork, ProngIndex, TypeInfo::get<T>(), TypeInfo::get<typename Initial::Head>()),
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
															 Context& context)
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
													  Context& context)
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

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideGetNames(const unsigned parent,
													const unsigned depth,
													StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Composite, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideIsActive(const unsigned prong,
													unsigned& index,
													MachineStructure& structure) const
{
	initial.deepIsActive(prong == ProngIndex, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
