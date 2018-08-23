namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
_CS<NS, NC, NO, TA, NI, TI>::_CS(StateParents& stateParents,
								 const ShortIndex fork,
								 Parents& forkParents,
								 ForkPointers& forkPointers)
	: initial(stateParents,
			  Parent(fork, PRONG_INDEX),
			  forkParents,
			  forkPointers)
{}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardGuard(const ShortIndex HSFM_IF_ASSERT(prong),
											  FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardGuard(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideGuard(const ShortIndex HSFM_IF_ASSERT(prong),
									   FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnterInitial(PlanControl& control) {
	initial.deepEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideEnter(const ShortIndex HSFM_IF_ASSERT(prong),
									   PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepEnter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
Status
_CS<NS, NC, NO, TA, NI, TI>::wideUpdate(const ShortIndex HSFM_IF_ASSERT(prong),
										FullControl& control)
{
	assert(prong == PRONG_INDEX);

	return initial.deepUpdate(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
template <typename TEvent>
void
_CS<NS, NC, NO, TA, NI, TI>::wideReact(const ShortIndex HSFM_IF_ASSERT(prong),
									   const TEvent& event,
									   FullControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepReact(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideExit(const ShortIndex HSFM_IF_ASSERT(prong),
									  PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepExit(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideForwardRequest(const ShortIndex HSFM_IF_ASSERT(prong),
												const TransitionType transition)
{
	assert(prong == PRONG_INDEX);

	initial.deepForwardRequest(transition);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRemain() {
	initial.deepRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestRestart() {
	initial.deepRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideRequestResume(const ShortIndex HSFM_IF_ASSERT(prong)) {
	assert(prong == PRONG_INDEX);

	initial.deepRequestResume();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
void
_CS<NS, NC, NO, TA, NI, TI>::wideChangeToRequested(const ShortIndex HSFM_IF_ASSERT(prong),
												   PlanControl& control)
{
	assert(prong == PRONG_INDEX);

	initial.deepChangeToRequested(control);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, ForkID NC, ForkID NO, typename TA, ShortIndex NI, typename TI>
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
