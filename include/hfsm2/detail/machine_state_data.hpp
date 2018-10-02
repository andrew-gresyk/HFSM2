#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	inline Parent() = default;

	inline Parent(const ForkID forkId_,
				  const ShortIndex prong_)
		: forkId{forkId_}
		, prong{prong_}
	{}

	inline explicit operator bool() const {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT_INDEX;
	}

	ForkID	   forkId = INVALID_FORK_ID;
	ShortIndex prong  = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TPayloadList>
struct RequestT {
	using PayloadList = TPayloadList;
	using PayloadBox = typename PayloadList::Variant;

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
		HFSM_ASSERT(type_ < Type::COUNT);
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
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	Type type = RESTART;
	StateID stateId = INVALID_STATE_ID;
	PayloadBox payload;
};

template <typename TPayloadList, ShortIndex NCount>
using RequestsT = Array<RequestT<TPayloadList>, NCount>;

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
	using Prongs = BitArrayT<ShortIndex>;

	HSFM_IF_DEBUG(std::type_index TYPE = typeid(CompoFork));
	Prongs prongs;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <ShortIndex NProngCount>
struct HSFM_UNLESS_DEBUG(alignas(4 * sizeof(ShortIndex))) OrthoForkT {
	static constexpr ShortIndex	PRONG_COUNT = NProngCount;

	using Prongs = BitArrayStorageT<ShortIndex, PRONG_COUNT>;

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
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  typename,
		  LongIndex>
struct ArgsT;

template <typename>
struct StateDataT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateDataT<ArgsT<TContext,
						TConfig,
						TStateList,
						TRegionList,
						NCompoCount,
						NOrthoCount,
						NOrthoUnits,
						TPayloadList,
						NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
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

	inline const Parent& forkParent(const ForkID forkId) const;
	inline const CompoFork* compoParent(const ForkID forkId) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const Request request);

	void clearOrthoRequested();

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
		  typename TRegionList,
		  LongIndex NCompoCount,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateDataT<ArgsT<TContext, TConfig, TStateList, TRegionList, NCompoCount, 0, 0, TPayloadList, NTaskCapacity>> {
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT = NCompoCount;

	using StateParents	= ParentsT<STATE_COUNT>;
	using CompoParents	= StaticArray<Parent,	 COMPO_COUNT>;
	using CompoForks	= StaticArray<CompoFork, COMPO_COUNT>;

	inline const Parent& forkParent(const ForkID forkId) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const Request request);

	inline void clearOrthoRequested()					{}

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoForks;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateDataT<ArgsT<TContext, TConfig, TStateList, TRegionList, 0, NOrthoCount, NOrthoUnits, TPayloadList, NTaskCapacity>> {
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using StateParents	= ParentsT<STATE_COUNT>;
	using OrthoParents	= StaticArray<Parent,	   ORTHO_COUNT>;
	using OrthoForks	= OrthoForksT<ORTHO_COUNT, ORTHO_STORAGE>;

	inline const Parent& forkParent(const ForkID forkId) const;

	inline bool isActive   (const StateID) const		{ return true;			}
	inline bool isResumable(const StateID) const		{ return false;			}

	inline void requestImmediate(const Request)			{ HFSM_ASSERT(false);		}
	inline void requestScheduled(const Request)			{ HFSM_ASSERT(false);		}

	inline void clearOrthoRequested()					{}

	StateParents stateParents;
	OrthoParents orthoParents;

	OrthoForks orthoRequested;
	OrthoForks orthoResumable;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_state_data.inl"
