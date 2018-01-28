namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

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
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(Control& control,
															 Context& context)
{
	initial.deepForwardSubstitute(control, context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideSubstitute(Control& control,
													  Context& context)
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

////////////////////////////////////////////////////////////////////////////////

}
