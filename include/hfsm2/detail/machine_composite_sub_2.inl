namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
_CS<NS, NC, NO, TA, NI, TI>::_CS(StateData& stateData,
								 const ForkID forkId)
	: initial{stateData, Parent{forkId, PRONG_INDEX}}
{}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
											  FullControl& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   FullControl& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(Control& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
									   Control& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
Status
_CS<NS, NC, NO, TA, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
										FullControl& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
									   const TEvent& event,
									   FullControl& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
									  Control& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(StateData& stateData,
												const ShortIndex HSFM_IF_ASSERT(prong),
												const RequestType transition)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepForwardRequest(stateData, transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRemain(StateData& stateData) {
	initial.deepRequestRemain(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRestart(StateData& stateData) {
	initial.deepRequestRestart(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestResume(StateData& stateData,
											   const ShortIndex HSFM_IF_ASSERT(prong))
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepRequestResume(stateData);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(StateData& stateData,
												   const ShortIndex HSFM_IF_ASSERT(prong),
												   Control& control)
{
	HFSM_ASSERT(prong == PRONG_INDEX);

	initial.deepChangeToRequested(stateData, control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideGetNames(const LongIndex parent,
										  const ShortIndex depth,
										  StructureStateInfos& _stateInfos) const
{
	initial.deepGetNames(parent, StructureStateInfo::COMPOSITE, depth, _stateInfos);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
