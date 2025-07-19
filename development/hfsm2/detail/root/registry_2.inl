namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
Prong
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::activeSubState(const StateID stateId) const noexcept {
	const StateID subStateId = stateId + 1;

	if (HFSM2_CHECKED(	 stateId < STATE_COUNT) &&
		HFSM2_CHECKED(subStateId < STATE_COUNT))
	{
		if (const Parent parent = stateParents[subStateId]) {
			HFSM2_ASSERT(parent.forkId != 0);

			if (parent.forkId > 0)
				return compoActive[parent.forkId - 1];
		}
	}

	return INVALID_PRONG;
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isActive() const noexcept {
	return compoActive[ROOT_ID] != INVALID_PRONG;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isActive(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive[parent.forkId - 1];
		} else
			return isActive();
	}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isResumable(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoResumable[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isPendingChange(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return compoRequested[parent.forkId - 1] !=
				   compoActive	 [parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isPendingEnter(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong != compoActive	 [parent.forkId - 1] &&
				   parent.prong == compoRequested[parent.forkId - 1];
		}

	return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::isPendingExit(const StateID stateId) const noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT))
		if (Parent parent = stateParents[stateId]) {
			HFSM2_ASSERT(parent.forkId > 0);

			return parent.prong == compoActive	 [parent.forkId - 1] &&
				   parent.prong != compoRequested[parent.forkId - 1];
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
const Parent&
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::forkParent(const ForkID forkId) const noexcept {
	HFSM2_ASSERT(forkId > 0);

	return compoParents[forkId - 1];
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::requestImmediate(const Transition& request) noexcept {
	// record request
	// promote it to all children

	if (request.destination == 0)
		return false;
	else if (HFSM2_CHECKED(request.destination < STATE_COUNT)) {
		Parent parent = stateParents[request.destination];

		if (HFSM2_CHECKED(parent)) {
			HFSM2_ASSERT(parent.forkId > 0);

			compoRequested[parent.forkId - 1] = parent.prong;

			for (parent = forkParent(parent.forkId);
				 parent;
				 parent = forkParent(parent.forkId))
			{
				HFSM2_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);

				if (compoActive	  [parent.forkId - 1] != parent.prong)
					compoRequested[parent.forkId - 1]  = parent.prong;
				else {
					parent = forkParent(parent.forkId);
					break;
				}
			}

			for (; parent; parent = forkParent(parent.forkId)) {
				HFSM2_ASSERT(parent.forkId > 0);
				compoRemains.set(parent.forkId - 1);
			}
		}
	}

	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::requestScheduled(const StateID stateId) noexcept {
	if (HFSM2_CHECKED(stateId < STATE_COUNT)) {
		const Parent parent = stateParents[stateId];

		HFSM2_ASSERT(parent.forkId > 0);
		compoResumable[parent.forkId - 1] = parent.prong;
	}
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::clearRequests() noexcept {
	compoRequested.clear();
	orthoRequested.clear();
	compoRemains  .clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(14)
void
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::clear() noexcept {
	clearRequests();

	compoActive	  .clear();
	compoResumable.clear();
}

//------------------------------------------------------------------------------

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_) HFSM2_IF_PLANS(, Long NTC_), typename TTP_>
HFSM2_CONSTEXPR(11)
bool
RegistryT<ArgsT<TG_, TSL_, TRL_, NCC_, 0, 0, TRO_ HFSM2_IF_SERIALIZATION(, NSB_) HFSM2_IF_PLANS(, NTC_), TTP_>>::empty() const noexcept {
	return compoRequested.empty()
		&& orthoRequested.empty()
		&& compoRemains	 .empty()
		&& compoActive	 .empty()
		&& compoResumable.empty();
}

////////////////////////////////////////////////////////////////////////////////

}
}
