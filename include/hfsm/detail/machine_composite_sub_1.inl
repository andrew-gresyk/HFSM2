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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideForwardSubstitute(const unsigned prong,
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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideSubstitute(const unsigned prong,
															 Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepSubstitute(		control, context, logger);
	else
		remaining.wideSubstitute(prong, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnterInitial(Context& context,
															   LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideEnter(const unsigned prong,
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepEnter(	   context, logger);
	else
		remaining.wideEnter(prong, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdateAndTransition(const unsigned prong,
																	  Control& control,
																	  Context& context,
																	  LoggerInterface* const logger)
{
	return prong == ProngIndex ?
		initial  .deepUpdateAndTransition(		 control, context, logger) :
		remaining.wideUpdateAndTransition(prong, control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideUpdate(const unsigned prong,
														 Context& context,
														 LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepUpdate(		context, logger);
	else
		remaining.wideUpdate(prong, context, logger);
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
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepReact(	   event, control, context, logger);
	else
		remaining.wideReact(prong, event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideLeave(const unsigned prong,
														Context& context,
														LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial  .deepLeave(	   context, logger);
	else
		remaining.wideLeave(prong, context, logger);
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
		initial	 .deepForwardRequest(		transition);
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
																	Context& context,
																	LoggerInterface* const logger)
{
	if (prong == ProngIndex)
		initial	 .deepChangeToRequested(	   context, logger);
	else
		remaining.wideChangeToRequested(prong, context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideGetNames(const unsigned parent,
														   const unsigned depth,
														   StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Composite, depth, _stateInfos);
	remaining.wideGetNames(parent, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI, typename... TR>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI, TR...>::wideIsActive(const unsigned prong,
														   unsigned& index,
														   MachineStructure& structure) const
{
	initial.deepIsActive(prong == ProngIndex, index, structure);
	remaining.wideIsActive(prong, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
