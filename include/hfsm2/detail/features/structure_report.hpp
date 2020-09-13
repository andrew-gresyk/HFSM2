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
<<<<<<< HEAD
	enum RegionType : Short {
=======
	enum RegionType : ShortIndex {
>>>>>>> master
		COMPOSITE,
		ORTHOGONAL,
	};

	StructureStateInfo() = default;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

<<<<<<< HEAD
	HFSM2_INLINE StructureStateInfo(const Long parent_,
									const RegionType region_,
									const Short depth_,
=======
	HFSM2_INLINE StructureStateInfo(const LongIndex parent_,
									const RegionType region_,
									const ShortIndex depth_,
>>>>>>> master
									const char* const name_)
		: name{name_}
		, parent{parent_}
		, region{region_}
		, depth{depth_}
	{}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	const char* name;
<<<<<<< HEAD
	Long parent;
	RegionType region;
	Short depth;
=======
	LongIndex parent;
	RegionType region;
	ShortIndex depth;
>>>>>>> master
};

#pragma pack(pop)

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////

}
}

#endif
