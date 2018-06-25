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
			  Parent(fork,
					 ProngIndex
					 HSFM_IF_DEBUG(, TypeInfo::get<T>())
					 HSFM_IF_DEBUG(, TypeInfo::get<typename Initial::Head>())),
			  stateParents,
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_IF_ASSERT(prong),
															 Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(Control& control,
															 Context& context,
															 LoggerInterface* const logger)
{
	initial.deepForwardSubstitute(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideSubstitute(Control& control,
													  Context& context,
													  LoggerInterface* const logger)
{
	initial.deepSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context,
														LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideEnter(Context& context,
												 LoggerInterface* const logger)
{
	initial.deepEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(Control& control,
															   Context& context,
															   LoggerInterface* const logger)
{
	return initial.deepUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideUpdate(Context& context,
												  LoggerInterface* const logger)
{
	initial.deepUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideReact(const TEvent& event,
												 Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	initial.deepReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideLeave(Context& context,
												 LoggerInterface* const logger)
{
	initial.deepLeave(context, logger);
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
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideChangeToRequested(Context& context,
															 LoggerInterface* const logger)
{
	initial.deepChangeToRequested(context, logger);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideGetNames(const unsigned parent,
													const unsigned depth,
													StateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StateInfo::Orthogonal, depth, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_O<T, TS...>::Sub<TN, TI>::wideIsActive(const bool isActive,
													unsigned& index,
													MachineStructure& structure) const
{
	initial.deepIsActive(isActive, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
