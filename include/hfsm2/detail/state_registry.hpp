#pragma once

namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

enum Strategy {
	Composite,
	Resumable,
	Utilitarian,
	RandomUtil,
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	HFSM_INLINE Parent() = default;

	HFSM_INLINE Parent(const ForkID forkId_)
		: forkId{forkId_}
	{}

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

template <typename TPayload>
struct RequestT {
	using Payload = TPayload;

	enum Type {
		REMAIN,
		CHANGE,
		RESTART,
		RESUME,
		UTILIZE,
		RANDOMIZE,
		SCHEDULE,

		COUNT
	};

	HFSM_INLINE RequestT() = default;

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	HFSM_INLINE RequestT(const Type type_,
						 const StateID stateId_,
						 const Payload& payload_)
		: type{type_}
		, stateId{stateId_}
		, payload{payload_}
	{
		HFSM_ASSERT(type_ < Type::COUNT);
	}

	Type type = CHANGE;
	StateID stateId = INVALID_STATE_ID;
	Payload payload;
};

template <typename TPayload, ShortIndex NCount>
using RequestsT = Array<RequestT<TPayload>, NCount>;

////////////////////////////////////////////////////////////////////////////////

template <LongIndex NCompoCount, LongIndex NOrthoCount, LongIndex NOrthoUnits>
struct AllForksT {
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using Compo = StaticArray<ShortIndex, COMPO_REGIONS>;
	using Ortho = BitArray	 <ShortIndex, ORTHO_UNITS>;

	Compo compo{INVALID_SHORT_INDEX};
	Ortho ortho;

	HFSM_INLINE void clear();
};

////////////////////////////////////////////////////////////////////////////////

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
struct StateRegistryT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							NOrthoCount,
							NOrthoUnits,
							TPayload,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;

	using Request		= RequestT<Payload>;

	static constexpr LongIndex  STATE_COUNT	  = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS = NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS	  = NOrthoUnits;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArray<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArray<Units,  ORTHO_UNITS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using AllForks		= AllForksT<COMPO_REGIONS, ORTHO_REGIONS, ORTHO_UNITS>;
	using OrthoBits		= typename AllForks::Ortho::Bits;

	using CompoRemains	= BitArray<ShortIndex, COMPO_REGIONS>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent&	  forkParent(const ForkID forkId) const;

	HFSM_INLINE OrthoBits resumableOrthoFork(const ForkID forkId);
	HFSM_INLINE OrthoBits requestedOrthoFork(const ForkID forkId);

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoActive{INVALID_SHORT_INDEX};
	AllForks resumable;

	AllForks requested;
	CompoRemains compoRemains;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  typename TPayload,
		  LongIndex NTaskCapacity>
struct StateRegistryT<ArgsT<TContext,
							TConfig,
							TStateList,
							TRegionList,
							NCompoCount,
							0,
							0,
							TPayload,
							NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;
	using Payload		= TPayload;
	using Request		= RequestT<Payload>;

	static constexpr LongIndex  STATE_COUNT = StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS = NCompoCount;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using AllForks		= AllForksT<COMPO_REGIONS, 0, 0>;
	using CompoRemains	= BitArray<ShortIndex, COMPO_REGIONS>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM_INLINE const Parent& forkParent(const ForkID forkId) const;

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoActive{INVALID_SHORT_INDEX};
	AllForks resumable;

	AllForks requested;
	CompoRemains compoRemains;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "state_registry.inl"
