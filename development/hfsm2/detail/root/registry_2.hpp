namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

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
	using StateList			= TStateList;
	using RegionList		= TRegionList;

	static constexpr Long  STATE_COUNT		= StateList ::SIZE;
	static constexpr Long  REGION_COUNT		= RegionList::SIZE;
	static constexpr Short COMPO_COUNT		= NCompoCount;

	using Payload			= TPayload;
	using Transition		= TransitionT<Payload>;

	using StateParents		= StaticArrayT<Parent  , STATE_COUNT >;
	using CompoParents		= StaticArrayT<Parent  , COMPO_COUNT >;
	using RegionHeads		= StaticArrayT<StateID , REGION_COUNT>;
	using RegionSizes		= StaticArrayT<Long    , REGION_COUNT>;

	using CompoForks		= StaticArrayT<Prong   , COMPO_COUNT >;
	using OrthoForks		= BitArrayT	  <0>;
	using CompoRemains		= BitArrayT	  <			 COMPO_COUNT >;

#if HFSM2_PLANS_AVAILABLE()
	using CompoStatuses		= BitArrayT	  <			 COMPO_COUNT >;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct BackUp final {
		CompoForks compoRequested;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	Prong activeSubState	(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(11)	bool  isActive			()						  const noexcept;
	HFSM2_CONSTEXPR(14)	bool  isActive			(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool  isResumable		(const StateID stateId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	bool  isPendingEnter	(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool  isPendingChange	(const StateID stateId)	  const noexcept;
	HFSM2_CONSTEXPR(14)	bool  isPendingExit		(const StateID stateId)	  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	const Parent& forkParent(const ForkID forkId)	  const noexcept;

	HFSM2_CONSTEXPR(14)	void  requestImmediate	(const Transition& request)		noexcept;

	HFSM2_CONSTEXPR(14)	void  requestScheduled	(const StateID stateId)			noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(14)	void  clearRequests		()								noexcept;
	HFSM2_CONSTEXPR(14)	void  clear				()								noexcept;

	HFSM2_CONSTEXPR(11)	bool  empty				()						  const noexcept;

	HFSM2_CONSTEXPR(14) void  backup			(	   BackUp& copy)	  const noexcept;
	HFSM2_CONSTEXPR(14) void  restore			(const BackUp& copy)			noexcept;

	HFSM2_CONSTEXPR(11)	bool  operator !=		(const BackUp& copy)	  const noexcept;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	StateParents	stateParents;
	CompoParents	compoParents;
	RegionHeads		regionHeads;
	RegionSizes		regionSizes;

	CompoForks		compoRequested{INVALID_PRONG};
	OrthoForks		orthoRequested;
	CompoForks		compoActive   {INVALID_PRONG};
	CompoForks		compoResumable{INVALID_PRONG};

	CompoRemains	compoRemains;

#if HFSM2_PLANS_AVAILABLE()
	CompoStatuses	compoStatuses;
#endif
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "registry_2.inl"
