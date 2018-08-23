namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
_CS<NS, NC, NO, TA, NI, TI, TR...>::_CS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
	, remaining(stateParents,
				fork,
				forkParents,
				forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
											  FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGuard(const ShortIndex prong,
									   FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepGuard(control);
	else
		remaining.wideGuard(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(const ShortIndex prong,
									   PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepEnter(control);
	else
		remaining.wideEnter(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(const ShortIndex prong,
										FullControl& control)
{
	return prong == PRONG_INDEX ?
		initial  .deepUpdate(control) :
		remaining.wideUpdate(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const ShortIndex prong,
									   const TEvent& event,
									   FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepReact(	   event, control);
	else
		remaining.wideReact(prong, event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(const ShortIndex prong,
									  PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepExit(control);
	else
		remaining.wideExit(prong, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(const ShortIndex prong,
												const TransitionType transition)
{
	if (prong == PRONG_INDEX)
		initial	 .deepForwardRequest(		transition);
	else
		remaining.wideForwardRequest(prong, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(const ShortIndex prong) {
	if (prong == PRONG_INDEX)
		initial	 .deepRequestResume();
	else
		remaining.wideRequestResume(prong);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(const ShortIndex prong,
												   PlanControl& control)
{
	if (prong == PRONG_INDEX)
		initial	 .deepChangeToRequested(control);
	else
		remaining.wideChangeToRequested(prong, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_CS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
	remaining.wideGetNames(parent,								  depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
