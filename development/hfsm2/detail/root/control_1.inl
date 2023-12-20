namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Origin::Origin(ControlT& control_,
								const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ControlT<TArgs>::Region::Region(ControlT& control_,
								const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::setRegion(const RegionID regionId_) noexcept {
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ < RegionList::SIZE);

	_regionId = regionId_;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::resetRegion(const RegionID regionId_) noexcept {
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);

	_regionId = regionId_;
}

//------------------------------------------------------------------------------

#if HFSM2_TRANSITION_HISTORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
ControlT<TArgs>::pinLastTransition(const StateID stateId_,
								   const Short index) noexcept
{
	if (index != INVALID_SHORT) {
		HFSM2_ASSERT(index < TransitionSets::CAPACITY);

		if (!_core.registry.isActive(stateId_))
			_core.transitionTargets[stateId_] = index;
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransitionTo(const StateID stateId_) const noexcept {
	if (HFSM2_CHECKED(stateId_ < StateList::SIZE)) {
		const Short index = _core.transitionTargets[stateId_];

		if (index < _core.previousTransitions.count())
			return &_core.previousTransitions[index];
	}

	return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
const typename ControlT<TArgs>::Transition*
ControlT<TArgs>::lastTransition() const noexcept {
	HFSM2_ASSERT(_originId < _core.transitionTargets.CAPACITY);

	return lastTransitionTo(_originId);
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
