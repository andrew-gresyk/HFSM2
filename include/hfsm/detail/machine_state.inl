namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID NS, typename TC, typename TSL, LongIndex NFC, typename TPL, LongIndex NPC, typename TH>
struct RegisterT {
	using StateParents = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateParents& stateParents, const Parent parent) {
		static constexpr auto TYPE_ID = TSL::template index<TH>();
		assertEquality<NS, TYPE_ID>();

		stateParents[NS] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TC, typename TSL, LongIndex NFC, typename TPL, LongIndex NPC>
struct RegisterT<NS, TC, TSL, NFC, TPL, NPC, State<TC, TSL, NFC, TPL, NPC>> {
	using StateParents = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateParents&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <StateID NS, typename TA, typename TH>
_S<NS, TA, TH>::_S(StateParents& stateParents,
					const Parent parent,
					Parents& /*forkParents*/,
					ForkPointers& /*forkPointers*/)
{
	using Register = RegisterT<STATE_ID,
							   Context,
							   StateList,
							   Args::FORK_COUNT,
							   PayloadList,
							   Args::PLAN_CAPACITY,
							   Head>;
	Register::execute(stateParents, parent);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
bool
_S<NS, TA, TH>::deepGuard(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::guard, ::hfsm::Method::GUARD);

	ControlOrigin origin{control, STATE_ID};

	const auto requestCountBefore = control._requests.count();

	_head.widePreGuard(control.context());
	_head.guard(control);

	return requestCountBefore < control._requests.count();
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnterInitial(PlanControl& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepEnter(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::enter, ::hfsm::Method::ENTER);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter(control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
Status
_S<NS, TA, TH>::deepUpdate(FullControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::update, ::hfsm::Method::UPDATE);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update(control);
	// TODO: _head.widePostUpdate(control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
template <typename TEvent>
void
_S<NS, TA, TH>::deepReact(const TEvent& event,
						   FullControl& control)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, TransitionControl&)>(&Head::react);
	HFSM_LOG_STATE_METHOD(reaction, ::hfsm::Method::REACT);

	ControlOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());

	//_head.react(event, control);
	(_head.*reaction)(event, control);
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepExit(PlanControl& control) {
	HFSM_LOG_STATE_METHOD(&Head::exit, ::hfsm::Method::EXIT);

	ControlOrigin origin{control, STATE_ID};

	_head.exit(control);
	_head.widePostExit(control.context());
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID NS, typename TA, typename TH>
const char*
_S<NS, TA, TH>::name() {
	if (isBare())
		return "";
	else {
		const std::type_index type = typeid(Head);
		const char* const raw = type.name();

	#if defined(_MSC_VER)

		auto first =
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
							0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;

	#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

		return raw;

	#else

		return raw;

	#endif
	}
}

//------------------------------------------------------------------------------

template <StateID NS, typename TA, typename TH>
void
_S<NS, TA, TH>::deepGetNames(const LongIndex parent,
							  const RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

#endif

///////////////////////////////////////////////////////////////////////////////

}
}
