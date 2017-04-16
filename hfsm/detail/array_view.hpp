#pragma once

#include <assert.h>

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
	ArrayView(const unsigned capacity, unsigned& count);
	~ArrayView();

public:
	inline void clear()										{ _count = 0;				}

	inline Item& grow();

	template <typename... TS>
	inline Item& emplace(TS&&... ts);

	inline unsigned resize(const unsigned count);

	template <typename TValue>
	inline unsigned operator << (const TValue& value);

	template <typename TValue>
	inline unsigned operator << (TValue&& value);

	inline		 Item* storage()							{ return &get(0);			}
	inline const Item* storage() const						{ return &get(0);			}

	inline		 Item& operator[] (const unsigned i)		{ return get(i);			}
	inline const Item& operator[] (const unsigned i) const	{ return get(i);			}

	inline		 Item& tail()								{ return get(_count - 1);	}
	inline const Item& tail() const							{ return get(_count - 1);	}

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

template <typename T>
ArrayView<T>::ArrayView(const unsigned capacity)
	: _capacity(capacity)
{}

//··············································································

template <typename T>
ArrayView<T>::ArrayView(const unsigned capacity, unsigned& count)
	: _capacity(capacity)
{
	count = resize(count);
}

//··············································································

template <typename T>
ArrayView<T>::~ArrayView() {
	if (_count > 0)
		for (int i = _count - 1; i >= 0; --i)
			get(i).~Item();
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::grow() {
	assert(_count < _capacity);

	auto& item = get(_count++);
	item = Item();

	return item;
}

//··············································································

template <typename T>
template <typename... TS>
T&
ArrayView<T>::emplace(TS&&... ts) {
	assert(_count < _capacity);

	auto& item = get(_count++);
	return *new (&item) Item(std::forward<TS>(ts)...);
}

//··············································································

template <typename T>
unsigned
ArrayView<T>::resize(const unsigned count) {
	const unsigned clampedCount = count < _capacity ?
		count : _capacity;

	if (clampedCount > _count) {
		for (unsigned i = _count; i < clampedCount; ++i)
			get(i) = Item();
	}
	else if (clampedCount < _count) {
		for (unsigned i = _count - 1; i >= clampedCount; --i)
			get(i).~Item();
	}

	return _count = clampedCount;
}

//------------------------------------------------------------------------------

template <typename T>
template <typename TValue>
unsigned
ArrayView<T>::operator << (const TValue& value) {
	assert(_count < _capacity);

	new (&get(_count)) Item(value);

	return _count++;
}

//··············································································

template <typename T>
template <typename TValue>
unsigned
ArrayView<T>::operator << (TValue&& value) {
	assert(_count < _capacity);

	new (&get(_count)) Item(std::move(value));

	return _count++;
}

//------------------------------------------------------------------------------

template <typename T>
T&
ArrayView<T>::get(const unsigned i) {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

//··············································································

template <typename T>
const T&
ArrayView<T>::get(const unsigned i) const {
	assert(0 <= i && i < _capacity);

	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

}
}
