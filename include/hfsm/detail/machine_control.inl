namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESTART, stateId);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::resume(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESUME, stateId);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
void
TransitionControlT<TC, TSL, TPL>::schedule(const StateID stateId) {
	if (!_locked) {
		const Transition transition{TransitionType::SCHEDULE, stateId};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::SCHEDULE, stateId);
	#endif
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TSL, typename TPL>
ControlLockT<TC, TSL, TPL>::ControlLockT(TransitionControl& control)
	: _control(!control._locked ? &control : nullptr)
{
	if (_control)
		_control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TSL, typename TPL>
ControlLockT<TC, TSL, TPL>::~ControlLockT() {
	if (_control)
		_control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
