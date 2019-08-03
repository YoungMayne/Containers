#pragma once


namespace yo {


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	class reverse_iterator {
	public:
		typedef T                                                 value;
		typedef Pointer                                           pointer;
		typedef Reference                                         reference;
		typedef Iterator                                          iterator_value;
		typedef reverse_iterator<T, Pointer, Reference, Iterator> iterator;
	public:
		reverse_iterator     ()                                   noexcept;
		reverse_iterator     (const iterator_value& it)               noexcept;
		reverse_iterator     (const iterator& it)                 noexcept;

		iterator& operator++ ()                                   noexcept;
		iterator  operator++ (int)                                noexcept;

		iterator& operator-- ()                                   noexcept;
		iterator  operator-- (int)                                noexcept;

#define OVERRIDED  template<typename size_t> iterator
		OVERRIDED  operator+  (const size_t value)const           noexcept;
		OVERRIDED& operator+= (const size_t value)                noexcept;
		OVERRIDED  operator-  (const size_t value)const           noexcept;
		OVERRIDED& operator-= (const size_t value)                noexcept;
#undef OVERRIDED

		reference operator*  ()const                              noexcept;
		value* operator->    ()const                              noexcept;

		bool operator==      (const iterator& it)const            noexcept;
		bool operator!=      (const iterator& it)const            noexcept;

		iterator& operator=  (const iterator& it)                 noexcept;
	public:
		iterator_value       ptr;
	};


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline yo::reverse_iterator<T, Pointer, Reference, Iterator>::reverse_iterator() noexcept {	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline yo::reverse_iterator<T, Pointer, Reference, Iterator>::reverse_iterator(const iterator_value& it) noexcept {
		this->ptr = it;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline yo::reverse_iterator<T, Pointer, Reference, Iterator>::reverse_iterator(const iterator& it) noexcept {
		this->ptr = it.ptr;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator& reverse_iterator<T, Pointer, Reference, Iterator>::operator++() noexcept {
		--ptr;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator reverse_iterator<T, Pointer, Reference, Iterator>::operator++(int) noexcept {
		iterator temp(*this);
		++(*this);
		return temp;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator& reverse_iterator<T, Pointer, Reference, Iterator>::operator--() noexcept {
		++ptr;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator reverse_iterator<T, Pointer, Reference, Iterator>::operator--(int) noexcept {
		iterator temp(*this);
		--(*this);
		return temp;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	template<typename size_t>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator reverse_iterator<T, Pointer, Reference, Iterator>::operator+(const size_t value) const noexcept {
		return ptr - value;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	template<typename size_t>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator& reverse_iterator<T, Pointer, Reference, Iterator>::operator+=(const size_t value) noexcept {
		ptr -= value;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	template<typename size_t>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator reverse_iterator<T, Pointer, Reference, Iterator>::operator-(const size_t value) const noexcept {
		return ptr + value;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	template<typename size_t>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator& reverse_iterator<T, Pointer, Reference, Iterator>::operator-=(const size_t value) noexcept {
		ptr += value;
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::reference reverse_iterator<T, Pointer, Reference, Iterator>::operator*() const noexcept {
		return *ptr;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::value* reverse_iterator<T, Pointer, Reference, Iterator>::operator->() const noexcept {
		return &(*ptr);
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline bool reverse_iterator<T, Pointer, Reference, Iterator>::operator==(const iterator& it) const noexcept {
		return ptr == it.ptr;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline bool reverse_iterator<T, Pointer, Reference, Iterator>::operator!=(const iterator& it) const noexcept {
		return ptr != it.ptr;
	}


	template<typename T, typename Pointer, typename Reference, typename Iterator>
	inline typename reverse_iterator<T, Pointer, Reference, Iterator>::iterator& reverse_iterator<T, Pointer, Reference, Iterator>::operator=(const iterator& it) noexcept {
		this->ptr = it.ptr;
		return *this;
	}


}
