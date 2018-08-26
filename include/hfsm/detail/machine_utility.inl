#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
bool
isActive(const RegistryT<TArgs>& registry,
		 const StateID stateId)
{
	using Args = TArgs;

	constexpr LongIndex STATE_COUNT = Args::STATE_COUNT;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParent(parent.fork))
		{
			const Fork& fork = registry.fork(parent.fork);

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.active;
		}

	return false;
}

//------------------------------------------------------------------------------

template <typename TArgs>
bool
isResumable(const RegistryT<TArgs>& registry,
			const StateID stateId)
{
	using Args = TArgs;

	constexpr LongIndex STATE_COUNT = Args::STATE_COUNT;
	assert(stateId < STATE_COUNT);

	if (stateId < STATE_COUNT)
		for (auto parent = registry.stateParents[stateId];
			 parent;
			 parent = registry.forkParent(parent.fork))
		{
			const Fork& fork = registry.fork(parent.fork);

			if (fork.active != STATE_COUNT)
				return parent.prong == fork.resumable;
		}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

}
}
