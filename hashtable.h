#pragma once


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


#include <initializer_list>

#include "iterators/hash_iterator.h"
#include "utility.h"


namespace yo {


	template<typename Key, typename Value>
	struct Pair {
		Key   first;
		Value second;
	};


	template<typename T>
	struct HashNode {
		HashNode* next;
		T         item;
	};


	template<typename Key, typename Value >
	class hashtable {
	public:
		typedef hashtable<Key, Value>                                            this_type;
		typedef Pair<Key, Value>                                                 value_type;
		typedef HashNode<value_type>                                             node_type;
		typedef size_t                                                           size_type;
		typedef Value&                                                           reference;
		typedef const Value&                                                     const_reference;
		typedef hash_iterator<value_type, node_type, value_type&>                iterator;
		typedef hash_iterator<const value_type, node_type, const value_type&>    const_iterator;
		typedef Pair<bool, iterator>                                             pair;
	public:
		hashtable                 ()                                             noexcept;
		hashtable                 (const std::initializer_list<node_type>& list) noexcept;
		hashtable                 (const node_type& item)                        noexcept;
		hashtable                 (const this_type& other)                       noexcept;

		~hashtable                ()noexcept;

		pair insert               (const value_type& item)                       noexcept;
		iterator insert_no_care   (const value_type& item)                       noexcept;
		iterator insert_or_assign (const value_type& item)                       noexcept;

		iterator erase            (const iterator& pos);

		iterator find             (const Key& key)                               noexcept;
		const_iterator find       (const Key& key)const                          noexcept;

		void max_load_factor      (float mlf)                                    noexcept;
		void rehash               ()                                             noexcept;
		void rehash               (size_type maxbuckets)                         noexcept;
		void reserve              (size_type maxbuckets)                         noexcept;
		void clear                ()                                             noexcept;

		iterator begin            ()                                             noexcept;
		const_iterator begin      ()const                                        noexcept;
		const_iterator cbegin     ()const                                        noexcept;

		iterator end              ()                                             noexcept;
		const_iterator end        ()const                                        noexcept;
		const_iterator cend       ()const                                        noexcept;

		float max_load_factor     ()const                                        noexcept;
		float load_factor         ()const                                        noexcept;

		bool empty                ()const                                        noexcept;

		size_type size            ()const                                        noexcept;
		size_type bucket_count    ()const                                        noexcept;
		size_type bucket          (const Key& key)const                          noexcept;
		size_type count           (const Key& key)const                          noexcept;
		size_type bucket_size     (size_type index)const                         noexcept;

		reference at              (const Key& key);
		const_reference at        (const Key& key)const;

		reference operator[]      (const Key& key)                               noexcept;

		this_type& operator=      (const this_type& other)                       noexcept;
	protected:
		node_type** elems;
		node_type*  end_ptr;
		size_type   BUCKET_SIZE;
		size_type   SIZE;
		float       max_factor;
	private:
		node_type** bucket_create (size_type size)const                          noexcept;
		void bucket_assign        (node_type**& src, node_type**& dst)           noexcept;
		void bucket_free          (node_type** b) noexcept;

		node_type* list_find      (const node_type* list, const Key& key)const   noexcept;
		size_type list_size       (const node_type* list)const                   noexcept;
		void list_push            (node_type*& list, const value_type& item)     noexcept;
		void list_move            (node_type*& list, node_type*& node)           noexcept;
		void list_free            (node_type*& list)                             noexcept;

		void node_free            (node_type*& node)                             noexcept;

		size_type hasher          (size_t key)const                              noexcept;
		size_type hasher          (const char* key)const                         noexcept;
	};


	template<typename Key, typename Value>
	inline hashtable<Key, Value>::hashtable() noexcept {
		end_ptr = new node_type;
		elems = bucket_create(BUCKET_SIZE = 2);
		max_factor = 1;
		SIZE = 0;
	}


	template<typename Key, typename Value>
	inline hashtable<Key, Value>::hashtable(const std::initializer_list<node_type>& list) noexcept : hashtable() {
		for (const auto& item : list) {
			insert(item);
		}
	}

	template<typename Key, typename Value>
	inline hashtable<Key, Value>::hashtable(const node_type& item) noexcept : hashtable() {
		insert(item);
	}


	template<typename Key, typename Value>
	inline hashtable<Key, Value>::hashtable(const this_type& other) noexcept : hashtable() {
		*this = other;
	}


	template<typename Key, typename Value>
	inline hashtable<Key, Value>::~hashtable() noexcept {
		bucket_free(elems);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::pair hashtable<Key, Value>::insert(const value_type& item) noexcept {
		if (load_factor() >= max_load_factor()) {
			rehash(BUCKET_SIZE * 2);
		}

		node_type** pos   = elems + hasher(item.first) + 1;
		node_type*  first = list_find(*pos, item.first);

		if (first != nullptr) {
			return { false, { pos, first, end_ptr } };
		}

		++SIZE;
		list_push(*pos, item);
		return { true, { pos, *pos, end_ptr } };
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::insert_no_care(const value_type& item) noexcept {
		node_type** pos = elems + hasher(item.first) + 1;

		insert_item(*pos, item);
		++SIZE;

		return { pos, *pos, end_ptr };
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::insert_or_assign(const value_type& item) noexcept {
		if (load_factor() >= max_load_factor()) {
			rehash(BUCKET_SIZE * 2);
		}

		node_type** pos = elems + hasher(item.first) + 1;
		node_type* first = list_find(*pos, item.first);

		if (first != nullptr) {
			first->item.second = item.second;
			return { pos, first, end_ptr };
		}

		++SIZE;
		list_push(*pos, item);
		return { pos, *pos, end_ptr };
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::erase(const iterator& pos) {
		YO_ASSERT_THROW(empty(), "Empty hashtable");
		iterator result = pos.next();

		if (*pos.container == pos.pos) {
			node_free(*pos.container);
		}
		else {
			node_type* it = *pos.container;
			while (it->next != pos.pos) {
				it = it->next;
			}
			node_free(it->next);
		}

		--SIZE;
		return result;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::find(const Key& key) noexcept {
		size_type i = hasher(key) + 1;

		node_type* first = list_find(elems[i], key);
		if (first != nullptr) {
			return { &elems[i], first, end_ptr };
		}

		return end();
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_iterator hashtable<Key, Value>::find(const Key& key) const noexcept {
		size_type i = hasher(key) + 1;

		node_type* first = list_find(elems[i], key);
		if (first != nullptr) {
			return { &elems[i], first, end_ptr };
		}

		return cend();
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::max_load_factor(float mlf) noexcept {
		max_factor = mlf;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::rehash() noexcept {
		rehash(bucket_count());
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::rehash(size_type maxbuckets) noexcept {
		node_type** new_bucket = bucket_create(BUCKET_SIZE = maxbuckets);
		bucket_assign(elems, new_bucket);

		delete elems;
		elems = new_bucket;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::reserve(size_type maxbuckets) noexcept {
		if (maxbuckets > bucket_count()) {
			rehash(maxbuckets);
		}
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::clear() noexcept {
		node_type** b = elems;
		for (++elems; *elems != end_ptr; ++elems) {
			while (*elems != nullptr) {
				node_free(*elems);
			}
		}
		SIZE = 0;
		elems = b;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::begin() noexcept {
		return iterator(&elems[1], end_ptr, 1);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_iterator hashtable<Key, Value>::begin() const noexcept {
		return const_iterator(&elems[1], end_ptr, 1);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_iterator hashtable<Key, Value>::cbegin() const noexcept {
		return const_iterator(&elems[1], end_ptr, 1);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::iterator hashtable<Key, Value>::end() noexcept {
		return iterator(&elems[BUCKET_SIZE + 1], end_ptr, -1);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_iterator hashtable<Key, Value>::end() const noexcept {
		return const_iterator(&elems[BUCKET_SIZE + 1], end_ptr, -1);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_iterator hashtable<Key, Value>::cend() const noexcept {
		return const_iterator(&elems[BUCKET_SIZE + 1], end_ptr, -1);
	}


	template<typename Key, typename Value>
	inline float hashtable<Key, Value>::max_load_factor() const noexcept {
		return max_factor;
	}


	template<typename Key, typename Value>
	inline float hashtable<Key, Value>::load_factor() const noexcept {
		return static_cast<float>(SIZE) / static_cast<float>(BUCKET_SIZE);
	}


	template<typename Key, typename Value>
	inline bool hashtable<Key, Value>::empty() const noexcept {
		return SIZE == 0;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::size() const noexcept {
		return SIZE;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::bucket_count() const noexcept {
		return BUCKET_SIZE;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::bucket(const Key& key) const noexcept {
		return hasher(key) + 1;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::count(const Key& key) const noexcept {
		size_type size = 0;

		for (const auto& item : *this) {
			if (item.first == key) {
				++size;
			}
		}

		return size;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::bucket_size(size_type index) const noexcept {
		return list_size(elems[index + 1]);
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::reference hashtable<Key, Value>::at(const Key& key) {
		iterator i = find(key);

		if (i != end()) {
			return i->second;
		}

		throw "No key";
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::const_reference hashtable<Key, Value>::at(const Key& key) const {
		const_iterator i = find(key);

		if (i != cend()) {
			return i->second;
		}

		throw "No key";
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::reference hashtable<Key, Value>::operator[](const Key& key) noexcept {
		iterator i = find(key);

		if (i == end()) {
			return insert({ key, Value() }).second->second;
		}

		return i->second;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::this_type& hashtable<Key, Value>::operator=(const this_type& other) noexcept {
		if (this != &other) {
			SIZE = 0;
			free_bucket(elems);
			elems = create_bucket(BUCKET_SIZE = other.bucket_count());

			for (const auto& item : other) {
				insert_no_care(item);
			}
		}

		return *this;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::node_type** hashtable<Key, Value>::bucket_create(size_type size) const noexcept {
		node_type** b = new node_type * [++size + 1];

		for (size_type i = 0; i < size; ++i) {
			b[i] = nullptr;
		}
		b[0] = b[size] = end_ptr;

		return b;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::bucket_assign(node_type**& src, node_type**& dst) noexcept {
		node_type** temp_bucket = src;
		for (++temp_bucket; *temp_bucket != end_ptr; ++temp_bucket) {
			node_type* row = *temp_bucket;
			while (row != nullptr) {
				list_move(row, dst[hasher(row->item.first) + 1]);
			}
		}
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::bucket_free(node_type** b) noexcept {
		node_type** temp_bucket = b;

		for (++b; *b != end_ptr; ++b) {
			list_free(*b);
		}

		delete temp_bucket;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::node_type* hashtable<Key, Value>::list_find(const node_type* list, const Key& key) const noexcept {
		while (list != nullptr) {
			if (list->item.first == key) {
				return (node_type*)list;
			}
			list = list->next;
		}
		return nullptr;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::list_size(const node_type* list) const noexcept {
		size_type size = 0;

		while (list != nullptr) {
			++size;
			list = list->next;
		}

		return size;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::list_push(node_type*& list, const value_type& item) noexcept {
		node_type* node = new node_type;
		node->next = list;
		node->item = item;
		list = node;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::list_move(node_type*& list, node_type*& node) noexcept {
		node_type* temp = list;
		list = list->next;
		temp->next = node;
		node = temp;
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::list_free(node_type*& list) noexcept {
		while (list != nullptr) {
			node_free(list);
		}
	}


	template<typename Key, typename Value>
	inline void hashtable<Key, Value>::node_free(node_type*& node) noexcept {
		node_type* temp_node = node;
		node = node->next;
		delete temp_node;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::hasher(size_t key) const noexcept {
		return key % BUCKET_SIZE;
	}


	template<typename Key, typename Value>
	inline typename hashtable<Key, Value>::size_type hashtable<Key, Value>::hasher(const char* key) const noexcept {
		size_type sum = 0;
		while (*key != '\0') {
			sum += *key++;
		}
		return hasher(sum);
	}




	template<typename Key, typename Value>
	bool operator==(const Pair<Key, Value>& left, const Pair<Key, Value>& right) {
		return left.first == right.first;
	}


	template<typename Key, typename Value>
	bool operator!=(const Pair<Key, Value>& left, const Pair<Key, Value>& right) {
		return left.first != right.first;
	}


	template<typename Key, typename Value>
	std::ostream& operator<<(std::ostream& os, const Pair<Key, Value>& node) {
		os << "[" << node.first << ", " << node.second << "]";
		return os;
	}



	template<typename Key, typename Value>
	bool operator==(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		if (yo::compare(left.begin(), right.begin(), left.end(), right.end()) == 0) {
			return left.size() == right.size();
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
			return left.size() == right.size();
		}
		return result == 1;
	}


	template<typename Key, typename Value>
	bool operator<=(const hashtable<Key, Value>& left, const hashtable<Key, Value>& right) {
		short result = yo::compare(left.begin(), right.begin(), left.end(), right.end());
		if (result == 0) {
			return left.size() == right.size();
		}
		return result == -1;
	}


	template<typename Key, typename Value>
	void swap(hashtable<Key, Value>& left, hashtable<Key, Value>& right) {
		left.swap(right);
	}


}
