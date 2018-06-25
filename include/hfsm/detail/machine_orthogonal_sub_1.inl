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
																	Context& context,
																	LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepForwardSubstitute(	   control, context, logger);
	else
		remaining.wideForwardSubstitute(prong, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(Control& control,
																	Context& context,
																	LoggerInterface* const logger)
{
	initial	 .deepForwardSubstitute(control, context, logger);
	remaining.wideForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	initial	 .deepSubstitute(control, context, logger);
	remaining.wideSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context,
															   LoggerInterface* const logger)
{
	initial  .deepEnterInitial(context, logger);
	remaining.wideEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideEnter(Context& context,
														LoggerInterface* const logger)
{
	initial  .deepEnter(context, logger);
	remaining.wideEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(Control& control,
																	  Context& context,
																	  LoggerInterface* const logger)
{
	return initial  .deepUpdateAndTransition(control, context, logger)
		|| remaining.wideUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(Context& context,
														 LoggerInterface* const logger)
{
	initial  .deepUpdate(context, logger);
	remaining.wideUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideReact(const TEvent& event,
														Control& control,
														Context& context,
														LoggerInterface* const logger)
{
	initial  .deepReact(event, control, context, logger);
	remaining.wideReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideLeave(Context& context,
														LoggerInterface* const logger)
{
	initial	 .deepLeave(context, logger);
	remaining.wideLeave(context, logger);
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
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI, TR...>::wideChangeToRequested(Context& context,
																	LoggerInterface* const logger)
{
	initial	 .deepChangeToRequested(context, logger);
	remaining.wideChangeToRequested(context, logger);
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
