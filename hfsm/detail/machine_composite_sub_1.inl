namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
																	Control& control,
																	Context& context)
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
															 Context& context)
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

////////////////////////////////////////////////////////////////////////////////

}
