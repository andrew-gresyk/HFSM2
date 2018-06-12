namespace hfsm {

////////////////////////////////////////////////////////////////////////////////

template <typename TC, unsigned TMS>
template <typename T>
M<TC, TMS>::_S<T>::_S(StateRegistry& stateRegistry,
					  const Parent parent,
					  Parents& stateParents,
					  Parents& /*forkParents*/,
					  ForkPointers& /*forkPointers*/)
{
	const auto id = stateRegistry.add(TypeInfo::get<Client>());
	stateParents[id] = parent;
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
bool
M<TC, TMS>::_S<T>::deepSubstitute(Control& control,
								  Context& context)
{
	const unsigned requestCountBefore = control.requestCount();

	_client.widePreSubstitute(context);
	_client.substitute(control, context);

	return requestCountBefore < control.requestCount();
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepEnterInitial(Context& context) {
	deepEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepEnter(Context& context) {
	_client.widePreEnter(context);
	_client.enter(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
bool
M<TC, TMS>::_S<T>::deepUpdateAndTransition(Control& control,
										   Context& context)
{
	_client.widePreUpdate(context);
	_client.update(context);

	const unsigned requestCountBefore = control.requestCount();

	_client.widePreTransition(context);
	_client.transition(control, context);

	return requestCountBefore < control.requestCount();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepUpdate(Context& context) {
	_client.widePreUpdate(context);
	_client.update(context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
template <typename TEvent>
void
M<TC, TMS>::_S<T>::deepReact(const TEvent& event,
							 Control& control,
							 Context& context)
{
	_client.widePreReact(event, context);
	_client.react(event, control, context);
}

//------------------------------------------------------------------------------

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepLeave(Context& context) {
	_client.leave(context);
	_client.widePostLeave(context);
}

//------------------------------------------------------------------------------

#ifdef HFSM_ENABLE_STRUCTURE_REPORT

template <typename TC, unsigned TMS>
template <typename T>
const char*
M<TC, TMS>::_S<T>::name() {
	if (isBare())
		return "";
	else {
		const char* const raw = TypeInfo::get<Client>()->name();

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepGetNames(const unsigned parent,
								const enum StateInfo::RegionType region,
								const unsigned depth,
								StateInfos& _stateInfos) const
{
	_stateInfos << StateInfo { parent, region, depth, name() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TC, unsigned TMS>
template <typename T>
void
M<TC, TMS>::_S<T>::deepIsActive(const bool isActive,
								unsigned& index,
								MachineStructure& structure) const
{
	if (!isBare())
		structure[index++].isActive = isActive;
}

#endif

////////////////////////////////////////////////////////////////////////////////

}
