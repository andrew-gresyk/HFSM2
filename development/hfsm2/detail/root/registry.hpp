namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

struct alignas(2 * sizeof(Prong)) Parent final {
	HFSM2_CONSTEXPR(11)
	Parent() = default;

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_)										noexcept
		: forkId{forkId_}
	{}

	HFSM2_CONSTEXPR(11)
	Parent(const ForkID forkId_,
		   const Prong prong_)											noexcept
		: forkId{forkId_}
		, prong{prong_}
	{}

	HFSM2_CONSTEXPR(11)
	explicit operator bool()									  const noexcept	{
		return forkId != INVALID_FORK_ID &&
			   prong  != INVALID_PRONG;
	}

	ForkID forkId = INVALID_FORK_ID;
	Prong  prong  = INVALID_PRONG;
};

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

template <
	typename
  , typename
  , typename
  , Long
  , Long
  , Long
  , typename
  HFSM2_IF_SERIALIZATION(, Long)
  HFSM2_IF_PLANS(, Long)
  , typename
>
struct ArgsT;

template <typename>
struct RegistryT;

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , Long NOrthoCount
  , Long NOrthoUnits
  , typename TReactOrder
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct RegistryT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , NCompoCount
			 , NOrthoCount
			 , NOrthoUnits
			 , TReactOrder
			 HFSM2_IF_SERIALIZATION(, NSerialBits)
			 HFSM2_IF_PLANS(, NTaskCapacity)
			 , TPayload
		   >
	   > final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Long  REGION_COUNT		= RegionList::SIZE;
	static constexpr Short COMPO_COUNT		= NCompoCount;
	static constexpr Short ORTHO_COUNT		= NOrthoCount;
	static constexpr Short ORTHO_UNITS		= NOrthoUnits;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent,  STATE_COUNT>;

	using CompoParents	= StaticArrayT<Parent,  COMPO_COUNT>;
	using OrthoParents	= StaticArrayT<Parent,  ORTHO_COUNT>;
	using OrthoUnits	= StaticArrayT<Units,   ORTHO_UNITS>;
	using RegionHeads	= StaticArrayT<StateID, REGION_COUNT>;
	using RegionSizes	= StaticArrayT<Short,   REGION_COUNT>;

	using CompoForks	= StaticArrayT<Prong,   COMPO_COUNT>;
	using OrthoForks	= BitArrayT	  <ORTHO_UNITS * 8>;
	using OrthoBits		= typename OrthoForks::Bits;
	using CompoRemains	= BitArrayT	  <COMPO_COUNT>;

	using BackUp		= BackUpT	  <RegistryT>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_COUNT>;
#endif

	HFSM2_CONSTEXPR(14)	Prong activeSubState		(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive				()						  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive				(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable			(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange		(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter			(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit			(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent& forkParent	(const ForkID forkId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	OrthoBits requestedOrthoFork(const ForkID forkId)			noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate		(const Transition& request)		noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled		(const StateID stateId)			noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests			()								noexcept;
	HFSM2_CONSTEXPR(14)	void clear					()								noexcept;

	HFSM2_CONSTEXPR(11)	bool empty					()						  const noexcept;

	StateParents stateParents;
	CompoParents compoParents;
	OrthoParents orthoParents;
	OrthoUnits	 orthoUnits;

	RegionHeads	 regionHeads;
	RegionSizes	 regionSizes;

	CompoForks compoRequested{INVALID_PRONG};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_PRONG};
	CompoForks compoResumable{INVALID_PRONG};

	CompoRemains compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses compoStatuses;
#endif
};

//------------------------------------------------------------------------------

template <
	typename TConfig
  , typename TStateList
  , typename TRegionList
  , Long NCompoCount
  , typename TReactOrder
  HFSM2_IF_SERIALIZATION(, Long NSerialBits)
  HFSM2_IF_PLANS(, Long NTaskCapacity)
  , typename TPayload
>
struct RegistryT<
		   ArgsT<
			   TConfig
			 , TStateList
			 , TRegionList
			 , NCompoCount
			 , 0
			 , 0
			 , TReactOrder
			 HFSM2_IF_SERIALIZATION(, NSerialBits)
			 HFSM2_IF_PLANS(, NTaskCapacity)
			 , TPayload
		   >
	   > final
{
	using StateList		= TStateList;
	using RegionList	= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList::SIZE;
	static constexpr Long  REGION_COUNT		= RegionList::SIZE;
	static constexpr Short COMPO_COUNT		= NCompoCount;

	using Payload		= TPayload;
	using Transition	= TransitionT<Payload>;

	using StateParents	= StaticArrayT<Parent,  STATE_COUNT>;
	using CompoParents	= StaticArrayT<Parent,  COMPO_COUNT>;
	using RegionHeads	= StaticArrayT<StateID, REGION_COUNT>;
	using RegionSizes	= StaticArrayT<Short,   REGION_COUNT>;

	using CompoForks	= StaticArrayT<Prong,   COMPO_COUNT>;
	using OrthoForks	= BitArrayT	  <0>;
	using CompoRemains	= BitArrayT	  <COMPO_COUNT>;

	using BackUp		= BackUpT	  <RegistryT>;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses	= BitArrayT	  <COMPO_COUNT>;
#endif

	HFSM2_CONSTEXPR(14)	Prong activeSubState	(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(11)	bool isActive			()						  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isActive			(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isResumable		(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool isPendingChange	(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingEnter		(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool isPendingExit		(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	const Parent& forkParent(const ForkID forkId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool requestImmediate	(const Transition& request)		noexcept;
	HFSM2_CONSTEXPR(14)	void requestScheduled	(const StateID stateId)			noexcept;

	HFSM2_CONSTEXPR(14)	void clearRequests		()								noexcept;
	HFSM2_CONSTEXPR(14)	void clear				()								noexcept;

	HFSM2_CONSTEXPR(11)	bool empty				()						  const noexcept;

	StateParents stateParents;
	CompoParents compoParents;

	RegionHeads	 regionHeads;
	RegionSizes	 regionSizes;

	CompoForks compoRequested{INVALID_PRONG};
	OrthoForks orthoRequested;
	CompoForks compoActive	 {INVALID_PRONG};
	CompoForks compoResumable{INVALID_PRONG};

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
