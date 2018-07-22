namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TPL, LongIndex NC>
StateInfoT<TPL>&
StateRegistryT<TPL, NC>::operator[] (const TypeInfo type) {
	auto* const result = _table.find(*type);
	HSFM_CHECKED(result);

	return *result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TPL, LongIndex NC>
const StateInfoT<TPL>&
StateRegistryT<TPL, NC>::operator[] (const TypeInfo type) const {
	const auto* const result = _table.find(*type);
	HSFM_CHECKED(result);

	return *result;
}

//------------------------------------------------------------------------------

template <typename TPL, LongIndex NC>
void
StateRegistryT<TPL, NC>::insert(const TypeInfo type) {
	HSFM_CHECKED(_table.insert(*type));
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::changeTo(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::RESTART, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESTART, state);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::resume(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::RESUME, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::RESUME, state);
	#endif
	}
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
void
TransitionControlT<TC, TPL>::schedule(const std::type_index state) {
	if (!_locked) {
		const Transition transition{TransitionType::SCHEDULE, state};
		_requests << transition;

	#ifdef HFSM_ENABLE_LOG_INTERFACE
		if (Base::_logger)
			Base::_logger->recordTransition(::hfsm::Transition::SCHEDULE, state);
	#endif
	}
}

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL>
ControlLockT<TC, TPL>::ControlLockT(TransitionControl& control)
	: _control(!control._locked ? &control : nullptr)
{
	if (_control)
		_control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL>
ControlLockT<TC, TPL>::~ControlLockT() {
	if (_control)
		_control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
