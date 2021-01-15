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

struct alignas(2 * sizeof(Short)) Parent {
	HFSM2_INLINE Parent() = default;

	HFSM2_INLINE Parent(const ForkID forkId_) noexcept
		: forkId{forkId_}
	{}

	HFSM2_INLINE Parent(const ForkID forkId_,
						const Short prong_) noexcept
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_INLINE explicit operator bool() const noexcept {
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT;
	}

	ForkID forkId = INVALID_FORK_ID;
	Short  prong  = INVALID_SHORT;
};

#pragma pack(pop)

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

template <typename
		, typename
		, typename
		, typename
		, Long
		, Long
		, Long
		, Long
		, Long
		HFSM2_IF_PLANS(, Long)
		, typename>
struct ArgsT;

template <typename>
struct RegistryT;

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NOrthoCount
		, Long NOrthoUnits
		, Long NSerialBits
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct RegistryT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , NOrthoCount
					 , NOrthoUnits
					 , NSerialBits
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;
	static constexpr Short ORTHO_REGIONS	= NOrthoCount;
	static constexpr Short ORTHO_UNITS		= NOrthoUnits;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;

	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;
	using OrthoParents	= StaticArrayT<Parent, ORTHO_REGIONS>;
	using OrthoUnits	= StaticArrayT<Units,  ORTHO_UNITS>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <Short,  ORTHO_UNITS>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArrayT	  <Short,  COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		 (const StateID stateId) const noexcept;
	bool isResumable	 (const StateID stateId) const noexcept;

	bool isPendingChange (const StateID stateId) const noexcept;
	bool isPendingEnter	 (const StateID stateId) const noexcept;
	bool isPendingExit	 (const StateID stateId) const noexcept;

	HFSM2_INLINE const Parent&	   forkParent(const ForkID forkId) const noexcept;

	HFSM2_INLINE OrthoBits requestedOrthoFork(const ForkID forkId) noexcept;

	bool requestImmediate(const Transition& request) noexcept;
	void requestScheduled(const StateID stateId) noexcept;

	void clearRequests() noexcept;
	void clear() noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		, Long NSerialBits
		, Long NSubstitutionLimit
		HFSM2_IF_PLANS(, Long NTaskCapacity)
		, typename TPayload>
struct RegistryT<ArgsT<TContext
					 , TConfig
					 , TStateList
					 , TRegionList
					 , NCompoCount
					 , 0
					 , 0
					 , NSerialBits
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>>
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Short COMPO_REGIONS	= NCompoCount;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent, STATE_COUNT>;
	using CompoParents	= StaticArrayT<Parent, COMPO_REGIONS>;

	using CompoForks	= StaticArrayT<Short,  COMPO_REGIONS>;
	using OrthoForks	= BitArrayT	  <Short,  0>;
	using CompoRemains	= BitArrayT	  <Short,  COMPO_REGIONS>;

	using BackUp		= BackUpT<RegistryT>;

	bool isActive		 (const StateID stateId) const noexcept;
	bool isResumable	 (const StateID stateId) const noexcept;

	bool isPendingChange (const StateID stateId) const noexcept;
	bool isPendingEnter	 (const StateID stateId) const noexcept;
	bool isPendingExit	 (const StateID stateId) const noexcept;

	HFSM2_INLINE const Parent& forkParent(const ForkID forkId) const noexcept;

	bool requestImmediate(const Transition& request) noexcept;
	void requestScheduled(const StateID stateId) noexcept;

	void clearRequests() noexcept;
	void clear() noexcept;

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;
};

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
void
backup(const TRegistry& registry,
	   BackUpT<TRegistry>& copy) noexcept
{
	overwrite(copy.compoRequested, registry.compoRequested);
	overwrite(copy.orthoRequested, registry.orthoRequested);
	overwrite(copy.compoResumable, registry.compoResumable);
}

//------------------------------------------------------------------------------

template <typename TRegistry>
void
restore(TRegistry& registry,
		const BackUpT<TRegistry>& copy) noexcept
{
	overwrite(registry.compoRequested, copy.compoRequested);
	overwrite(registry.orthoRequested, copy.orthoRequested);
	overwrite(registry.compoResumable, copy.compoResumable);
}

////////////////////////////////////////////////////////////////////////////////

}
}

#include "registry_1.inl"
#include "registry_2.inl"
