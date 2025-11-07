#if HFSM2_STRUCTURE_REPORT_AVAILABLE()

namespace hfsm2 {

//------------------------------------------------------------------------------

struct StructureEntry final {
	HFSM2_CONSTEXPR(11)
	StructureEntry(const bool isActive_ = false,
				   const wchar_t* const prefix_ = nullptr,
				   const char   * const name_   = nullptr)				noexcept
		: isActive{isActive_}
		, prefix  {prefix_  }
		, name    {name_    }
	{}

	bool isActive;
	const wchar_t* prefix;
	const char   * name;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

enum class RegionType {
	COMPOSITE,
	ORTHOGONAL,

	COUNT
};

//------------------------------------------------------------------------------

struct StructureStateInfo final {
	StructureStateInfo() noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_CONSTEXPR(11)
	StructureStateInfo(const Long parent_,
					   const RegionType regionType_,
					   const Short depth_,
					   const char* const name_)							noexcept
		: name{name_}
		, parent{parent_}
		, regionType{regionType_ }
		, depth{depth_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	const char* name	  = nullptr;
	Long parent			  = INVALID_LONG;
	RegionType regionType = RegionType::COUNT;
	Short depth			  = INVALID_SHORT;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
