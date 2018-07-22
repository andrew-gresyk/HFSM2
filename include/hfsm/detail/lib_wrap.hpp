#pragma once

namespace hfsm {
namespace detail {

//------------------------------------------------------------------------------

template <typename T>
class Wrap;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Wrap {
public:
	using Item = T;
	using Unit = uint32_t;
	enum { UNIT_COUNT = sizeof(Item) / sizeof(Unit) };

	using Storage = Unit[UNIT_COUNT];

public:
	inline Wrap() = default;

	template <typename... Ts>
	inline void create(Ts&&... ts)			{ new (&get()) Item(std::forward<Ts>(ts)...);	}

	inline Wrap(const Item& item)				{ get() = item;								}
	inline Wrap(Item&& item)					{ get() = std::move(item);					}

	inline Wrap& operator = (const Item& item)	{ get() = item;				return *this;	}
	inline Wrap& operator = (Item&& item)		{ get() = std::move(item);	return *this;	}

	inline void clear()										{ fill(_storage, 0);			}

	inline		 T& operator *()							{ return  get();				}
	inline const T& operator *() const						{ return  get();				}

	inline		 T* operator->()							{ return &get();				}
	inline const T* operator->() const						{ return &get();				}

	inline bool operator == (const Wrap other) const		{ return get() == other.get();	}

private:
	inline		 T& get()			{ return *reinterpret_cast<		 T* const>(&_storage);	}
	inline const T& get() const		{ return *reinterpret_cast<const T* const>(&_storage);	}

private:
	Storage _storage = { 0 };
};

////////////////////////////////////////////////////////////////////////////////

}
}
