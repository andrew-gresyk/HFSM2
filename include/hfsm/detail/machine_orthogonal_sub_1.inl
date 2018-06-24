namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::Sub(StateRegistry& stateRegistry,
											  const Index fork,
											  Parents& stateParents,
											  Parents& forkParents,
											  ForkPointers& forkPointers)
	: initial(stateRegistry,
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
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
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(Control& control,
																	Context& context)
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
															 Context& context)
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(Context& context) {
	initial.deepChangeToRequested(context);
	remaining.wideChangeToRequested(context);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideGetNames(const unsigned parent,
														   const unsigned depth,
														   StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Orthogonal, depth, _stateInfos);
	remaining.wideGetNames(parent, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideIsActive(const bool isActive,
														   unsigned& index,
														   MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
	remaining.wideIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
