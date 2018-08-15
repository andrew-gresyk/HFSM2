namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

namespace {

template <StateID TID, typename TC, typename TSL, typename TPL, LongIndex NPC, typename TH>
struct Register {
	using StateRegistry = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateRegistry& stateRegistry, const Parent parent) {
		static constexpr auto TYPE_ID = TSL::template index<TH>();
		assertEquality<TID, TYPE_ID>();

		stateRegistry[TID] = parent;
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, LongIndex NPC>
struct Register<TID, TC, TSL, TPL, NPC, State<TC, TSL, TPL, NPC>> {
	using StateRegistry = Array<Parent, TSL::SIZE>;

	static inline void
	execute(StateRegistry&, const Parent) {}
};

}

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TA, typename TH>
_S<TID, TA, TH>::_S(StateRegistry& stateRegistry,
					const Parent parent,
					Parents& /*forkParents*/,
					ForkPointers& /*forkPointers*/)
{
	Register<STATE_ID, Context, StateList, PayloadList, Args::PLAN_CAPACITY, Head>::execute(stateRegistry, parent);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
bool
_S<TID, TA, TH>::deepGuard(FullControl& control) {
	const auto requestCountBefore = control._requests.count();

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::guard), LoggerInterface::Method::GUARD>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	_head.widePreGuard(control.context());
	_head.guard((TransitionControl&) control);

	return requestCountBefore < control._requests.count();
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
void
_S<TID, TA, TH>::deepEnterInitial(PlanControl& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TA, typename TH>
void
_S<TID, TA, TH>::deepEnter(PlanControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	_head.widePreEnter(control.context());
	_head.enter((Control&) control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
Status
_S<TID, TA, TH>::deepUpdate(FullControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::update), LoggerInterface::Method::UPDATE>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	_head.widePreUpdate(control.context());
	_head.update((TransitionControl&) control);
	// TODO: _head.widePostUpdate(control.context());

	return control._status;
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
template <typename TEvent>
void
_S<TID, TA, TH>::deepReact(const TEvent& event,
						   FullControl& control)
{
	auto reaction = static_cast<void(Head::*)(const TEvent&, TransitionControl&)>(&Head::react);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(reaction), LoggerInterface::Method::REACT>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	_head.widePreReact(event, control.context());

	//_head.react(event, control);
	(_head.*reaction)(event, (TransitionControl&) control);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TA, typename TH>
void
_S<TID, TA, TH>::deepExit(PlanControl& control) {
#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif

	ControlOrigin origin{control, STATE_ID};

	_head.exit((Control&) control);
	_head.widePostExit(control.context());
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TA, typename TH>
const char*
_S<TID, TA, TH>::name() {
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

template <StateID TID, typename TA, typename TH>
void
_S<TID, TA, TH>::deepGetNames(const LongIndex parent,
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
