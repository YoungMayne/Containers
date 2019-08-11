#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "iterators/reverse_iterator.h"
#include "iterators/hash_iterator.h"
#include "utility.h"
#include "string.h"
#include "vector.h"
#include "list.h"


namespace yo {


	template<typename Key, typename Value>
	struct HashNode {
		Key   key;
		Value value;
	};


	template<typename Key, typename Value, size_t MIN_BSIZE = 10>
	class hashtable {
	public:
		typedef HashNode<Key, Value>                                                    node_type;
		typedef list<node_type>                                                         list_type;
		typedef vector<list_type, MIN_BSIZE>                                            container;
		typedef Value&                                                                  reference;
		typedef const Value&                                                            const_reference;
		typedef typename list_type::iterator                                            list_iterator;
		typedef typename list_type::const_iterator                                      const_list_iterator;

		typedef hash_iterator<node_type, typename list_iterator::pointer,
			    node_type&, list_type*, list_iterator>                                  iterator;

		typedef hash_iterator<node_type, typename const_list_iterator::pointer,
			    const node_type&, const list_type*, const_list_iterator>                const_iterator;

		typedef reverse_iterator<node_type, typename const_iterator::pointer,
			    const node_type&, const_iterator>                                       const_reverse_iterator;

		typedef reverse_iterator<node_type, typename iterator::pointer,
			    node_type&, iterator>                                                   reverse_iterator;
	public:
		hashtable                        ()                                             noexcept;
		hashtable                        (const std::initializer_list<node_type>& list) noexcept;
		hashtable                        (const hashtable<Key, Value>& other)           noexcept;
		hashtable                        (const node_type& item)                        noexcept;

		~hashtable                       ()                                             noexcept;

		iterator insert                  (const node_type& item)                        noexcept;
		iterator insert                  (const iterator& pos, const node_type& item)   noexcept;

		iterator erase                   (const iterator& pos)                          noexcept;
		iterator erase                   (const Key& key)                               noexcept;
		iterator erase                   (iterator first, const iterator& last)         noexcept;
		iterator erase                   (const iterator& first, 
			                              const iterator& last, const Key& key)         noexcept;

		iterator find                    (const Key& key)                               noexcept;
		const_iterator find              (const Key& key)const                          noexcept;

		void resize                      (size_t newsize)                               noexcept;
		void reset                       ()                                             noexcept;
		void clear                       ()                                             noexcept;
		void rehash                      ()                                             noexcept;
		void emplace                     (const iterator& pos, const node_type& item)   noexcept;
		void swap                        (hashtable<Key, Value>& other)                 noexcept;

		size_t bucket_size               ()const                                        noexcept;
		size_t elems_count               ()const                                        noexcept;

		iterator begin                   ()                                             noexcept;
		const_iterator begin             ()const                                        noexcept;
		const_iterator cbegin            ()const                                        noexcept;
		reverse_iterator rbegin          ()                                             noexcept;
		const_reverse_iterator rbegin    ()const                                        noexcept;
		const_reverse_iterator crbegin   ()const                                        noexcept;

		iterator end                     ()                                             noexcept;
		const_iterator end               ()const                                        noexcept;
		const_iterator cend              ()const                                        noexcept;
		reverse_iterator rend            ()                                             noexcept;
		const_reverse_iterator rend      ()const                                        noexcept;
		const_reverse_iterator crend     ()const                                        noexcept;

		bool empty                       ()const                                        noexcept;

		node_type& front                 ();
		node_type& back                  ();
		const node_type& front           ()const;
		const node_type& back            ()const;

		list_type* data                  ()                                             noexcept;
		const list_type* data            ()const                                        noexcept;

		reference at                     (const Key& key)                               noexcept;
		const_reference at               (const Key& key)const;

		reference operator[]             (const Key& key)                               noexcept;
		const_reference operator[]       (const Key& key)const;

		vector<node_type> items          ()const                                        noexcept;

		hashtable<Key, Value>& operator= (const hashtable<Key, Value>& other)           noexcept;
	public:
		container elems;
		size_t    limit;
	private:
		size_t hasher                    (size_t key)const                              noexcept;
		size_t hasher                    (const string& key)const                       noexcept;

		void calculate_new_bucket        (const list_type& list)                        noexcept;
	};


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value, MIN_BSIZE>::hashtable() noexcept {
		elems.resize(limit = MIN_BSIZE);
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value, MIN_BSIZE>::hashtable(const std::initializer_list<node_type>& list) noexcept : hashtable() {
		for (const auto& item : list) {
			insert(item);
		}
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value, MIN_BSIZE>::hashtable(const hashtable<Key, Value>& other) noexcept : hashtable() {
		*this = other;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value, MIN_BSIZE>::hashtable(const node_type& item) noexcept : hashtable() {
		insert(item);
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value, MIN_BSIZE>::~hashtable() noexcept {}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::insert(const node_type& item) noexcept {
		size_t i = hasher(item.key);

		if (find_first(elems[i].begin(), elems[i].end(), item) == elems[i].end()) {
			elems[i].push_back(item);
			calculate_new_bucket(elems[i]);
			return iterator(&elems[i], --elems[i].end(), elems.front().begin(), elems.back().end());
		}

		return end();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::insert(const iterator& pos, const node_type& item) noexcept {
		auto result = pos.container->insert(pos.it, item);
		return iterator(pos.container, result, elems.front().begin(), elems.back().end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::erase(const iterator& pos) noexcept {
		iterator newpos = pos.next();
		pos.container->erase(pos.it);
		return iterator(newpos.container, newpos.it, elems.front().begin(), elems.back().end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::erase(const Key& key) noexcept {
		return erase(begin(), end(), key);
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::erase(iterator first, const iterator& last) noexcept {
		while (first != last) {
			first = erase(first);
		}
		return last;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::erase(const iterator& first, const iterator& last, const Key& key) noexcept {
		iterator i = find_first<iterator, node_type>(first, last, { key, Value() });

		if (i != last) {
			return erase(i);
		}

		return last;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::find(const Key& key) noexcept {
		size_t i = hasher(key);
		auto it = find_first<list_iterator, node_type>(elems[i].begin(), elems[i].end(), { key, Value() });

		if (it == elems[i].end()) {
			return end();
		}

		return iterator(&elems[i], it, elems.front().begin(), elems.back().end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_iterator hashtable<Key, Value, MIN_BSIZE>::find(const Key& key) const noexcept	{
		size_t i = hasher(key);
		auto it = find_first<const_list_iterator, node_type>(elems[i].cbegin(), elems[i].cend(), { key, Value() });

		if (it == elems[i].cend()) {
			return cend();
		}

		return const_iterator(&elems[i], it, elems.front().cbegin(), elems.back().cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::resize(size_t newsize) noexcept {
		elems.resize(limit = newsize < MIN_BSIZE ? MIN_BSIZE : newsize);
		rehash();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::reset() noexcept {
		clear();
		elems.resize(limit = MIN_BSIZE);
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::clear() noexcept {
		for (auto& list : elems) {
			list.clear();
		}
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::rehash() noexcept	{
		yo::vector<node_type> nodes = items();
		clear();
		for (const auto& item : nodes) {
			insert(item);
		}
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::emplace(const iterator& pos, const node_type& item) noexcept {
		pos.container->emplace(pos.it, item);
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::swap(hashtable<Key, Value>& other) noexcept {
		if (this != &other) {
			auto first = items();
			auto second = other.items();

			this->clear();
			other.clear();

			for (const auto& item : second) {
				insert(item);
			}
			for (const auto& item : first) {
				other.insert(item);
			}
		}
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline size_t hashtable<Key, Value, MIN_BSIZE>::bucket_size() const noexcept {
		return elems.size();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline size_t hashtable<Key, Value, MIN_BSIZE>::elems_count() const noexcept {
		size_t size = 0;
		for (const auto& list : elems) {
			size += list.size();
		}
		return size;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::begin() noexcept {
		return iterator(&elems.front(), elems.back().end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_iterator hashtable<Key, Value, MIN_BSIZE>::begin() const noexcept {
		return const_iterator(&elems.front(), elems.back().cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_iterator hashtable<Key, Value, MIN_BSIZE>::cbegin() const noexcept	{
		return const_iterator(&elems.front(), elems.back().cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::reverse_iterator hashtable<Key, Value, MIN_BSIZE>::rbegin() noexcept {
		return reverse_iterator(end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reverse_iterator hashtable<Key, Value, MIN_BSIZE>::rbegin() const noexcept {
		return const_reverse_iterator(cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reverse_iterator hashtable<Key, Value, MIN_BSIZE>::crbegin() const noexcept {
		return const_reverse_iterator(cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::iterator hashtable<Key, Value, MIN_BSIZE>::end() noexcept {
		return iterator(&elems.back(), elems.back().end(), elems.front().begin(), elems.back().end());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_iterator hashtable<Key, Value, MIN_BSIZE>::end() const noexcept {
		return const_iterator(&elems.back(), elems.back().cend(), elems.front().cbegin(), elems.back().cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_iterator hashtable<Key, Value, MIN_BSIZE>::cend() const noexcept {
		return const_iterator(&elems.back(), elems.back().cend(), elems.front().cbegin(), elems.back().cend());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::reverse_iterator hashtable<Key, Value, MIN_BSIZE>::rend() noexcept {
		return reverse_iterator(begin());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reverse_iterator hashtable<Key, Value, MIN_BSIZE>::rend() const noexcept {
		return const_reverse_iterator(cbegin());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reverse_iterator hashtable<Key, Value, MIN_BSIZE>::crend() const noexcept {
		return const_reverse_iterator(cbegin());
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline bool hashtable<Key, Value, MIN_BSIZE>::empty() const noexcept {
		for (const auto& list : elems) {
			if (!list.empty()) {
				return false;
			}
		}
		return true;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::node_type& hashtable<Key, Value, MIN_BSIZE>::front()  {
		YO_ASSERT_THROW(empty(), "Empty hashtable");
		return *begin();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::node_type& hashtable<Key, Value, MIN_BSIZE>::back()  {
		YO_ASSERT_THROW(empty(), "Empty hashtable");
		return *--end();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline const typename hashtable<Key, Value, MIN_BSIZE>::node_type& hashtable<Key, Value, MIN_BSIZE>::front() const {
		YO_ASSERT_THROW(empty(), "Empty hashtable");
		return *cbegin();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline const typename hashtable<Key, Value, MIN_BSIZE>::node_type& hashtable<Key, Value, MIN_BSIZE>::back() const {
		YO_ASSERT_THROW(empty(), "Empty hashtable");
		return *--cend();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::list_type* hashtable<Key, Value, MIN_BSIZE>::data() noexcept {
		return elems.data();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline const typename hashtable<Key, Value, MIN_BSIZE>::list_type* hashtable<Key, Value, MIN_BSIZE>::data() const noexcept {
		return elems.data();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::reference hashtable<Key, Value, MIN_BSIZE>::at(const Key& key) noexcept {
		iterator i = find(key);

		if (i == end()) {
			return insert({ key, Value() })->value;
		}

		return i->value;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reference hashtable<Key, Value, MIN_BSIZE>::at(const Key& key) const {
		const_iterator i = find(key);
		YO_ASSERT_THROW(i == end(), "Unexisting key");
		return i->value;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::reference hashtable<Key, Value, MIN_BSIZE>::operator[](const Key& key) noexcept {
		iterator i = find(key);

		if (i == end()) {
			return insert({ key, Value() })->value;
		}

		return i->value;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline typename hashtable<Key, Value, MIN_BSIZE>::const_reference hashtable<Key, Value, MIN_BSIZE>::operator[](const Key& key) const {
		const_iterator i = find(key);
		YO_ASSERT_THROW(i == end(), "Unexisting key");
		return i->value;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline vector<typename hashtable<Key, Value, MIN_BSIZE>::node_type> hashtable<Key, Value, MIN_BSIZE>::items() const noexcept {
		vector<node_type> result;
		for (const auto& item : *this) {
			result.push_back(item);
		}
		return result;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline hashtable<Key, Value>& hashtable<Key, Value, MIN_BSIZE>::operator=(const hashtable<Key, Value>& other) noexcept {
		if (this != &other) {
			elems.resize(other.elems.size());
			for (size_t i = 0; i < elems.size(); ++i) {
				elems[i] = other.elems[i];
			}
			limit = other.limit;
		}
		return *this;
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline size_t hashtable<Key, Value, MIN_BSIZE>::hasher(size_t key) const noexcept {
		return key % elems.size();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline size_t hashtable<Key, Value, MIN_BSIZE>::hasher(const string& key) const noexcept {
		size_t index = 0;
		for (auto c : key) {
			index += (int)c;
		}
		return index % elems.size();
	}


	template<typename Key, typename Value, size_t MIN_BSIZE>
	inline void hashtable<Key, Value, MIN_BSIZE>::calculate_new_bucket(const list_type& list) noexcept {
		if (list.size() >= limit) {
			elems.resize(limit *= 2);
			rehash();
		}
	}



	template<typename Key, typename Value>
	bool operator==(const HashNode<Key, Value>& left, const HashNode<Key, Value>& right) {
		return left.key == right.key;
	}


	template<typename Key, typename Value>
	bool operator!=(const HashNode<Key, Value>& left, const HashNode<Key, Value>& right) {
		return left.key != right.key;
	}


	template<typename Key, typename Value>
	std::ostream& operator<<(std::ostream& os, const HashNode<Key, Value>& node) {
		os << "[" << node.key << ", " << node.value << "]";
		return os;
	}



	template<typename Key, typename Value>
	bool operator==(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		if (yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0) {
			return left.elems_count() == right.elems_count();
		}
		return false;
	}


	template<typename Key, typename Value>
	bool operator!=(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		return !(left == right);
	}


	template<typename Key, typename Value>
	bool operator>(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 1;
	}


	template<typename Key, typename Value>
	bool operator<(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		return yo::compare(left.begin(), right.begin(), left.end(), right.end()) == -1;
	}


	template<typename Key, typename Value>
	bool operator>=(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		short result = yo::compare(left.begin(), right.begin(), left.end(), right.end());
		if (result == 0) {
			return left.elems_count() == right.elems_count();
		}
		return result == 1;
	}


	template<typename Key, typename Value>
	bool operator<=(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		short result = yo::compare(left.begin(), right.begin(), left.end(), right.end());
		if (result == 0) {
			return left.elems_count() == right.elems_count();
		}
		return result == -1;
	}


	template<typename Key, typename Value>
	void swap(hashtable<Key, Value>& left, hashtable<Key, Value>& right) {
		left.swap(right);
	}


}
