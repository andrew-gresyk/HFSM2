#pragma once

#include "wrap.hpp"

#include <typeindex>

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

class TypeInfo
	: public Wrap<std::type_index>
{
	using Base = Wrap<std::type_index>;

public:
	typedef std::type_index Native;

public:
	inline TypeInfo() = default;

	inline TypeInfo(std::type_info&& type)
		: Base(std::move(std::type_index(type)))
	{}

	template <typename T>
	static inline TypeInfo get() { return TypeInfo(typeid(T)); }
};

////////////////////////////////////////////////////////////////////////////////

}
}
