namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreEntryGuard(Context& context) noexcept {
	TF::preEntryGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreEnter(Context& context) noexcept {
	TF::preEnter(context);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreReenter(Context& context) noexcept {
	TF::preReenter(context);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreUpdate(Context& context) noexcept {
	TF::preUpdate(context);
}

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePostUpdate(Context& context) noexcept {
	TF::	postUpdate(context);
}

//------------------------------------------------------------------------------

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreReact(const TEvent& event,
					 Context& context) noexcept
{
	TF::preReact(event, context);
}

template <typename TF>
template <typename TEvent>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePostReact(const TEvent& event,
					  Context& context) noexcept
{
	TF::	postReact(event, context);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePreExitGuard(Context& context) noexcept {
	TF::preExitGuard(context);
}

//------------------------------------------------------------------------------

template <typename TF>
HFSM2_CONSTEXPR(14)
void
B_<TF>::widePostExit(Context& context) noexcept {
	TF::postExit(context);
}

////////////////////////////////////////////////////////////////////////////////

}
}
