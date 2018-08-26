#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	inline Parent() = default;

	inline Parent(const ForkID fork_,
				  const ShortIndex prong_)
		: fork(fork_)
		, prong(prong_)
	{}

	inline explicit operator bool() const {
		return fork  != INVALID_FORK_ID &&
			   prong != INVALID_SHORT_INDEX;
	}

	ForkID	   fork  = INVALID_FORK_ID;
	ShortIndex prong = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

using Parents = ArrayView<Parent>;

////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#pragma pack(push, 1)
#endif

struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) Fork {
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;
	HSFM_IF_DEBUG(std::type_index TYPE = typeid(Fork));
};

#ifndef _DEBUG
#pragma pack(pop)
#endif

using ForkPointers = ArrayView<Fork*>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NStateCount>
using ParentsT = Array<Parent, NStateCount>;

template <LongIndex NForkCount>
using ForkPointersT = Array<Fork*, NForkCount>;

//------------------------------------------------------------------------------

template <typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct RegistryT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NCompositeCount,
		  LongIndex NOrthogonalCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, NCompositeCount, NOrthogonalCount, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;

	static constexpr LongIndex STATE_COUNT		= StateList::SIZE;
	static constexpr LongIndex COMPOSITE_COUNT	= NCompositeCount;
	static constexpr LongIndex ORTHOGONAL_COUNT	= NOrthogonalCount;

	using StateParents	= ParentsT<STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPOSITE_COUNT>;
	using OrthoParents	= StaticArray<Parent, ORTHOGONAL_COUNT>;

	using CompoForks	= StaticArray<Fork, COMPOSITE_COUNT>;
	using OrthoForks	= StaticArray<Fork, ORTHOGONAL_COUNT>;

	inline Parent& forkParent(const ForkID fork) {
		assert(fork != 0);

		return fork > 0 ?
			compoParents[ fork - 1] :
			orthoParents[-fork - 1];
	}

	inline const Parent& forkParent(const ForkID fork) const {
		assert(fork != 0);

		return fork > 0 ?
			compoParents[ fork - 1] :
			orthoParents[-fork - 1];
	}

	inline Fork& fork(const ForkID fork) {
		assert(fork != 0);

		return fork > 0 ?
			compoForks[ fork - 1] :
			orthoForks[-fork - 1];
	}

	inline const Fork& fork(const ForkID fork) const {
		assert(fork != 0);

		return fork > 0 ?
			compoForks[ fork - 1] :
			orthoForks[-fork - 1];
	}

	StateParents stateParents;

	CompoParents compoParents;
	OrthoParents orthoParents;

	CompoForks compoForks;
	OrthoForks orthoForks;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NCompositeCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, NCompositeCount, 0, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;

	static constexpr LongIndex STATE_COUNT		= StateList::SIZE;
	static constexpr LongIndex COMPOSITE_COUNT	= NCompositeCount;

	using StateParents	= ParentsT<STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPOSITE_COUNT>;
	using CompoForks	= StaticArray<Fork, COMPOSITE_COUNT>;

	inline Parent& forkParent(const ForkID fork) {
		assert(fork > 0);

		return compoParents[fork - 1];
	}

	inline const Parent& forkParent(const ForkID fork) const {
		assert(fork > 0);

		return compoParents[fork - 1];
	}

	inline Fork& fork(const ForkID fork) {
		assert(fork > 0);

		return compoForks[ fork - 1];
	}

	inline const Fork& fork(const ForkID fork) const {
		assert(fork > 0);

		return compoForks[ fork - 1];
	}

	StateParents stateParents;
	CompoParents compoParents;
	CompoForks compoForks;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NOrthogonalCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, 0, NOrthogonalCount, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;

	static constexpr LongIndex STATE_COUNT		= StateList::SIZE;
	static constexpr LongIndex ORTHOGONAL_COUNT	= NOrthogonalCount;

	using StateParents	= ParentsT<STATE_COUNT>;
	using OrthoParents	= StaticArray<Parent, ORTHOGONAL_COUNT>;
	using OrthoForks	= StaticArray<Fork, ORTHOGONAL_COUNT>;

	inline Parent& forkParent(const ForkID fork) {
		assert(fork < 0);

		return orthoParents[-fork - 1];
	}

	inline const Parent& forkParent(const ForkID fork) const {
		assert(fork < 0);

		return orthoParents[-fork - 1];
	}

	inline Fork& fork(const ForkID fork) {
		assert(fork < 0);

		return orthoForks[-fork - 1];
	}

	inline const Fork& fork(const ForkID fork) const {
		assert(fork < 0);

		return orthoForks[-fork - 1];
	}

	StateParents stateParents;
	OrthoParents orthoParents;
	OrthoForks orthoForks;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
bool isActive(const RegistryT<TArgs>& registry,
			  const StateID stateId);

template <typename TArgs>
bool isResumable(const RegistryT<TArgs>& registry,
				 const StateID stateId);

template <typename TArgs>
inline
bool isScheduled(const RegistryT<TArgs>& registry,
				 const StateID stateId)
{
	return isResumable(registry, stateId);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_utility.inl"
