namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, typename TH, typename... TS>
_C<TC, TPL, TH, TS...>::_C(StateRegistryBase& stateRegistry,
						   const Parent parent,
						   Parents& forkParents,
						   ForkPointers& forkPointers)
	: _fork(static_cast<ShortIndex>(forkPointers << &_fork),
			parent,
			forkParents)
	, _state(stateRegistry,
			 parent,
			 forkParents,
			 forkPointers)
	, _subStates(stateRegistry,
				 _fork.self,
				 TypeInfo::get<Head>(),
				 forkParents,
				 forkPointers)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRegister(StateRegistryBase& stateRegistry,
									 const Parent parent)
{
	_state	  .deepRegister(stateRegistry, parent);
	_subStates.wideRegister(stateRegistry,
							_fork.self,
							TypeInfo::get<Head>());
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepForwardGuard(TransitionControl& control) {
	assert(_fork.requested != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideForwardGuard(_fork.requested, control);
	else
		_subStates.wideGuard	   (_fork.requested, control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepGuard(TransitionControl& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	if (!_state	  .deepGuard(				  control))
		_subStates.wideGuard(_fork.requested, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepEnterInitial(Control& control) {
	assert(_fork.active    == INVALID_SHORT_INDEX &&
		   _fork.resumable == INVALID_SHORT_INDEX &&
		   _fork.requested == INVALID_SHORT_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.active = 0;

	_state	  .deepEnter	   (control);
	_subStates.wideEnterInitial(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepEnter(Control& control) {
	assert(_fork.active	   == INVALID_SHORT_INDEX &&
		   _fork.requested != INVALID_SHORT_INDEX);

	HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
	_fork.active = _fork.requested;

	HSFM_IF_DEBUG(_fork.requestedType.clear());
	_fork.requested = INVALID_SHORT_INDEX;

	_state	  .deepEnter(control);
	_subStates.wideEnter(_fork.active, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
bool
_C<TC, TPL, TH, TS...>::deepUpdate(TransitionControl& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_state	  .deepUpdate(control)) {
		ControlLock lock(control);
		_subStates.wideUpdate(_fork.active, control);

		return true;
	} else
		return _subStates.wideUpdate(_fork.active, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
template <typename TEvent>
void
_C<TC, TPL, TH, TS...>::deepReact(const TEvent& event,
								  TransitionControl& control)
{
	assert(_fork.active != INVALID_SHORT_INDEX);

	_state	  .deepReact(			   event, control);
	_subStates.wideReact(_fork.active, event, control);
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepExit(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	_subStates.wideExit(_fork.active, control);
	_state	  .deepExit(control);

	HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
	_fork.resumable = _fork.active;

	HSFM_IF_DEBUG(_fork.activeType.clear());
	_fork.active = INVALID_SHORT_INDEX;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepForwardRequest(const TransitionType transition) {
	if (_fork.requested != INVALID_SHORT_INDEX)
		_subStates.wideForwardRequest(_fork.requested, transition);
	else
		switch (transition) {
		case Transition::REMAIN:
			deepRequestRemain();
			break;

		case Transition::RESTART:
			deepRequestRestart();
			break;

		case Transition::RESUME:
			deepRequestResume();
			break;

		default:
			assert(false);
		}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestRemain() {
	if (_fork.active == INVALID_SHORT_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestRemain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestRestart() {
	HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
	_fork.requested = 0;

	_subStates.wideRequestRestart();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepRequestResume() {
	if (_fork.resumable != INVALID_SHORT_INDEX) {
		HSFM_IF_DEBUG(_fork.requestedType = _fork.resumableType);
		_fork.requested = _fork.resumable;
	} else {
		HSFM_IF_DEBUG(_fork.requestedType = TypeInfo::get<typename SubStates::Initial::Head>());
		_fork.requested = 0;
	}

	_subStates.wideRequestResume(_fork.requested);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepChangeToRequested(Control& control) {
	assert(_fork.active != INVALID_SHORT_INDEX);

	if (_fork.requested == _fork.active)
		_subStates.wideChangeToRequested(_fork.requested, control);
	else if (_fork.requested != INVALID_SHORT_INDEX) {
		_subStates.wideExit(_fork.active, control);

		HSFM_IF_DEBUG(_fork.resumableType = _fork.activeType);
		_fork.resumable = _fork.active;

		HSFM_IF_DEBUG(_fork.activeType = _fork.requestedType);
		_fork.active = _fork.requested;

		HSFM_IF_DEBUG(_fork.requestedType.clear());
		_fork.requested = INVALID_SHORT_INDEX;

		_subStates.wideEnter(_fork.active, control);
	}
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepGetNames(const LongIndex parent,
									 const enum StructureStateInfo::RegionType region,
									 const ShortIndex depth,
									 StructureStateInfos& _stateInfos) const
{
	_state	  .deepGetNames(parent, region,			 depth,		_stateInfos);
	_subStates.wideGetNames(_stateInfos.count() - 1, depth + 1, _stateInfos);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH, typename... TS>
void
_C<TC, TPL, TH, TS...>::deepIsActive(const bool isActive,
									 LongIndex& index,
									 MachineStructure& structure) const
{
	_state	  .deepIsActive(isActive,									   index, structure);
	_subStates.wideIsActive(isActive ? _fork.active : INVALID_SHORT_INDEX, index, structure);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
