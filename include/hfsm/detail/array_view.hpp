#pragma once

namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 4)

template <typename T>
class ArrayView {
public:
	using Item = T;

	template <typename>
	friend class Iterator;

protected:
	ArrayView(const unsigned capacity);
	~ArrayView();

public:
	inline void clear()										{ _count = 0;				}

	inline unsigned resize(const unsigned count);

	template <typename TValue>
	inline unsigned operator << (TValue&& value);

	inline		 Item& operator[] (const unsigned i)		{ return get(i);			}
	inline const Item& operator[] (const unsigned i) const	{ return get(i);			}

	inline unsigned capacity() const						{ return _capacity;			}
	inline unsigned count() const							{ return _count;			}

protected:
	inline unsigned first() const							{ return 0;					}
	inline unsigned limit() const							{ return _count;			}

	inline unsigned prev(const unsigned i) const			{ return i - 1;				}
	inline unsigned next(const unsigned i) const			{ return i + 1;				}

	inline		 Item& get(const unsigned i);
	inline const Item& get(const unsigned i) const;

private:
	// hacks
	inline		 Item* data()		{ return reinterpret_cast<		Item*>(&_count + 1);	}
	inline const Item* data() const	{ return reinterpret_cast<const Item*>(&_count + 1);	}

protected:
	const unsigned _capacity;
	unsigned _count = 0;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////

}
}

#include "array_view.inl"
