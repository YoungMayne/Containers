#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "utility.h"
#include "iterators/reverse_iterator.h"


namespace yo {


	template<typename T, size_t _capacity = 2>
	class vector {
	public:
		typedef T                                                             value;
		typedef T&                                                            reference;
		typedef const T&                                                      const_reference;
		typedef T*                                                            iterator;
		typedef const T*                                                      const_iterator;
		typedef reverse_iterator<T, const T*, const T&, const_iterator>       const_reverse_iterator;
		typedef reverse_iterator<T, T*, T&, iterator>                         reverse_iterator;
	public:
		vector                         ();
		vector                         (const std::initializer_list<T>& list) noexcept;
		vector                         (size_t count, const T& item)          noexcept;
		vector                         (const vector<T>& other)               noexcept;
		vector                         (const T& item)                        noexcept;

		~vector                        ()                                     noexcept;

		void push_back                 (const T& item)                        noexcept;
		reference push_back            ()                                     noexcept;
		void push_back_uinitialized    ()                                     noexcept;

		void pop_back                  ();
		void clear                     ()                                     noexcept;
		void resize                    (size_t newsize)                       noexcept;
		void reserve                   (size_t newcap)                        noexcept;
		void reverse                   ()                                     noexcept;
		void emplace                   (const iterator& it, const T& item)    noexcept;
		void swap                      (vector<T>& other)                     noexcept;
		void shrink_to_fit             ()                                     noexcept;

		iterator insert                (const_iterator pos, const T& item)    noexcept;
		iterator insert                (const_iterator pos, size_t count,
			                            const T& item)                        noexcept;
		iterator insert                (const_iterator pos,
			                            const vector<T>& other)               noexcept;
		template<typename InputIterator>
		iterator insert                (const_iterator pos, 
			                            InputIterator first, 
			                            const InputIterator& last)            noexcept;

		iterator erase                 (const_iterator pos);
		iterator erase                 (const_iterator first, 
			                            const_iterator last);

		iterator erase_all             (const T& item)                        noexcept;
		iterator erase_all             (const_iterator first, 
			                            const_iterator last, const T& item)   noexcept;

		iterator erase_first           (const T& item)                        noexcept;
		iterator erase_first           (const_iterator first, 
			                            const_iterator last, 
			                            const T& item)                        noexcept;

		iterator begin                 ()                                     noexcept;
		const_iterator begin           ()const                                noexcept;
		const_iterator cbegin          ()const                                noexcept;
		reverse_iterator rbegin        ()                                     noexcept;
		const_reverse_iterator rbegin  ()const                                noexcept;
		const_reverse_iterator crbegin ()const                                noexcept;

		iterator end                   ()                                     noexcept;
		const_iterator end             ()const                                noexcept;
		const_iterator cend            ()const                                noexcept;
		reverse_iterator rend          ()                                     noexcept;
		const_reverse_iterator rend    ()const                                noexcept;
		const_reverse_iterator crend   ()const                                noexcept;

		bool empty                     ()const                                noexcept;

		size_t size                    ()const                                noexcept;
		size_t capacity                ()const                                noexcept;

		reference front                ();
		reference back                 ();
		const_reference front          ()const;
		const_reference back           ()const;

		T* data                        ()                                     noexcept;
		const T* data                  ()const                                noexcept;

		reference at                   (const size_t& pos);
		const_reference at             (const size_t& pos)const;

		reference operator[]           (const size_t& pos);
		const_reference operator[]     (const size_t& pos)const;

		vector<T>& operator=           (const vector<T>& other)               noexcept;
	protected:
		size_t SIZE;
		size_t CAP;

		T*     elems;
	private:
		void replace_elems             (size_t _oldsize, size_t _newsize)     noexcept;
		void calculate_new_cap         ()                                     noexcept;
	};


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::vector() {
		YO_ASSERT_THROW(_capacity < 1, "Capacity can not be less than 1!");
		SIZE = 0;
		elems = new T[CAP = _capacity];
	}


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::vector(const std::initializer_list<T>& list) noexcept : vector() {
		size_t index = 0;
		resize(list.size());
		for (const T& item : list) {
			elems[index++] = item;
		}
	}


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::vector(size_t count, const T& item) noexcept : vector() {
		resize(count);
		while (count > 0) {
			elems[--count] = item;
		}
	}


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::vector(const vector<T>& other) noexcept : vector() {
		*this = other;
	}


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::vector(const T& item) noexcept : vector() {
		push_back(item);
	}


	template<typename T, size_t _capacity>
	inline vector<T, _capacity>::~vector() noexcept {
		delete[] elems;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::push_back(const T& item) noexcept {
		calculate_new_cap();
		elems[SIZE++] = item;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reference vector<T, _capacity>::push_back() noexcept	{
		calculate_new_cap();
		return elems[SIZE++];
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::push_back_uinitialized() noexcept	{
		calculate_new_cap();
		++SIZE;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::pop_back() {
		YO_ASSERT_THROW(SIZE-- == 0, "Empty vector");
		calculate_new_cap();
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::clear() noexcept {
		replace_elems(SIZE = 0, CAP = _capacity);
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::resize(size_t newsize) noexcept {
		CAP = newsize < _capacity ? _capacity : newsize;
		replace_elems(SIZE < newsize ? SIZE : newsize, CAP);
		SIZE = newsize;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::reserve(size_t newcap) noexcept {
		if (newcap > CAP) {
			replace_elems(SIZE, CAP = newcap);
		}
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::reverse() noexcept {
		for (size_t i = 0, j = empty() ? SIZE : SIZE - 1; i < j; ++i, --j) {
			swap(elems[i], elems[j]);
		}
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::emplace(const iterator& it, const T& item) noexcept {
		*it = item;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::swap(vector<T>& other) noexcept {
		yo::swap(CAP, other.CAP);
		yo::swap(SIZE, other.SIZE);
		yo::swap(elems, other.elems);
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::shrink_to_fit() noexcept {
		if (CAP != SIZE) {
			replace_elems(SIZE, CAP = SIZE < _capacity ? _capacity : SIZE);
		}
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::insert(const_iterator pos, const T& item) noexcept {
		if (empty()) { push_back(item); return end() - 1; }

		T last = back();
		size_t diff = pos - elems;
		for (auto i = end() - 1; i != pos; --i) {
			*i = *(i - 1);
		}
		*(elems + diff) = item;

		push_back(last);
		return elems + diff;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::insert(const_iterator pos, size_t count, const T& item) noexcept {
		while (count-- > 0) {
			pos = insert(pos, item);
		}
		return (iterator)pos;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::insert(const_iterator pos, const vector<T>& other) noexcept {
		for (const auto& item : other) {
			pos = insert(pos, item);
		}
		return (iterator)pos;
	}


	template<typename T, size_t _capacity>
	template<typename InputIterator>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::insert(const_iterator pos, InputIterator first, const InputIterator& last) noexcept {
		while (first != last) {
			pos = insert(pos, *first++);
		}
		return (iterator)pos;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase(const_iterator pos) {
		YO_ASSERT_THROW(pos == end(), "Incorrect position");

		size_t diff = pos - elems;

		for (auto i = (iterator)pos; i != end(); ++i) {
			*i = *(i + 1);
		}
		pop_back();

		return elems + diff;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase(const_iterator first, const_iterator last) {
		YO_ASSERT_THROW(first == end(), "Incorrect position");
		while (first != last) {
			size_t diff = last - elems;
			first = erase(first);
			last = elems + diff - 1;
		}
		return (iterator)first;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase_all(const T& item) noexcept	{
		return erase_all(begin(), end(), item);
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase_all(const_iterator first, const_iterator last, const T& item) noexcept {
		size_t diff = last - elems;
		iterator result = (iterator)last;
		iterator i = erase_first(first, last, item);

		while (i != last) {
			result = i;
			i = erase_first(i, last = elems + diff - 1, item);
			diff = last - elems;
		}

		return result;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase_first(const T& item) noexcept {
		return erase_first(begin(), end(), item);
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::erase_first(const_iterator first, const_iterator last, const T& item) noexcept {
		iterator i = (iterator)yo::find_first(first, last, item);

		if (i != end()) {
			return erase(i);
		}

		return i;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::begin() noexcept {
		return &elems[0];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_iterator vector<T, _capacity>::begin()const  noexcept {
		return &elems[0];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_iterator vector<T, _capacity>::cbegin()const  noexcept {
		return &elems[0];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reverse_iterator vector<T, _capacity>::rbegin() noexcept {
		return end();
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reverse_iterator vector<T, _capacity>::rbegin() const noexcept {
		return cend();
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reverse_iterator vector<T, _capacity>::crbegin() const noexcept {
		return cend();
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::iterator vector<T, _capacity>::end() noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_iterator vector<T, _capacity>::end()const noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_iterator vector<T, _capacity>::cend()const noexcept {
		return &elems[SIZE];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reverse_iterator vector<T, _capacity>::rend() noexcept {
		return begin();
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reverse_iterator vector<T, _capacity>::rend() const noexcept {
		return cbegin();
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reverse_iterator vector<T, _capacity>::crend() const noexcept {
		return cbegin();
	}


	template<typename T, size_t _capacity>
	inline bool vector<T, _capacity>::empty() const noexcept {
		return SIZE == 0;
	}


	template<typename T, size_t _capacity>
	inline size_t vector<T, _capacity>::size() const noexcept {
		return SIZE;
	}


	template<typename T, size_t _capacity>
	inline size_t vector<T, _capacity>::capacity() const noexcept {
		return CAP;
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reference vector<T, _capacity>::front() {
		YO_ASSERT_THROW(SIZE == 0, "Empty vector");
		return elems[0];
	}

	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reference vector<T, _capacity>::back() {
		YO_ASSERT_THROW(SIZE == 0, "Empty vector");
		return elems[SIZE - 1];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reference vector<T, _capacity>::front() const {
		YO_ASSERT_THROW(SIZE == 0, "Empty vector");
		return elems[0];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reference vector<T, _capacity>::back() const {
		YO_ASSERT_THROW(SIZE == 0, "Empty vector");
		return elems[SIZE - 1];
	}


	template<typename T, size_t _capacity>
	inline T* vector<T, _capacity>::data() noexcept {
		return &elems[0];
	}


	template<typename T, size_t _capacity>
	inline const T* vector<T, _capacity>::data() const noexcept {
		return &elems[0];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reference vector<T, _capacity>::at(const size_t& pos) {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reference vector<T, _capacity>::at(const size_t& pos) const {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::reference vector<T, _capacity>::operator[](const size_t& pos) {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t _capacity>
	inline typename vector<T, _capacity>::const_reference vector<T, _capacity>::operator[](const size_t& pos) const {
		YO_ASSERT_THROW(pos >= SIZE, "Out of range");
		return elems[pos];
	}


	template<typename T, size_t _capacity>
	inline vector<T>& vector<T, _capacity>::operator=(const vector<T>& other) noexcept {
		if (this != &other) {
			resize(other.size());
			for (size_t i = 0; i < SIZE; ++i) {
				elems[i] = other[i];
			}
		}
		return *this;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::replace_elems(size_t _oldsize, size_t _newsize) noexcept {
		T* _new = new T[_newsize];
		for (size_t i = 0; i < _oldsize; ++i) {
			_new[i] = elems[i];
		}
		delete[] elems;
		elems = _new;
	}


	template<typename T, size_t _capacity>
	inline void vector<T, _capacity>::calculate_new_cap() noexcept {
		size_t new_cap = CAP * 0.5;
		if (SIZE <= new_cap) {
			replace_elems(SIZE, CAP = new_cap < _capacity ? _capacity : new_cap);
		}
		else if (SIZE >= CAP) {
			replace_elems(SIZE, CAP *= 2);
		}
	}



	template<typename T>
	inline bool operator==(const vector<T>& left, const vector<T>& right) {
		if (yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0) {
			return left.size() == right.size();
		}
		return false;
	}


	template<typename T>
	inline bool operator!=(const vector<T>& left, const vector<T>& right) {
		return !(left == right);
	}


	template<typename T>
	inline bool operator>(const vector<T>& left, const vector<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 1;
	}


	template<typename T>
	inline bool operator<(const vector<T>& left, const vector<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == -1;
	}


	template<typename T>
	inline bool operator>=(const vector<T>& left, const vector<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) >= 0;
	}


	template<typename T>
	inline bool operator<=(const vector<T>& left, const vector<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) <= 0;
	}


	template<typename T>
	void swap(vector<T>& left, vector<T>& right) {
		left.swap(right);
	}


}
