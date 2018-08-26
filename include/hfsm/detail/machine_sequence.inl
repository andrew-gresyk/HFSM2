namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepGuard(FullControl& control) {
	Composite::deepGuard(control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
Status
_Q<NS, NC, NO, TA, TH, TS...>::deepUpdate(FullControl& control) {
	Fork& fork = Composite::compoFork(control);
	
	assert(fork.active != INVALID_SHORT_INDEX);

	ControlRegion region{control, HEAD_ID, SubStateList::SIZE};

	if (const Status stateStatus = _headState.deepUpdate(control)) {
		ControlLock lock{control};
		_subStates.wideUpdate(fork.active, control);

		return stateStatus;
	} else {
		const Status status = _subStates.wideUpdate(fork.active, control);

		if (status.failure || status.outerTransition)
			return status;
		else if (status.success)
			_success = true;

		if (_success) {
			ControlOrigin origin{control, HEAD_ID};

			Plan plan = control.plan(HEAD_ID);

			for (auto it = plan.begin(); it; ++it)
				if (control.isActive(it->origin)) {
					control.changeTo(it->destination);
					it.remove();

					_success = false;
				}
		}

		return {_success,
				status.failure,
				status.innerTransition,
				status.outerTransition};
	}
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
template <typename TEvent>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepReact(const TEvent& event,
										 FullControl& control)
{
	Composite::template deepReact<TEvent>(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, ShortIndex NC, ShortIndex NO, typename TA, typename TH, typename... TS>
void
_Q<NS, NC, NO, TA, TH, TS...>::deepExit(PlanControl& control) {
	Composite::deepExit(control);

	auto plan = control.plan(HEAD_ID);
	plan.clear();

	_success = false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
