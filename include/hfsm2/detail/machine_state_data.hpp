#pragma once

namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	HFSM_INLINE Parent() = default;

	HFSM_INLINE Parent(const ForkID forkId_,
					   const ShortIndex prong_)
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM_INLINE explicit operator bool() const {
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

	HFSM_INLINE RequestT() = default;

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	template <typename T,
			  typename = typename std::enable_if<contains<T>(), T>::type>
	HFSM_INLINE RequestT(const Type type_,
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

using OrthoFork = BitArrayT<ShortIndex>;

template <ShortIndex NProngCount>
using OrthoForkT = BitArrayStorageT<ShortIndex, NProngCount>;

template <ShortIndex NCapacity, LongIndex NStorageSize>
using OrthoForksT = ObjectPool<OrthoFork, NCapacity, NStorageSize>;

//------------------------------------------------------------------------------

template <LongIndex NCompoCount, LongIndex NOrthoCount, LongIndex NOrthoUnits>
struct AllForksT {
	static constexpr ShortIndex COMPO_COUNT	  = NCompoCount;
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using Compo = StaticArray<ShortIndex,  COMPO_COUNT>;
	using Ortho = OrthoForksT<ORTHO_COUNT, ORTHO_STORAGE>;

	Compo compo{INVALID_SHORT_INDEX};
	Ortho ortho;

	HFSM_INLINE void clear();
};

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

	using CompoForks	= StaticArray<ShortIndex,  COMPO_COUNT>;
	using AllForks		= AllForksT<COMPO_COUNT, ORTHO_COUNT, ORTHO_UNITS>;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;
	HFSM_INLINE ForkID parentCompoFork(const ForkID forkId) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearOrthoRequested();

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;

	CompoForks compoActive	 {INVALID_SHORT_INDEX};

	AllForks resumable;
	AllForks requested;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  typename TPayloadList,
		  LongIndex NTaskCapacity>
struct StateDataT<ArgsT<TContext,
						TConfig,
						TStateList,
						TRegionList,
						NCompoCount,
						0,
						0,
						TPayloadList,
						NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using PayloadList	= TPayloadList;

	using Request		= RequestT<PayloadList>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_COUNT = NCompoCount;

	using StateParents	= ParentsT<STATE_COUNT>;
	using CompoParents	= StaticArray<Parent,	  COMPO_COUNT>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_COUNT>;

	using AllForks		= AllForksT<COMPO_COUNT, 0, 0>;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;

	bool isActive	(const StateID stateId) const;
	bool isResumable(const StateID stateId) const;

	void requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	HFSM_INLINE void clearOrthoRequested()										{}

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoActive{INVALID_SHORT_INDEX};

	AllForks resumable;
	AllForks requested;
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
struct StateDataT<ArgsT<TContext,
						TConfig,
						TStateList,
						TRegionList,
						0,
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
	static constexpr ShortIndex ORTHO_COUNT	  = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;
	static constexpr LongIndex  ORTHO_STORAGE = ORTHO_COUNT * sizeof(OrthoFork) + ORTHO_UNITS;

	using StateParents	= ParentsT<STATE_COUNT>;
	using OrthoParents	= StaticArray<Parent,	   ORTHO_COUNT>;

	using AllForks		= AllForksT<0, ORTHO_COUNT, ORTHO_UNITS>;

	HFSM_INLINE bool isActive   (const StateID) const	{ return true;			}
	HFSM_INLINE bool isResumable(const StateID) const	{ return false;			}

	HFSM_INLINE void requestImmediate(const Request)	{ HFSM_BREAK();			}
	HFSM_INLINE void requestScheduled(const Request)	{ HFSM_BREAK();			}

	HFSM_INLINE void clearOrthoRequested()										{}

	StateParents stateParents;
	OrthoParents orthoParents;

	AllForks resumable;
	AllForks requested;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_state_data.inl"
