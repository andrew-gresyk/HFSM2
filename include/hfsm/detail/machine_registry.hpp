#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	inline Parent() = default;

	inline Parent(const ForkID fork_,
				  const ShortIndex prong_)
		: fork{fork_}
		, prong{prong_}
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

template <typename TPayloadList>
struct RequestT {
	using PayloadList = TPayloadList;
	using Payload = typename PayloadList::Container;

	enum Type {
		REMAIN,
		RESTART,
		RESUME,
		SCHEDULE,

		COUNT
	};

	template <typename T>
	static constexpr bool contains() { return PayloadList::template contains<T>();	}

	inline RequestT() = default;

	inline RequestT(const Type type_,
					   const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		assert(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	inline RequestT(const Type type_,
					const StateID stateId_,
					T* const payload_)
		: type{type_}
		, stateId{stateId_}
		, payload{payload_}
	{
		assert(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

// TODO: replace with Array
template <typename TPayloadList>
using RequestQueueT = ArrayView<RequestT<TPayloadList>>;

////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#pragma pack(push, 1)
#endif

struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) CompoFork {
	HSFM_IF_DEBUG(std::type_index TYPE = typeid(CompoFork));
	ShortIndex active	 = INVALID_SHORT_INDEX;
	ShortIndex resumable = INVALID_SHORT_INDEX;
	ShortIndex requested = INVALID_SHORT_INDEX;
};

//------------------------------------------------------------------------------

struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) OrthoFork {
	HSFM_IF_DEBUG(std::type_index TYPE = typeid(CompoFork));
	BitArray prongs;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <ShortIndex NProngCount>
struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) OrthoForkT {
	static constexpr ShortIndex	PRONG_COUNT = NProngCount;

	using Prongs = BitArrayT<PRONG_COUNT>;

	HSFM_IF_DEBUG(std::type_index TYPE = typeid(CompoFork));
	Prongs prongs;
};

#ifndef _DEBUG
#pragma pack(pop)
#endif

template <ShortIndex NCapacity, LongIndex NStorageSize>
using OrthoForksT = ObjectPool<OrthoFork, NCapacity, NStorageSize>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCount>
using ParentsT = Array<Parent, NCount>;

//------------------------------------------------------------------------------

template <typename,
		  typename,
		  typename,
		  LongIndex,
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
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, NCompoCount, NOrthoCount, NOrthoUnits, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using StateParents	= ParentsT<STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPO_COUNT>;
	using OrthoParents	= StaticArray<Parent, ORTHO_COUNT>;

	using CompoForks	= StaticArray<CompoFork,   COMPO_COUNT>;
	using OrthoForks	= OrthoForksT<ORTHO_COUNT, ORTHO_STORAGE>;

	inline const Parent& forkParent(const ForkID fork) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const Request request);

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;

	CompoForks compoForks;
	OrthoForks orthoRequested;
	OrthoForks orthoResumable;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NCompoCount,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, NCompoCount, 0, 0, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT = NCompoCount;

	using StateParents	= ParentsT<STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPO_COUNT>;
	using CompoForks	= StaticArray<CompoFork,	  COMPO_COUNT>;

	inline const Parent& forkParent(const ForkID fork) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const Request request);

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoForks;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NPlanCapacity>
struct RegistryT<ArgsT<TContext, TConfig, TStateList, 0, NOrthoCount, NOrthoUnits, TPayloadList, NPlanCapacity>> {
	using StateList		= TStateList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using StateParents	= ParentsT<STATE_COUNT>;
	using OrthoParents	= StaticArray<Parent,	   ORTHO_COUNT>;
	using OrthoForks	= OrthoForksT<ORTHO_COUNT, ORTHO_STORAGE>;

	inline const Parent& forkParent(const ForkID fork) const;

	inline bool isActive   (const StateID) const		{ return true;			}
	inline bool isResumable(const StateID) const		{ return false;			}

	inline void requestImmediate(const Request)			{ assert(false);		}
	inline void requestScheduled(const Request)			{ assert(false);		}

	StateParents stateParents;
	OrthoParents orthoParents;

	OrthoForks orthoRequested;
	OrthoForks orthoResumable;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_registry.inl"
