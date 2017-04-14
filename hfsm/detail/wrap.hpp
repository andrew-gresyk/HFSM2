#pragma once

#include "utility.hpp"

namespace hfsm::detail {

//------------------------------------------------------------------------------

template <typename T>
class Wrap;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Wrap {
	using Item = T;
	using Storage = typename std::aligned_storage<sizeof(Item)>::type;

public:
	inline Wrap() = default;

	template <typename... Ts>
	inline void create(Ts&&... ts)			{ new (&get()) Item(std::forward<Ts>(ts)...);	}

	inline Wrap(const Item item)							{ get() = item;					}
	inline Wrap(Item&& item)								{ get() = std::move(item);		}

	inline Wrap& operator = (const Item item)	{ get() = item;				return *this;	}
	inline Wrap& operator = (Item&& item)		{ get() = std::move(item);	return *this;	}

	inline void clear()										{ fill(_storage, 0);			}

	inline		 T& operator *()							{ return  get();				}
	inline const T& operator *() const						{ return  get();				}

	inline		 T* operator->()							{ return &get();				}
	inline const T* operator->() const						{ return &get();				}

	inline explicit operator bool() const					{ return integer() != 0;		}

	inline bool operator == (const Wrap other) const		{ return get() == other.get();	}

private:
	inline		 T& get()			{ return *reinterpret_cast<		 T* const>(&_storage);	}
	inline const T& get() const		{ return *reinterpret_cast<const T* const>(&_storage);	}

	inline unsigned integer() const { return *reinterpret_cast<const unsigned* const>(&_storage); }
	static_assert(sizeof(unsigned) < sizeof(Storage), "");

private:
	Storage _storage;
};

////////////////////////////////////////////////////////////////////////////////

}
