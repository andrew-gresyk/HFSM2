namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TContext, unsigned TMaxSubstitutions>
template <typename T>
struct M<TContext, TMaxSubstitutions>::_S {
	using Client = T;

	enum : unsigned {
		ReverseDepth = 1,
		DeepWidth	 = 0,
		StateCount	 = 1,
		ForkCount	 = 0,
		ProngCount	 = 0,
		Width		 = 1,
	};

	_S(StateRegistry& stateRegistry,
	   const Parent parent,
	   Parents& stateParents,
	   Parents& forkParents,
	   ForkPointers& forkPointers);

	inline void deepForwardSubstitute(Control&, Context&)					{}
	inline bool deepSubstitute(Control& control, Context& context);

	inline void deepEnterInitial(Context& context);
	inline void deepEnter(Context& context);

	inline bool deepUpdateAndTransition(Control& control, Context& context);
	inline void deepUpdate(Context& context);

	template <typename TEvent>
	inline void deepReact(const TEvent& event, Control& control, Context& context);
		
	inline void deepLeave(Context& context);

	inline void deepForwardRequest(const enum Transition::Type)				{}
	inline void deepRequestRemain()											{}
	inline void deepRequestRestart()										{}
	inline void deepRequestResume()											{}
	inline void deepChangeToRequested(Context&)	{}

	Client _client;

	HSFM_DEBUG_ONLY(const TypeInfo _type = TypeInfo::get<Client>());
};

////////////////////////////////////////////////////////////////////////////////

}

#include "machine_state_methods.inl"
