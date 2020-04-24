namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePreEntryGuard(Context& context) {
	TF_::preEntryGuard(context);
	B_<TR_...>::widePreEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePreEnter(Context& context) {
	TF_::preEnter(context);
	B_<TR_...>::widePreEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePreReenter(Context& context) {
	TF_::preReenter(context);
	B_<TR_...>::widePreReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePreUpdate(Context& context) {
	TF_::preUpdate(context);
	B_<TR_...>::widePreUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
template <typename TEvent>
void
B_<TF_, TR_...>::widePreReact(const TEvent& event,
							  Context& context)
{
	TF_::preReact(event, context);
	B_<TR_...>::widePreReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePreExitGuard(Context& context) {
	TF_::preExitGuard(context);
	B_<TR_...>::widePreExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF_, typename... TR_>
void
B_<TF_, TR_...>::widePostExit(Context& context) {
	TF_::postExit(context);
	B_<TR_...>::widePostExit(context);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TF_>
void
B_<TF_>::widePreEntryGuard(Context& context) {
	TF_::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF_>
void
B_<TF_>::widePreEnter(Context& context) {
	TF_::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF_>
void
B_<TF_>::widePreReenter(Context& context) {
	TF_::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF_>
void
B_<TF_>::widePreUpdate(Context& context) {
	TF_::preUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF_>
template <typename TEvent>
void
B_<TF_>::widePreReact(const TEvent& event,
					  Context& context)
{
	TF_::preReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF_>
void
B_<TF_>::widePreExitGuard(Context& context) {
	TF_::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF_>
void
B_<TF_>::widePostExit(Context& context) {
	TF_::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
