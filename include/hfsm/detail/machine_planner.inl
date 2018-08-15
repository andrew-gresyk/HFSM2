namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TA, typename TH>
bool
_P<TID, TA, TH>::deepGuard(FullControl& control) {
	const auto requestCountBefore = control._requests.count();

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		State::template log<decltype(&Head::guard), LoggerInterface::Method::GUARD>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	State::_head.widePreGuard(control.context());
	State::_head.guard(control);

	return requestCountBefore < control._requests.count();
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
void
_P<TID, TA, TH>::deepEnter(PlanControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		State::template log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	State::_head.widePreEnter(control.context());
	State::_head.enter(control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
Status
_P<TID, TA, TH>::deepUpdate(FullControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		State::template log<decltype(&Head::update), LoggerInterface::Method::UPDATE>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	State::_head.widePreUpdate(control.context());
	State::_head.update(control);
	// TODO: _head.widePostUpdate(control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
void
_P<TID, TA, TH>::deepExit(PlanControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		State::template log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	State::_head.exit(control);
	State::_head.widePostExit(control.context());
}

////////////////////////////////////////////////////////////////////////////////

}
}
