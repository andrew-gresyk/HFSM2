namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename TH>
M<TC, TMS>::_S<TH>::_S(StateRegistry& stateRegistry,
					   const Parent parent,
					   Parents& stateParents,
					   Parents& /*forkParents*/,
					   ForkPointers& /*forkPointers*/)
{
	const auto id = stateRegistry.add(TypeInfo::get<Head>());
	stateParents[id] = parent;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
bool
M<TC, TMS>::_S<TH>::deepSubstitute(Control& control,
								   Context& context,
								   LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::substitute), LoggerInterface::Method::Substitute>(*logger));

	const unsigned requestCountBefore = control.requestCount();

	_head.widePreSubstitute(context);
	_head.substitute(control, context);

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepEnterInitial(Context& context,
									 LoggerInterface* const logger)
{
	deepEnter(context, logger);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepEnter(Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::enter), LoggerInterface::Method::Enter>(*logger));

	_head.widePreEnter(context);
	_head.enter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
bool
M<TC, TMS>::_S<TH>::deepUpdateAndTransition(Control& control,
											Context& context,
											LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::update), LoggerInterface::Method::Update>(*logger));

	_head.widePreUpdate(context);
	_head.update(context);

	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::transition), LoggerInterface::Method::Transition>(*logger));

	const unsigned requestCountBefore = control.requestCount();

	_head.widePreTransition(context);
	_head.transition(control, context);

	return requestCountBefore < control.requestCount();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepUpdate(Context& context,
							   LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::update), LoggerInterface::Method::Update>(*logger));

	_head.widePreUpdate(context);
	_head.update(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
template <typename TEvent>
void
M<TC, TMS>::_S<TH>::deepReact(const TEvent& event,
							  Control& control,
							  Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::react<TEvent>), LoggerInterface::Method::React>(*logger));

	_head.widePreReact(event, context);
	_head.react(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepLeave(Context& context,
							  LoggerInterface* const HFSM_IF_LOGGER(logger))
{
	HFSM_IF_LOGGER(if (logger) log<decltype(&Head::leave), LoggerInterface::Method::Leave>(*logger));

	_head.leave(context);
	_head.widePostLeave(context);
}

//------------------------------------------------------------------------------

#if defined HFSM_ENABLE_STRUCTURE_REPORT || defined HFSM_ENABLE_LOG_INTERFACE

template <typename TC, unsigned TMS>
template <typename TH>
const char*
M<TC, TMS>::_S<TH>::name() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Head>()->name();

		unsigned first =

		#if defined(_MSC_VER)
			raw[0] == 's' ? 7 : // Struct
			raw[0] == 'c' ? 6 : // Class
		#elif defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
			raw[0] ? 1 :
		#endif
			0;

		for (auto c = first; raw[c]; ++c)
			if (raw[c] == ':')
				first = c + 1;

		return raw + first;
	}
}

#endif

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepGetNames(const unsigned parent,
								 const enum StateInfo::RegionType region,
								 const unsigned depth,
								 StateInfos& _stateInfos) const
{
	_stateInfos << StateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename TH>
void
M<TC, TMS>::_S<TH>::deepIsActive(const bool isActive,
								 unsigned& index,
								 MachineStructure& structure) const
{
	if (!isBare())
		structure[index++].isActive = isActive;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
