namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID NHeadIndex,
		  ShortIndex NCompoIndex,
		  ShortIndex NOrthoIndex,
		  typename TArgs,
		  typename THead,
		  typename... TSubStates>
struct _O final {
	static constexpr StateID	HEAD_ID		= NHeadIndex;
	static constexpr ShortIndex COMPO_INDEX	= NCompoIndex;
	static constexpr ShortIndex ORTHO_INDEX	= NOrthoIndex;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			 = TArgs;
	using Head			 = THead;

	using StateList		 = typename Args::StateList;
	using PayloadList	 = typename Args::PayloadList;

	using StateParents	 = Array<Parent, Args::STATE_COUNT>;
	using Request		 = RequestT<PayloadList>;
	using RequestType	 = typename Request::Type;

	using Registry		 = RegistryT	 <Args>;
	using ControlLock	 = ControlLockT	 <Args>;
	using ControlRegion	 = ControlRegionT<Args>;
	using Control		 = ControlT		 <Args>;
	using PlanControl	 = PlanControlT	 <Args>;
	using FullControl	 = FullControlT	 <Args>;

	using HeadState		 = _S <HEAD_ID, Args, Head>;
	using SubStates		 = _OS<HEAD_ID + 1, COMPO_INDEX, ORTHO_INDEX + 1, Args, 0, TSubStates...>;
	using Forward		 = _OF<Head, TSubStates...>;
	using SubStateList	 = typename Forward::StateList;

	_O(Registry& registry, const Parent parent);

	inline		 OrthoFork& orthoRequested(		 Registry& registry)	   { return registry.orthoRequested[ORTHO_INDEX];	}
	inline const OrthoFork& orthoRequested(const Registry& registry) const { return registry.orthoRequested[ORTHO_INDEX];	}

	inline		 OrthoFork& orthoRequested(		 Control&  control)		   { return orthoRequested(control.registry());		}
	inline const OrthoFork& orthoRequested(const Control&  control)	 const { return orthoRequested(control.registry());		}

	inline void   deepForwardGuard		(FullControl& control);
	inline void   deepGuard				(FullControl& control);

	inline void   deepEnterInitial		(PlanControl& control);
	inline void   deepEnter				(PlanControl& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   deepReact				(const TEvent& event,
										 FullControl& control);

	inline void   deepExit				(PlanControl& control);

	inline void   deepForwardRequest	(Registry& registry, const RequestType transition);
	inline void   deepRequestRemain		(Registry& registry);
	inline void   deepRequestRestart	(Registry& registry);
	inline void   deepRequestResume		(Registry& registry);
	inline void   deepChangeToRequested	(Registry& registry, PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	using RegionType		= typename StructureStateInfo::RegionType;

	static constexpr LongIndex NAME_COUNT	 = HeadState::NAME_COUNT  + SubStates::NAME_COUNT;

	void deepGetNames(const LongIndex parent,
					  const RegionType region,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	HeadState _headState;
	SubStates _subStates;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_orthogonal.inl"
