namespace hfsm2 {
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
	static constexpr ShortIndex REGION_ID	= COMPO_INDEX + ORTHO_INDEX;
	static constexpr ForkID		ORTHO_ID	= (ForkID) -ORTHO_INDEX - 1;

	using Args			= TArgs;
	using Head			= THead;

	using StateList		= typename Args::StateList;
	using RegionList	= typename Args::RegionList;
	using PayloadList	= typename Args::PayloadList;

	using StateParents	= Array<Parent, Args::STATE_COUNT>;
	using Request		= RequestT<PayloadList>;
	using RequestType	= typename Request::Type;

	using StateData		= StateDataT  <Args>;
	using Control		= ControlT	  <Args>;
	using ControlRegion	= typename Control::Region;

	using FullControl	= FullControlT<Args>;
	using ControlLock	= typename FullControl::Lock;

	using HeadState		= _S <HEAD_ID, Args, Head>;
	using SubStates		= _OS<HEAD_ID + 1, COMPO_INDEX, ORTHO_INDEX + 1, Args, 0, TSubStates...>;
	using Forward		= _OF<Head, TSubStates...>;
	using SubStateList	= typename Forward::StateList;

	static constexpr ShortIndex REGION_SIZE	= SubStateList::SIZE;

	_O(StateData& stateData, const Parent parent);

	inline		 OrthoFork& orthoRequested(		 StateData& stateData)		 { return stateData.orthoRequested[ORTHO_INDEX];	}
	inline const OrthoFork& orthoRequested(const StateData& stateData) const { return stateData.orthoRequested[ORTHO_INDEX];	}

	inline		 OrthoFork& orthoRequested(		 Control&  control)		   { return orthoRequested(control.stateData());		}
	inline const OrthoFork& orthoRequested(const Control&  control)	 const { return orthoRequested(control.stateData());		}

	inline void   deepForwardGuard		(FullControl& control);
	inline void   deepGuard				(FullControl& control);

	inline void   deepEnterInitial		(Control& control);
	inline void   deepEnter				(Control& control);

	inline Status deepUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   deepReact				(const TEvent& event,
										 FullControl& control);

	inline void   deepExit				(Control& control);

	inline void   deepForwardRequest	(StateData& stateData, const RequestType transition);
	inline void   deepRequestRemain		(StateData& stateData);
	inline void   deepRequestRestart	(StateData& stateData);
	inline void   deepRequestResume		(StateData& stateData);
	inline void   deepChangeToRequested	(StateData& stateData, Control& control);

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
