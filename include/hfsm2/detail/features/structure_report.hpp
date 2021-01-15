#ifdef HFSM2_ENABLE_STRUCTURE_REPORT

namespace hfsm2 {

//------------------------------------------------------------------------------

struct StructureEntry {
	bool isActive;
	const wchar_t* prefix;
	const char* name;
};

namespace detail {

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4324) // structure was padded due to alignment specifier
#endif

#pragma pack(push, 1)

struct alignas(alignof(void*)) StructureStateInfo {
	enum class RegionType : Short {
		COMPOSITE,
		ORTHOGONAL,

		COUNT
	};

	StructureStateInfo() noexcept = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	HFSM2_INLINE StructureStateInfo(const Long parent_,
									const RegionType regionType_,
									const Short depth_,
									const char* const name_) noexcept
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

#pragma pack(pop)

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
