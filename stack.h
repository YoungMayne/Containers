#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "utility.h"
#include "iterators/reverse_iterator.h"


namespace yo {


	template<typename T, size_t MAX_SIZE = 100>
	class stack {
	public:
		typedef T                                                             value;
		typedef T*                                                            pointer;
		typedef const T*                                                      const_pointer;
		typedef const T*                                                      iterator;
		typedef T&                                                            reference;
		typedef const T&                                                      const_reference;
		typedef reverse_iterator<value, pointer, const_reference, iterator>   reverse_iterator;
	public:
		stack                     ()                                          noexcept;
		stack                     (const std::initializer_list<value>& list);
		stack                     (const value& item);

		template<typename Container = stack<T, MAX_SIZE>>
		stack                     (const Container& other);

		void push                 (const T& item);
		reference push            ();

		void swap                 (stack<T, MAX_SIZE>& other)                 noexcept;
		void clear                ()                                          noexcept;

		const_reference pop       ();

		reference top             ()                                          noexcept;
		const_reference top       ()const                                     noexcept;

		const_pointer data        ()const                                     noexcept;

		iterator begin            ()const                                     noexcept;
		iterator end              ()const                                     noexcept;

		reverse_iterator rbegin   ()const                                     noexcept;
		reverse_iterator rend     ()const                                     noexcept;

		size_t size               ()const                                     noexcept;
		size_t max_size           ()const                                     noexcept;

		bool empty                ()const                                     noexcept;

		template<typename Container = stack<T, MAX_SIZE>>
		stack<T, MAX_SIZE>& 
		                operator= (const Container& other);
	protected:
		size_t  SIZE;
		value   elems[MAX_SIZE == 0 ? 1 : MAX_SIZE];
	};


	template<typename T, size_t MAX_SIZE>
	inline stack<T, MAX_SIZE>::stack() noexcept {
		SIZE = 0;
	}


	template<typename T, size_t MAX_SIZE>
	inline stack<T, MAX_SIZE>::stack(const std::initializer_list<value>& list) : stack() {
		YO_ASSERT_THROW(list.size() > MAX_SIZE, "Out of bounds");
		for (const auto& item : list) {
			elems[SIZE++] = item;
		}
	}


	template<typename T, size_t MAX_SIZE>
	template<typename Container>
	inline stack<T, MAX_SIZE>::stack(const Container& other) {
		YO_ASSERT_THROW(other.size() > MAX_SIZE, "Out of bounds");
		*this = other;
	}


	template<typename T, size_t MAX_SIZE>
	inline stack<T, MAX_SIZE>::stack(const value& item)	: stack() {
		elems[SIZE++] = item;
	}


	template<typename T, size_t MAX_SIZE>
	inline void stack<T, MAX_SIZE>::push(const value& item) {
		YO_ASSERT_THROW(size() >= MAX_SIZE, "Out of bounds");
		elems[SIZE++] = item;
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::reference yo::stack<T, MAX_SIZE>::push() {
		YO_ASSERT_THROW(size() >= MAX_SIZE, "Out of bounds");
		return elems[SIZE++];
	}


	template<typename T, size_t MAX_SIZE>
	inline void stack<T, MAX_SIZE>::swap(stack<T, MAX_SIZE>& other) noexcept {
		size_t length = size() > other.size() ? size() : other.size();
		for (size_t i = 0; i < length; ++i) {
			yo::swap(elems[i], other.elems[i]);
		}
		yo::swap(SIZE, other.SIZE);
	}


	template<typename T, size_t MAX_SIZE>
	inline void stack<T, MAX_SIZE>::clear() noexcept {
		SIZE = 0;
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::const_reference yo::stack<T, MAX_SIZE>::pop() {
		YO_ASSERT_THROW(empty(), "Empty stack");
		return elems[--SIZE];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::reference yo::stack<T, MAX_SIZE>::top() noexcept {
		return elems[SIZE - 1];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::const_reference yo::stack<T, MAX_SIZE>::top() const noexcept {
		return elems[SIZE - 1];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::const_pointer stack<T, MAX_SIZE>::data() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::iterator stack<T, MAX_SIZE>::begin() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::iterator stack<T, MAX_SIZE>::end() const noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::reverse_iterator stack<T, MAX_SIZE>::rbegin() const noexcept	{
		return &elems[SIZE];
	}


	template<typename T, size_t MAX_SIZE>
	inline typename stack<T, MAX_SIZE>::reverse_iterator stack<T, MAX_SIZE>::rend() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t MAX_SIZE>
	inline size_t stack<T, MAX_SIZE>::size() const noexcept {
		return SIZE;
	}


	template<typename T, size_t MAX_SIZE>
	inline size_t stack<T, MAX_SIZE>::max_size() const noexcept {
		return MAX_SIZE;
	}


	template<typename T, size_t MAX_SIZE>
	inline bool stack<T, MAX_SIZE>::empty() const noexcept {
		return SIZE == 0;
	}


	template<typename T, size_t MAX_SIZE>
	template<typename Container>
	inline stack<T, MAX_SIZE>& stack<T, MAX_SIZE>::operator=(const Container& other) {
		YO_ASSERT_THROW(other.size() > MAX_SIZE, "Out of bounds");
		SIZE = 0;
		for (const auto& item : other) {
			elems[SIZE++] = item;
		}
		return *this;
	}



	template<typename T, size_t MAX_SIZE>
	inline bool operator==(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		if (yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0) {
			return left.size() == right.size();
		}
		return false;
	}


	template<typename T, size_t MAX_SIZE>
	inline bool operator!=(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		return !(left == right);
	}


	template<typename T, size_t MAX_SIZE>
	inline bool operator>(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 1;
	}


	template<typename T, size_t MAX_SIZE>
	inline bool operator<(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == -1;
	}


	template<typename T, size_t MAX_SIZE>
	inline bool operator>=(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		short result = yo::compare(left.begin(), right.begin(), left.end(), right.end());
		if (result == 0) {
			return left.size() == right.size();
		}
		return result == 1;
	}


	template<typename T, size_t MAX_SIZE>
	inline bool operator<=(const stack<T, MAX_SIZE>& left, const stack<T, MAX_SIZE>& right) {
		short result = yo::compare(left.begin(), right.begin(), left.end(), right.end());
		if (result == 0) {
			return left.size() == right.size();
		}
		return result == -1;
	}


	template<typename T, size_t MAX_SIZE>
	void swap(stack<T, MAX_SIZE>& left, stack<T, MAX_SIZE>& right) {
		left.swap(right);
	}


}
