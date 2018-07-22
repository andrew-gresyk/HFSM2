#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

// TODO: check where TypeInfo is passed instead of std::type_index

class alignas(sizeof(std::type_index)) TypeInfo
	: public Wrap<std::type_index>
{
	using Base = Wrap<std::type_index>;

public:
	inline TypeInfo() = default;

	inline TypeInfo(const std::type_index type)
		: Base(type)
	{}

	template <typename T>
	static inline TypeInfo get() { return TypeInfo(typeid(T)); }
};

////////////////////////////////////////////////////////////////////////////////

}
}
