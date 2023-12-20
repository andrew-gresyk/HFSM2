namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Origin::Origin(ConstControlT& control_,
									 const StateID stateId_) noexcept
	: control{control_}
	, prevId{control._originId}
{
	HFSM2_ASSERT(stateId_ < StateList::SIZE);
	control._originId = stateId_;
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Origin::~Origin() noexcept {
	control._originId = prevId;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
ConstControlT<TArgs>::Region::Region(ConstControlT& control_,
									 const RegionID regionId_) noexcept
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(regionId_);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
ConstControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
