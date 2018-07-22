namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, typename TPL, typename TH>
_S<TC, TPL, TH>::_S(StateRegistryBase& stateRegistry,
					const Parent /*parent*/,
					Parents& /*forkParents*/,
					ForkPointers& /*forkPointers*/)
{
	stateRegistry.insert(TypeInfo::get<Head>());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepRegister(StateRegistryBase& stateRegistry,
							  const Parent parent)
{
	auto& stateInfo = stateRegistry.get(TypeInfo::get<Head>());
	stateInfo.parent = parent;
	stateInfo.payload.reset();
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
bool
_S<TC, TPL, TH>::deepGuard(TransitionControl& control) {
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

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepEnterInitial(Control& control) {
	deepEnter(control);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepEnter(Control& control) {
	_head.widePreEnter(control.context());
	_head.enter(control);

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::enter), LoggerInterface::Method::ENTER>(*logger);
#endif
}

//------------------------------------------------------------------------------

template <typename TC, typename TPL, typename TH>
bool
_S<TC, TPL, TH>::deepUpdate(TransitionControl& control)
{
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

template <typename TC, typename TPL, typename TH>
template <typename TEvent>
void
_S<TC, TPL, TH>::deepReact(const TEvent& event,
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

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepExit(Control& control) {
	_head.exit(control);
	_head.widePostExit(control.context());

#ifdef HFSM_ENABLE_LOG_INTERFACE
	if (auto* const logger = control.logger())
		log<decltype(&Head::exit), LoggerInterface::Method::EXIT>(*logger);
#endif
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, typename TPL, typename TH>
const char*
_S<TC, TPL, TH>::name() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Head>()->name();

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

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepGetNames(const LongIndex parent,
							  const enum StructureStateInfo::RegionType region,
							  const ShortIndex depth,
							  StructureStateInfos& _stateInfos) const
{
	_stateInfos << StructureStateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, typename TPL, typename TH>
void
_S<TC, TPL, TH>::deepIsActive(const bool isActive,
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
