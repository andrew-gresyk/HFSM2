namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
_OS<NS, NC, NO, TA, NI, TI, TR...>::_OS(Registry& registry,
										const ForkID fork)
	: initial  {registry, Parent{fork, PRONG_INDEX}}
	, remaining{registry, fork}
{}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(const ShortIndex prong,
													 FullControl& control)
{
	if (prong == PRONG_INDEX)
		initial  .deepForwardGuard(control);
	else
		remaining.wideForwardGuard(prong, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardGuard(FullControl& control) {
	initial	 .deepForwardGuard(control);
	remaining.wideForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideGuard(FullControl& control) {
	initial	 .deepGuard(control);
	remaining.wideGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnterInitial(PlanControl& control) {
	initial  .deepEnterInitial(control);
	remaining.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideEnter(PlanControl& control) {
	initial  .deepEnter(control);
	remaining.wideEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
Status
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideUpdate(FullControl& control) {
	const auto status =	   initial	.deepUpdate(control);
	return combine(status, remaining.wideUpdate(control));
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
template <typename TEvent>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideReact(const TEvent& event,
									   FullControl& control)
{
	initial  .deepReact(event, control);
	remaining.wideReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideExit(PlanControl& control) {
	initial	 .deepExit(control);
	remaining.wideExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideForwardRequest(Registry& registry,
													   const ShortIndex prong,
													   const TransitionType transition)
{
	if (prong == PRONG_INDEX) {
		initial	 .deepForwardRequest(registry, 		  transition);
		remaining.wideForwardRequest(registry, prong, Transition::REMAIN);
	} else {
		initial	 .deepForwardRequest(registry, 		  Transition::REMAIN);
		remaining.wideForwardRequest(registry, prong, transition);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRemain(Registry& registry) {
	initial	 .deepRequestRemain(registry);
	remaining.wideRequestRemain(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestRestart(Registry& registry) {
	initial	 .deepRequestRestart(registry);
	remaining.wideRequestRestart(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideRequestResume(Registry& registry) {
	initial	 .deepRequestResume(registry);
	remaining.wideRequestResume(registry);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideChangeToRequested(Registry& registry,
														  PlanControl& control)
{
	initial	 .deepChangeToRequested(registry, control);
	remaining.wideChangeToRequested(registry, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI, typename... TR>
void
_OS<NS, NC, NO, TA, NI, TI, TR...>::wideGetNames(const LongIndex parent,
												 const ShortIndex depth,
												 StructureStateInfos& _stateInfos) const
{
	initial	 .deepGetNames(parent, StructureStateInfo::ORTHOGONAL, depth, _stateInfos);
	remaining.wideGetNames(parent,								   depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
