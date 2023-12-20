namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
PlanControlT<TArgs>::Region::Region(PlanControlT& control_,
									const RegionID regionId_,
									const StateID index,
									const Long size) noexcept
	: control  {control_}
	, prevId   {control._regionId}
	, prevIndex{control._regionStateId}
	, prevSize {control._regionSize}
{
	control.setRegion(regionId_, index, size);
}

//------------------------------------------------------------------------------

template <typename TArgs>
HFSM2_CONSTEXPR(20)
PlanControlT<TArgs>::Region::~Region() noexcept {
	control.resetRegion(prevId, prevIndex, prevSize);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::setRegion(const RegionID regionId_,
							   const StateID index,
							   const Long size) noexcept
{
	HFSM2_ASSERT(_regionId <= regionId_ && regionId_ <  RegionList::SIZE);
	HFSM2_ASSERT(_regionStateId <= index && index + size <= _regionStateId + _regionSize);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TArgs>
HFSM2_CONSTEXPR(14)
void
PlanControlT<TArgs>::resetRegion(const RegionID regionId_,
								 const StateID index,
								 const Long size) noexcept
{
	HFSM2_ASSERT(regionId_ <= _regionId && _regionId < RegionList::SIZE);
	HFSM2_ASSERT(index <= _regionStateId && _regionStateId + _regionSize <= index + size);

	_regionId	   = regionId_;
	_regionStateId = index;
	_regionSize	   = size;

	_taskStatus.clear();
}

////////////////////////////////////////////////////////////////////////////////

}
}
