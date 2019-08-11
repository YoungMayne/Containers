#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "iterators/reverse_iterator.h"


namespace yo {


	template<typename T, size_t SIZE>
	class array {
	public:
		typedef T                                                              value;
		typedef value&                                                         reference;
		typedef const value&                                                   const_reference;
		typedef value*                                                         iterator;
		typedef const value*                                                   const_iterator;
		typedef reverse_iterator<T, const T*, const T&, const_iterator>        const_reverse_iterator;
		typedef reverse_iterator<T, T*, T&, iterator>                          reverse_iterator;
	public:
		array                          ()                                      noexcept;
		array                          (const std::initializer_list<T>& list);
		array                          (const T& item)                         noexcept;
		template<typename Container = array<T, SIZE>>
		array                          (const Container& other);

		void fill                      (const T& item)                         noexcept;
		void reverse                   ()                                      noexcept;
		void emplace                   (iterator pos, const T& item)           noexcept;
		void swap                      (array<T, SIZE>& other)                 noexcept;
		
		iterator begin                 ()                                      noexcept;
		const_iterator begin           ()const                                 noexcept;
		const_iterator cbegin          ()const                                 noexcept;
		reverse_iterator rbegin        ()                                      noexcept;
		const_reverse_iterator rbegin  ()const                                 noexcept;
		const_reverse_iterator crbegin ()const                                 noexcept;

		iterator end                   ()                                      noexcept;
		const_iterator end             ()const                                 noexcept;
		const_iterator cend            ()const                                 noexcept;
		reverse_iterator rend          ()                                      noexcept;
		const_reverse_iterator rend    ()const                                 noexcept;
		const_reverse_iterator crend   ()const                                 noexcept;

		T* data                        ()                                      noexcept;
		const T* data                  ()const                                 noexcept;

		bool empty                     ()const                                 noexcept;

		size_t size                    ()const                                 noexcept;
		size_t max_size                ()const                                 noexcept;

		reference front                ();
		reference back                 ();
		const_reference front          ()const;
		const_reference back           ()const;

		reference random               ();
		const_reference random         ()const;

		reference at                   (const size_t& pos);
		const_reference at             (const size_t& pos)const;

		reference operator[]           (const size_t n);
		const_reference operator[]     (const size_t n)const;

		template<typename Container = array<T, SIZE>>
		array<T, SIZE>& operator=      (const Container& other);
	protected:
		T elems[SIZE == 0 ? 1 : SIZE];
	};


	template<typename T, size_t SIZE>
	inline array<T, SIZE>::array() noexcept {}


	template<typename T, size_t SIZE>
	inline array<T, SIZE>::array(const std::initializer_list<T>& list) {
		YO_ASSERT_THROW(list.size() > SIZE, "Out of bounds");
		size_t index = 0;
		for (const T& elem : list) {
			elems[index++] = elem;
		}	
	}


	template<typename T, size_t SIZE>
	template<typename Container>
	inline array<T, SIZE>::array(const Container& other) {
		YO_ASSERT_THROW(other.size() > SIZE, "Out of bounds");
		*this = other;
	}


	template<typename T, size_t SIZE>
	inline array<T, SIZE>::array(const T& item) noexcept {
		fill(item);
	}


	template<typename T, size_t SIZE>
	inline void array<T, SIZE>::fill(const T& item) noexcept {
		for (size_t i = 0; i < SIZE; ++i) {
			elems[i] = item;
		}
	}


	template<typename T, size_t SIZE>
	inline void array<T, SIZE>::reverse() noexcept {
		for (size_t i = 0, j = empty() ? SIZE : SIZE - 1; i < j; ++i, --j) {
			T temp = elems[i];
			elems[i] = elems[j];
			elems[j] = temp;
		}
	}


	template<typename T, size_t SIZE>
	inline void array<T, SIZE>::emplace(iterator it, const T& item) noexcept {
		*it = item;
	}


	template<typename T, size_t SIZE>
	inline void array<T, SIZE>::swap(array<T, SIZE>& other) noexcept {
		for (size_t i = 0; i < SIZE; ++i) {
			yo::swap(elems[i], other[i]);
		}
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::iterator array<T, SIZE>::begin() noexcept {
		return &elems[0];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_iterator array<T, SIZE>::begin() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_iterator array<T, SIZE>::cbegin() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::reverse_iterator array<T, SIZE>::rbegin() noexcept {
		return end();
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reverse_iterator array<T, SIZE>::rbegin() const noexcept {
		return cend();
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reverse_iterator array<T, SIZE>::crbegin() const noexcept {
		return cend();
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::iterator array<T, SIZE>::end() noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_iterator array<T, SIZE>::end() const noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_iterator array<T, SIZE>::cend() const noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::reverse_iterator array<T, SIZE>::rend() noexcept {
		return begin();
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reverse_iterator array<T, SIZE>::rend() const noexcept {
		return cbegin();
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reverse_iterator array<T, SIZE>::crend() const noexcept {
		return cbegin();
	}


	template<typename T, size_t SIZE>
	inline T* array<T, SIZE>::data() noexcept {
		return &elems[0];
	}


	template<typename T, size_t SIZE>
	inline const T* array<T, SIZE>::data() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t SIZE>
	inline bool array<T, SIZE>::empty() const noexcept {
		return SIZE == 0;
	}


	template<typename T, size_t SIZE>
	inline size_t array<T, SIZE>::size() const noexcept {
		return SIZE;
	}


	template<typename T, size_t SIZE>
	inline size_t array<T, SIZE>::max_size() const noexcept {
		return SIZE;
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::reference array<T, SIZE>::front() {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[0];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::reference array<T, SIZE>::back() {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[SIZE - 1];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reference array<T, SIZE>::front() const {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[0];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reference array<T, SIZE>::back() const {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[SIZE - 1];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::reference array<T, SIZE>::random() {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[yo::random<size_t>(0, size() - 1)];
	}


	template<typename T, size_t SIZE>
	inline typename array<T, SIZE>::const_reference array<T, SIZE>::random() const {
		YO_ASSERT_THROW(SIZE == 0, "Empty array");
		return elems[yo::random<size_t>(0, size() - 1)];
	}


	template<typename T, size_t SIZE>
	inline  typename array<T, SIZE>::reference array<T, SIZE>::at(const size_t& pos) {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t SIZE>
	inline  typename array<T, SIZE>::const_reference array<T, SIZE>::at(const size_t& pos) const {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t SIZE>
	inline  typename array<T, SIZE>::reference array<T, SIZE>::operator[](const size_t n) {
		YO_ASSERT_THROW(n >= SIZE, "Out of range");
		return elems[n];
	}


	template<typename T, size_t SIZE>
	inline  typename array<T, SIZE>::const_reference array<T, SIZE>::operator[](const size_t n) const {
		YO_ASSERT_THROW(n >= SIZE, "Out of range");
		return elems[n];
	}


	template<typename T, size_t SIZE>
	template<typename Container>
	inline array<T, SIZE>& array<T, SIZE>::operator=(const Container& other) {
		YO_ASSERT_THROW(other.size() > SIZE, "Out of bounds");
		size_t index = 0;
		for (const auto& item : other) {
			elems[index++] = item;
		}
		return *this;
	}



	template<typename T, size_t size>
	inline bool operator==(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0;
	}


	template<typename T, size_t size>
	inline bool operator!=(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) != 0;
	}


	template<typename T, size_t size>
	inline bool operator>(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 1;
	}


	template<typename T, size_t size>
	inline bool operator<(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == -1;
	}


	template<typename T, size_t size>
	inline bool operator>=(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) >= 0;
	}


	template<typename T, size_t size>
	inline bool operator<=(const array<T, size>& left, const array<T, size>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) <= 0;
	}


	template<typename T, size_t size>
	void swap(array<T, size>& left, array<T, size>& right) {
		left.swap(right);
	}


}
