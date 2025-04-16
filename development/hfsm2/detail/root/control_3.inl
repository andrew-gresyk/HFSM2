namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
FullControlBaseT<TArgs>::Lock::Lock(FullControlBaseT& control_) noexcept
	: control{!control_._locked ? &control_ : nullptr}
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
FullControlBaseT<TArgs>::Lock::~Lock() noexcept {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::changeTo(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::restart(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::resume(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::select(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

//------------------------------------------------------------------------------

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::utilize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::randomize(const StateID stateId_) noexcept {
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::schedule(const StateID stateId_) noexcept {
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

//------------------------------------------------------------------------------

#if HFSM2_PLANS_AVAILABLE()

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::succeed(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::SUCCESS;

	_core.planData.tasksSuccesses.set(stateId_);

	HFSM2_LOG_TASK_STATUS(context(), _regionStateId, stateId_, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
FullControlBaseT<TArgs>::fail(const StateID stateId_) noexcept {
	_taskStatus.result = TaskStatus::FAILURE;

	_core.planData.tasksFailures.set(stateId_);

	HFSM2_LOG_TASK_STATUS(context(), _regionStateId, stateId_, StatusEvent::FAILED);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
template <typename TState>
HFSM2_CONSTEXPR(14)
TaskStatus
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>::updatePlan(TState& headState,
																													const TaskStatus subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID; // SPECIFIC

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		HFSM2_LOG_PLAN_STATUS(context(), _regionStateId, StatusEvent::FAILED);

		headState.wrapPlanFailed(*this);

		return TaskStatus{_taskStatus.result};
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, STATE_ID}; // SPECIFIC

					if (const Payload* const payload = it->payload())
						changeWith(it->destination, *it->payload());
					else
						changeTo  (it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;

			return TaskStatus{};
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			HFSM2_LOG_PLAN_STATUS(context(), _regionStateId, StatusEvent::SUCCEEDED);

			plan().clearTasks();
			headState.wrapPlanSucceeded(*this);

			return TaskStatus{_taskStatus.result};
		}
	} else
		return TaskStatus{};
}

#endif

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::changeWith(const StateID  stateId_,
																																	 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::CHANGE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::CHANGE, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::restartWith(const StateID  stateId_,
																																	  const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESTART, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESTART, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::resumeWith(const StateID  stateId_,
																																	 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RESUME, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RESUME, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::selectWith(const StateID  stateId_,
																																	 const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SELECT, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SELECT, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#if HFSM2_UTILITY_THEORY_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::utilizeWith(const StateID  stateId_,
																																	  const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::UTILIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::UTILIZE, stateId_);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::randomizeWith(const StateID  stateId_,
																																		const Payload& payload) noexcept
{
	if (!_locked) {
		_core.requests.emplace(Transition{_originId, stateId_, TransitionType::RANDOMIZE, payload});

		if (stateId_ < _regionStateId || _regionStateId + _regionSize <= stateId_)
			_taskStatus.outerTransition = true;

		HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::RANDOMIZE, stateId_);
	}
}

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::scheduleWith(const StateID  stateId_,
																																	   const Payload& payload) noexcept
{
	_core.requests.emplace(Transition{_originId, stateId_, TransitionType::SCHEDULE, payload});

	HFSM2_LOG_TRANSITION(context(), _originId, TransitionType::SCHEDULE, stateId_);
}

////////////////////////////////////////////////////////////////////////////////

#if HFSM2_PLANS_AVAILABLE()

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_>
template <typename TState>
HFSM2_CONSTEXPR(14)
TaskStatus
FullControlT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, void>>::updatePlan(TState& headState,
																													const TaskStatus subStatus) noexcept
{
	constexpr StateID STATE_ID = TState::STATE_ID; // SPECIFIC

	HFSM2_ASSERT(subStatus);

	if (subStatus.result == TaskStatus::FAILURE) {
		_taskStatus.result = TaskStatus::FAILURE;
		HFSM2_LOG_PLAN_STATUS(context(), _regionStateId, StatusEvent::FAILED);

		headState.wrapPlanFailed(*this);

		return TaskStatus{_taskStatus.result};
	} else if (subStatus.result == TaskStatus::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			TasksBits successesToClear;
			successesToClear.set();

			for (auto it = p.begin();
				 it && isActive(it->origin);
				 ++it)
			{
				if (_core.planData.tasksSuccesses.get(it->origin)) {
					Origin origin{*this, STATE_ID}; // SPECIFIC

					changeTo(it->destination);

					if (it->cyclic())
						_core.planData.tasksSuccesses.clear(it->origin); // SPECIFIC
					else
						successesToClear.clear(it->origin);

					it.remove();
				}
			}

			_core.planData.tasksSuccesses &= successesToClear;

			return TaskStatus{};
		} else {
			_taskStatus.result = TaskStatus::SUCCESS;
			HFSM2_LOG_PLAN_STATUS(context(), _regionStateId, StatusEvent::SUCCEEDED);

			plan().clearTasks();
			headState.wrapPlanSucceeded(*this);

			return TaskStatus{_taskStatus.result};
		}
	} else
		return TaskStatus{};
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
