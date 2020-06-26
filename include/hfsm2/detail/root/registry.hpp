namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

enum Strategy {
	Composite,
	Resumable,

//#ifdef HFSM2_ENABLE_UTILITY_THEORY
	Utilitarian,
	RandomUtil,
//#endif
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(ShortIndex)) Parent {
	HFSM2_INLINE Parent() = default;

	HFSM2_INLINE Parent(const ForkID forkId_)
		: forkId{forkId_}
	{}

	HFSM2_INLINE Parent(const ForkID forkId_,
						const ShortIndex prong_)
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_INLINE explicit operator bool() const {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT_INDEX;
	}

	ForkID	   forkId = INVALID_FORK_ID;
	ShortIndex prong  = INVALID_SHORT_INDEX;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

struct Request {
	enum Type {
		REMAIN,
		CHANGE,
		RESTART,
		RESUME,

	//#ifdef HFSM2_ENABLE_UTILITY_THEORY
		UTILIZE,
		RANDOMIZE,
	//#endif

		SCHEDULE,

		COUNT
	};

	HFSM2_INLINE Request() = default;

	HFSM2_INLINE Request(const Type type_,
						 const StateID stateId_)
		: type{type_}
		, stateId{stateId_}
	{
		HFSM2_ASSERT(type_ < Type::COUNT);
	}

	Type type = CHANGE;
	StateID stateId = INVALID_STATE_ID;
};

template <ShortIndex NCount>
using RequestsT = Array<Request, NCount>;

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
struct BackUpT {
	using CompoForks = typename TRegistry::CompoForks;
	using OrthoForks = typename TRegistry::OrthoForks;

	CompoForks compoRequested;
	OrthoForks orthoRequested;
	CompoForks compoResumable;
};

////////////////////////////////////////////////////////////////////////////////

template <typename,
		  typename,
		  typename,
		  typename,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  LongIndex,
		  LongIndex>
struct ArgsT;

template <typename>
struct RegistryT;

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NOrthoCount,
		  LongIndex NOrthoUnits,
		  LongIndex NSerialBits,
		  LongIndex NTaskCapacity>
struct RegistryT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   NOrthoCount,
					   NOrthoUnits,
					   NSerialBits,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS	= NCompoCount;
	static constexpr ShortIndex ORTHO_REGIONS	= NOrthoCount;
	static constexpr ShortIndex ORTHO_UNITS		= NOrthoUnits;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;

	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArray<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArray<Units,  ORTHO_UNITS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using OrthoForks	= BitArray	 <ShortIndex, ORTHO_UNITS>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArray	 <ShortIndex, COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM2_INLINE const Parent&	   forkParent(const ForkID forkId) const;

	HFSM2_INLINE OrthoBits requestedOrthoFork(const ForkID forkId);

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();
	void reset();

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoRequested{INVALID_SHORT_INDEX};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT_INDEX};
	CompoForks compoResumable{INVALID_SHORT_INDEX};

	CompoRemains compoRemains;
};

//------------------------------------------------------------------------------

template <typename TContext,
		  typename TConfig,
		  typename TStateList,
		  typename TRegionList,
		  LongIndex NCompoCount,
		  LongIndex NSerialBits,
		  LongIndex NTaskCapacity>
struct RegistryT<ArgsT<TContext,
					   TConfig,
					   TStateList,
					   TRegionList,
					   NCompoCount,
					   0,
					   0,
					   NSerialBits,
					   NTaskCapacity>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr LongIndex  STATE_COUNT		= StateList::SIZE;
	static constexpr ShortIndex COMPO_REGIONS	= NCompoCount;

	using StateParents	= StaticArray<Parent, STATE_COUNT>;
	using CompoParents	= StaticArray<Parent, COMPO_REGIONS>;

	using CompoForks	= StaticArray<ShortIndex, COMPO_REGIONS>;
	using OrthoForks	= BitArray	 <ShortIndex, 0>;
	using CompoRemains	= BitArray	 <ShortIndex, COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		(const StateID stateId) const;
	bool isResumable	(const StateID stateId) const;

	bool isPendingChange(const StateID stateId) const;
	bool isPendingEnter	(const StateID stateId) const;
	bool isPendingExit	(const StateID stateId) const;

	HFSM2_INLINE const Parent& forkParent(const ForkID forkId) const;

	bool requestImmediate(const Request request);
	void requestScheduled(const StateID stateId);

	void clearRequests();
	void reset();

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoRequested{INVALID_SHORT_INDEX};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT_INDEX};
	CompoForks compoResumable{INVALID_SHORT_INDEX};

	CompoRemains compoRemains;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
void
backup(const TRegistry& registry, BackUpT<TRegistry>& copy) {
	overwrite(copy.compoRequested, registry.compoRequested);
	overwrite(copy.orthoRequested, registry.orthoRequested);
	overwrite(copy.compoResumable, registry.compoResumable);
}

//------------------------------------------------------------------------------

template <typename TRegistry>
void
restore(TRegistry& registry, const BackUpT<TRegistry>& copy) {
	overwrite(registry.compoRequested, copy.compoRequested);
	overwrite(registry.orthoRequested, copy.orthoRequested);
	overwrite(registry.compoResumable, copy.compoResumable);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#include "registry.inl"
