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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardSubstitute(const unsigned HSFM_IF_ASSERT(prong),
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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideSubstitute(const unsigned HSFM_IF_ASSERT(prong),
													  Control& control,
													  Context& context,
													  LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepSubstitute(control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnterInitial(Context& context,
														LoggerInterface* const logger)
{
	initial.deepEnterInitial(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideEnter(const unsigned HSFM_IF_ASSERT(prong),
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepEnter(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
bool
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdateAndTransition(const unsigned HSFM_IF_ASSERT(prong),
															   Control& control,
															   Context& context,
															   LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	return initial.deepUpdateAndTransition(control, context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideUpdate(const unsigned HSFM_IF_ASSERT(prong),
												  Context& context,
												  LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepUpdate(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
template <typename TEvent>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideReact(const unsigned HSFM_IF_ASSERT(prong),
												 const TEvent& event,
												 Control& control,
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepReact(event, control, context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideLeave(const unsigned HSFM_IF_ASSERT(prong),
												 Context& context,
												 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepLeave(context, logger);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideForwardRequest(const unsigned HSFM_IF_ASSERT(prong),
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
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideRequestResume(const unsigned HSFM_IF_ASSERT(prong)) {
	assert(prong == ProngIndex);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T, typename... TS>
template <unsigned TN, typename TI>
void
M<TC, TMS>::_C<T, TS...>::Sub<TN, TI>::wideChangeToRequested(const unsigned HSFM_IF_ASSERT(prong),
															 Context& context,
															 LoggerInterface* const logger)
{
	assert(prong == ProngIndex);

	initial.deepChangeToRequested(context, logger);
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
