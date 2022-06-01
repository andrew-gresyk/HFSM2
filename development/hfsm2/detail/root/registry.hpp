namespace hfsm2 {
namespace detail {

//------------------------------------------------------------------------------

enum Strategy {
	Composite,
	Resumable,
	Selectable,

//#if HFSM2_UTILITY_THEORY_AVAILABLE()
	Utilitarian,
	RandomUtil,
//#endif
};

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

struct alignas(2 * sizeof(Short)) Parent final {
	HFSM2_CONSTEXPR(11)
	Parent() = default;

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_)										  noexcept
		: forkId{forkId_}
	{}

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_,
		   const Short prong_)											  noexcept
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_CONSTEXPR(11)
	explicit operator bool()										const noexcept	{
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_SHORT;
	}

	ForkID forkId = INVALID_FORK_ID;
	Short  prong  = INVALID_SHORT;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
struct BackUpT final {
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
		HFSM2_IF_SERIALIZATION(, Long)
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
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
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
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>> final
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
	using OrthoForks	= BitArrayT	  <ORTHO_UNITS * 8>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArrayT	  <COMPO_REGIONS>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_REGIONS>;
#endif

	using BackUp		= BackUpT<RegistryT>;

	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive		 ()								const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive		 (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable	 (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter	 (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit	 (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent&	  forkParent(const ForkID forkId)	const noexcept;

	HFSM2_CONSTEXPR(14)	OrthoBits requestedOrthoFork(const ForkID forkId)		  noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate(const Transition& request)		  noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled(const StateID stateId)			  noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests()									  noexcept;
	HFSM2_CONSTEXPR(14)	void clear()											  noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits orthoUnits;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses compoStatuses;
#endif
};

//------------------------------------------------------------------------------

template <typename TContext
		, typename TConfig
		, typename TStateList
		, typename TRegionList
		, Long NCompoCount
		HFSM2_IF_SERIALIZATION(, Long NSerialBits)
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
					 HFSM2_IF_SERIALIZATION(, NSerialBits)
					 , NSubstitutionLimit
					 HFSM2_IF_PLANS(, NTaskCapacity)
					 , TPayload>> final
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
	using OrthoForks	= BitArrayT	  <0>;
	using CompoRemains	= BitArrayT	  <COMPO_REGIONS>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_REGIONS>;
#endif

	using BackUp		= BackUpT<RegistryT>;

	HFSM2_CONSTEXPR(14)	Short activeSubState (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive		 ()								const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive		 (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable	 (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter	 (const StateID stateId)		const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit	 (const StateID stateId)		const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent& forkParent(const ForkID forkId)		const noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate(const Transition& request)		  noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled(const StateID stateId)			  noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests()									  noexcept;
	HFSM2_CONSTEXPR(14)	void clear()											  noexcept;

	StateParents stateParents;
	CompoParents compoParents;

	CompoForks compoRequested{INVALID_SHORT};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_SHORT};
	CompoForks compoResumable{INVALID_SHORT};

	CompoRemains compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses compoStatuses;
#endif
};

////////////////////////////////////////////////////////////////////////////////

template <typename TRegistry>
HFSM2_CONSTEXPR(14)
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
HFSM2_CONSTEXPR(14)
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
