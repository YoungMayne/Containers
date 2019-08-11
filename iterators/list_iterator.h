#pragma once


namespace yo {


	template<typename T, typename Pointer, typename Reference>
	class list_iterator {
	public:
		typedef T                                      value;
		typedef Pointer                                pointer;
		typedef Reference                              reference;
		typedef list_iterator<T, Pointer, Reference>   iterator;
	public:
		list_iterator        ()                        noexcept;
		list_iterator        (const pointer ptr)       noexcept;
		list_iterator        (const iterator& it)      noexcept;

		iterator& operator++ ()                        noexcept;
		iterator  operator++ (int)                     noexcept;

		iterator& operator-- ()                        noexcept;
		iterator  operator-- (int)                     noexcept;

		reference operator*  ()const                   noexcept;
		value*    operator-> ()const                   noexcept;

		bool operator==      (const iterator& it)const noexcept;
		bool operator!=      (const iterator& it)const noexcept;

		iterator& operator=  (const iterator& it)      noexcept;
	public:
		pointer              ptr;
	};


	template<typename T, typename Pointer, typename Reference>
	inline list_iterator<T, Pointer, Reference>::list_iterator() noexcept {}


	template<typename T, typename Pointer, typename Reference>
	inline list_iterator<T, Pointer, Reference>::list_iterator(const pointer ptr) noexcept {
		this->ptr = ptr;
	}


	template<typename T, typename Pointer, typename Reference>
	inline list_iterator<T, Pointer, Reference>::list_iterator(const iterator& it) noexcept {
		this->ptr = it.ptr;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::iterator& list_iterator<T, Pointer, Reference>::operator++() noexcept {
		this->ptr = ptr->next;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::iterator list_iterator<T, Pointer, Reference>::operator++(int) noexcept {
		iterator temp(*this);
		operator++();
		return temp;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::iterator& list_iterator<T, Pointer, Reference>::operator--() noexcept	{
		ptr = ptr->prev;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::iterator list_iterator<T, Pointer, Reference>::operator--(int) noexcept	{
		iterator temp(*this);
		operator--();
		return temp;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::reference list_iterator<T, Pointer, Reference>::operator*() const noexcept {
		return ptr->item;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::value* list_iterator<T, Pointer, Reference>::operator->() const noexcept	{
		return &this->operator*();
	}


	template<typename T, typename Pointer, typename Reference>
	inline bool list_iterator<T, Pointer, Reference>::operator==(const iterator& it) const noexcept {
		return ptr == it.ptr;
	}


	template<typename T, typename Pointer, typename Reference>
	inline bool list_iterator<T, Pointer, Reference>::operator!=(const iterator& it) const noexcept	{
		return ptr != it.ptr;
	}


	template<typename T, typename Pointer, typename Reference>
	inline typename list_iterator<T, Pointer, Reference>::iterator& list_iterator<T, Pointer, Reference>::operator=(const iterator& it) noexcept {
		this->ptr = it.ptr;
		return *this;
	}


}
