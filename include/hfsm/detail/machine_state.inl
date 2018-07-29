namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

namespace {
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
struct Register {
	using StateRegistry2 = StateRegistry2T<TPL>;

	static inline void
	execute(StateRegistry2& stateRegistry, const Parent parent) {
		static constexpr auto TYPE_ID = TSL::template index<TH>();
		assertEquality<TID, TYPE_ID>();

		auto& stateInfo = stateRegistry[TID];
		stateInfo.parent = parent;
		stateInfo.payload.reset();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL>
struct Register<TID, TC, TSL, TPL, Base<TC, TSL, TPL>> {
	using StateRegistry2 = StateRegistry2T<TPL>;

	static inline void
	execute(StateRegistry2&, const Parent) {}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
}

////////////////////////////////////////////////////////////////////////////////

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
_S<TID, TC, TSL, TPL, TH>::_S(StateRegistry2& stateRegistry,
							  const Parent parent,
							  Parents& /*forkParents*/,
							  ForkPointers& /*forkPointers*/)
{
	Register<TID, TC, TSL, TPL, TH>::execute(stateRegistry, parent);
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
bool
_S<TID, TC, TSL, TPL, TH>::deepGuard(TransitionControl& control) {
	const auto requestCountBefore = control.requestCount();

	_head.widePreGuard(control.context());
	_head.guard(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::guard), LoggerInterface::Method::GUARD>(*logger);
#endif

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepEnterInitial(Control& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepEnter(Control& control) {
	_head.widePreEnter(control.context());
	_head.enter(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
bool
_S<TID, TC, TSL, TPL, TH>::deepUpdate(TransitionControl& control) {
	const auto requestCountBefore = control.requestCount();

	_head.widePreUpdate(control.context());
	_head.update(control);
	// TODO: _head.widePostUpdate(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::update), LoggerInterface::Method::UPDATE>(*logger);
#endif

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
template <typename TEvent>
void
_S<TID, TC, TSL, TPL, TH>::deepReact(const TEvent& event,
									 TransitionControl& control)
{
	_head.widePreReact(event, control.context());

	//_head.react(event, control);
	auto reaction = static_cast<void(Head::*)(const TEvent&, TransitionControl&)>(&Head::react);
	(_head.*reaction)(event, control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(reaction), LoggerInterface::Method::REACT>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepExit(Control& control) {
	_head.exit(control);
	_head.widePostExit(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
const char*
_S<TID, TC, TSL, TPL, TH>::name() {
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

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepGetNames(const LongIndex parent,
										const RegionType region,
										const ShortIndex depth,
										StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <StateID TID, typename TC, typename TSL, typename TPL, typename TH>
void
_S<TID, TC, TSL, TPL, TH>::deepIsActive(const bool isActive,
										LongIndex& index,
										MachineStructure& structure) const
{
	if (!isBare())
		structure[index++].isActive = isActive;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
}
