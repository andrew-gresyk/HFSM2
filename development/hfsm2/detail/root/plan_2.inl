#if HFSM2_PLANS_AVAILABLE()

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TG_, typename TSL_, typename TRL_, Long NCC_, Long NOC_, Long NOU_, typename TRO_ HFSM2_IF_SERIALIZATION(, Long NSB_), Long NTC_, typename TTP_>
HFSM2_CONSTEXPR(14)
bool
PayloadPlanT<ArgsT<TG_, TSL_, TRL_, NCC_, NOC_, NOU_, TRO_ HFSM2_IF_SERIALIZATION(, NSB_), NTC_, TTP_>>:: append(const StateID origin,
																												 const StateID destination,
																												 const TransitionType transitionType,
																												 const Payload& payload) noexcept
{
	if (_planData.tasks.count() < TASK_CAPACITY) {
		_planData.planExists.set(_regionId);

		return linkTask(_planData.tasks.emplace(origin, destination, transitionType, payload));
	} else
		return false;
}

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
