#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "utility.h"
#include "iterators/list_iterator.h"
#include "iterators/reverse_iterator.h"


namespace yo {


	template<typename T>
	struct ListNode {
		ListNode* next;
		ListNode* prev;
		T		  item;

		ListNode() noexcept
		{ next = prev = nullptr; }

		ListNode(const T& item) noexcept
		:ListNode() { this->item = item; }
	};


	template <typename T>
	class list {
	public:
		typedef T                                                             value;
		typedef T&                                                            reference;
		typedef const T&                                                      const_reference;
		typedef ListNode<value>                                               Node;
		typedef Node*                                                         pointer_type;
		typedef list_iterator<T, pointer_type, T&>                            iterator;
		typedef list_iterator<const T, pointer_type, const T&>                const_iterator;
		typedef reverse_iterator<T, pointer_type, const T&, const_iterator>   const_reverse_iterator;
		typedef reverse_iterator<T, pointer_type, T&, iterator>               reverse_iterator;
	public:
		list                           ()                                     noexcept;
		list                           (const std::initializer_list<T>& list) noexcept;
		list                           (size_t count, const T& item)          noexcept;
		list                           (const list<T>& other)                 noexcept;
		list                           (const T& item)                        noexcept;

		~list                          ()                                     noexcept;

		void push_back                 (const T& item)					      noexcept;
		reference push_back            ()                                     noexcept;
		void push_back_uninitialized   ()                                     noexcept;

		void push_front                (const T& item)			              noexcept;
		reference push_front           ()                                     noexcept;
		void push_front_uninitialized  ()                                     noexcept;

		void pop_back                  ();
		void pop_front                 ();
		void fill                      (const T& item)                        noexcept;
		void clear                     ()                                     noexcept;
		void emplace                   (iterator pos, const T& item)          noexcept;
		void swap                      (list<value>& other)                   noexcept;

		iterator insert                (const iterator& pos, const T& item)   noexcept;
		iterator insert                (const iterator& pos, size_t count,
			                            const T& item)                        noexcept;
		iterator insert                (const iterator& pos, 
										const list<T>& other)                 noexcept;
		template<typename InputIterator>
		iterator insert                (const iterator& pos, 
									    InputIterator first, 
										const InputIterator& last)            noexcept;

		iterator erase                 (iterator pos);
		iterator erase                 (iterator first,
										const iterator& last);

		iterator erase_all             (const T& item)                        noexcept;
		iterator erase_all             (const iterator& first,
								        const iterator& last,
								        const T& item)                        noexcept;

		iterator erase_first           (const T& item)                        noexcept;
		iterator erase_first           (const iterator& first, 
			                            const iterator& last,
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

		size_t size                    ()const                                noexcept;

		bool empty                     ()const                                noexcept;

		reference front                ();
		reference back                 ();
		const_reference front          ()const;
		const_reference back           ()const;
		
		list<T>& operator=             (const list<T>& other)                 noexcept;
	protected:
		pointer_type pFront;
		pointer_type pBack;
#ifdef YO_LIST_SIZE_CACHE
		size_t SIZE;
#endif
	private:
		static void tie                (pointer_type p1, pointer_type p2) noexcept;
	};


	template<typename T>
	inline list<T>::list() noexcept {
		pFront = pBack = new Node;
#ifdef YO_LIST_SIZE_CACHE
		SIZE = 0;
#endif
	}


	template<typename T>
	inline list<T>::list(const std::initializer_list<T>& list) noexcept : list() {
		for (const auto& item : list) {
			push_back() = item;
		}
	}


	template<typename T>
	inline list<T>::list(size_t count, const T& item) noexcept : list() {
		while (count-- > 0) {
			push_back() = item;
		}
	}


	template<typename T>
	inline list<T>::list(const list<T>& other) noexcept : list() {
		*this = other;
	}


	template<typename T>
	inline list<T>::list(const T& item) noexcept : list() {
		push_back() = item;
	}


	template<typename T>
	inline list<T>::~list() noexcept {
		while (pFront != pBack) {
			pFront = pFront->next;
			delete pFront->prev;
		}
		delete pBack;
	}


	template<typename T>
	inline void list<T>::push_back(const T& item) noexcept {
		push_back() = item;
	}


	template<typename T>
	inline typename list<T>::reference list<T>::push_back() noexcept {
		push_back_uninitialized();
		return pBack->prev->item;
	}


	template<typename T>
	inline void list<T>::push_back_uninitialized() noexcept	{
		tie(pBack, new Node);
		pBack = pBack->next;
#ifdef YO_LIST_SIZE_CACHE
		++SIZE;
#endif
	}


	template<typename T>
	inline void list<T>::push_front(const T& item) noexcept {
		push_front() = item;
	}


	template<typename T>
	inline typename list<T>::reference list<T>::push_front() noexcept	{
		push_front_uninitialized();
		return pFront->item;
	}


	template<typename T>
	inline void list<T>::push_front_uninitialized() noexcept {
		tie(new Node, pFront);
		pFront = pFront->prev;
#ifdef YO_LIST_SIZE_CACHE
		++SIZE;
#endif
	}


	template<typename T>
	inline void list<T>::pop_back() {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		pBack = pBack->prev;
		delete pBack->next;
#ifdef YO_LIST_SIZE_CACHE
		--SIZE;
#endif
	}


	template<typename T>
	inline void list<T>::pop_front() {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		pFront = pFront->next;
		delete pFront->prev;
#ifdef YO_LIST_SIZE_CACHE
		--SIZE;
#endif
	}


	template<typename T>
	inline void list<T>::fill(const T& item) noexcept {
		for (auto& elem : *this) {
			elem = item;
		}
	}


	template<typename T>
	inline void list<T>::clear() noexcept {
		while (pFront != pBack) {
			pFront = pFront->next;
			delete pFront->prev;
		}
#ifdef YO_LIST_SIZE_CACHE
		SIZE = 0;
#endif
	}


	template<typename T>
	inline void list<T>::emplace(iterator pos, const T& item) noexcept {
		*pos = item;
	}


	template<typename T>
	inline void list<T>::swap(list<T>& other) noexcept {
		yo::swap(pFront, other.pFront);
		yo::swap(pBack, other.pBack);
#ifdef YO_LIST_SIZE_CACHE
		yo::swap(SIZE, other.SIZE);
#endif
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::insert(const iterator& pos, const T& item) noexcept {
		if (pos.ptr == pFront) {
			push_front(item);
		}
		else {
			tie(pos.ptr->prev, new Node(item));
			tie(pos.ptr->prev->next, pos.ptr);
#ifdef YO_LIST_SIZE_CACHE
			++SIZE;
#endif
		}
		return pos.ptr->prev;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::insert(const iterator& pos, size_t count, const T& item) noexcept	{
		while (count-- > 0) {
			insert(pos, item);
		}
		return pos.ptr->prev;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::insert(const iterator& pos, const list<T>& other) noexcept	{
		for (const auto& item : other) {
			insert(pos, item);
		}
		return pos.ptr->prev;
	}


	template<typename T>
	template<typename InputIterator>
	inline typename list<T>::iterator list<T>::insert(const iterator& pos, InputIterator first, const InputIterator& last) noexcept {
		while (first != last) {
			insert(pos, *first++);
		}
		return pos.ptr->prev;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase(iterator pos) {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		iterator result = pos.ptr->next;
		if (pos.ptr == pFront) {
			pop_front();
		}
		else {
			tie(pos.ptr->prev, pos.ptr->next);
			delete pos.ptr;
#ifdef YO_LIST_SIZE_CACHE
			--SIZE;
#endif
		}
		return result;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase(iterator first, const iterator& last) {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		while (first != last) {
			first = erase(first);
		}
		return last.ptr;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase_all(const T& item) noexcept	{
		return erase_all(begin(), end(), item);
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase_all(const iterator& first, const iterator& last, const T& item) noexcept {
		iterator result = last;
		iterator i = erase_first(first, last, item);
		while (i != last) {
			result = i;
			i = erase_first(i, last, item);
		}
		return result;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase_first(const T& item) noexcept {
		return erase_first(begin(), end(), item);
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::erase_first(const iterator& first, const iterator& last, const T& item) noexcept {
		iterator i = yo::find_first(first, last, item);

		if (i != last) {
			return erase(i);
		}

		return i;
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::begin() noexcept {
		return pFront;
	}


	template<typename T>
	inline typename list<T>::const_iterator list<T>::begin() const noexcept {
		return pFront;
	}


	template<typename T>
	inline typename list<T>::const_iterator list<T>::cbegin() const noexcept {
		return pFront;
	}


	template<typename T>
	inline typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
		return end();
	}


	template<typename T>
	inline typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept {
		return end();
	}


	template<typename T>
	inline typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept {
		return end();
	}


	template<typename T>
	inline typename list<T>::iterator list<T>::end() noexcept {
		return pBack;
	}


	template<typename T>
	inline typename list<T>::const_iterator list<T>::end() const noexcept {
		return pBack;
	}


	template<typename T>
	inline typename list<T>::const_iterator list<T>::cend() const noexcept {
		return pBack;
	}


	template<typename T>
	inline typename list<T>::reverse_iterator list<T>::rend() noexcept {
		return begin();
	}


	template<typename T>
	inline typename list<T>::const_reverse_iterator list<T>::rend() const noexcept {
		return begin();
	}


	template<typename T>
	inline typename list<T>::const_reverse_iterator list<T>::crend() const noexcept {
		return begin();
	}


	template<typename T>
	inline size_t list<T>::size() const noexcept {
#ifdef YO_LIST_SIZE_CACHE
		return SIZE;
#else
		pointer_type temp = pFront;
		size_t length = 0;
		while (temp != pBack) {
			length += 1;
			temp = temp->next;
		}
		return length;
#endif
	}


	template<typename T>
	inline bool list<T>::empty() const noexcept {
		return pFront == pBack;
	}


	template<typename T>
	inline typename list<T>::reference list<T>::front() {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		return pFront->item;
	}


	template<typename T>
	inline typename list<T>::reference list<T>::back() {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		return pBack->prev->item;
	}


	template<typename T>
	inline typename list<T>::const_reference list<T>::front() const {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		return pFront->item;
	}


	template<typename T>
	inline typename list<T>::const_reference list<T>::back() const {
		YO_ASSERT_THROW(pFront == pBack, "Empty list");
		return pBack->prev->item;
	}


	template<typename T>
	inline list<T>& list<T>::operator=(const list<T>& other) noexcept {
		if (this != &other) {
			clear();
			for (const auto& item : other) {
				push_back(item);
			}
#ifdef YO_LIST_SIZE_CACHE
			SIZE = other.SIZE;
#endif
		}
		return *this;
	}


	template<typename T>
	inline void list<T>::tie(pointer_type p1, pointer_type p2) noexcept {
		p1->next = p2;
		p2->prev = p1;
	}



	template<typename T>
	inline bool operator==(const list<T>& left, const list<T>& right) {
		if (yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0) {
			return left.size() == right.size();
		}
		return false;
	}


	template<typename T>
	inline bool operator!=(const list<T>& left, const list<T>& right) {
		return !(left == right);
	}


	template<typename T>
	inline bool operator>(const list<T>& left, const list<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 1;
	}


	template<typename T>
	inline bool operator<(const list<T>& left, const list<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == -1;
	}


	template<typename T>
	inline bool operator>=(const list<T>& left, const list<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) >= 0;
	}


	template<typename T>
	inline bool operator<=(const list<T>& left, const list<T>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) <= 0;
	}


	template<typename T>
	void swap(list<T>& left, list<T>& right) {
		left.swap(right);
	}


}
