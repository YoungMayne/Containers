#pragma once


namespace yo {


	template<typename T, typename Node, typename Reference>
	class hash_iterator {
	public:
		typedef T value_type;
		typedef Node node_type;
		typedef Reference reference;
		typedef hash_iterator<T, Node, Reference> iterator;
	public:
		hash_iterator()noexcept;
		hash_iterator(node_type** container, node_type* end_ptr, int type)noexcept;
		hash_iterator(node_type** container, node_type* pos, node_type* end_ptr)noexcept;
		hash_iterator(const iterator& other)noexcept;

		iterator& operator++()noexcept;
		iterator operator++(int)noexcept;

		iterator next()const noexcept;

		reference operator*()const noexcept;
		value_type* operator->()const noexcept;

		bool operator==(const iterator& other)const noexcept;
		bool operator!=(const iterator& other)const noexcept;

		iterator& operator=(const iterator& other)noexcept;
	public:
		node_type** container;
		node_type*  end_ptr;
		node_type*  pos;
	};


	template<typename T, typename Node, typename Reference>
	inline hash_iterator<T, Node, Reference>::hash_iterator() noexcept {}

	template<typename T, typename Node, typename Reference>
	inline hash_iterator<T, Node, Reference>::hash_iterator(node_type** container, node_type* end_ptr, int type) noexcept {
		this->container = container;
		while (*this->container != end_ptr && *this->container == nullptr) {
			this->container += type;
		}

		this->end_ptr = end_ptr;
		pos = *this->container;
	}

	template<typename T, typename Node, typename Reference>
	inline hash_iterator<T, Node, Reference>::hash_iterator(node_type** container, node_type* pos, node_type* end_ptr) noexcept {
		this->container = container;
		this->end_ptr = end_ptr;
		this->pos = pos;
	}


	template<typename T, typename Node, typename Reference>
	inline hash_iterator<T, Node, Reference>::hash_iterator(const iterator& other) noexcept {
		*this = other;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::iterator& hash_iterator<T, Node, Reference>::operator++() noexcept {
		pos = pos->next;
		while (pos == nullptr) {
			pos = *++container;
		}
		return *this;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::iterator hash_iterator<T, Node, Reference>::operator++(int) noexcept {
		iterator temp(*this);
		operator++();
		return temp;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::iterator hash_iterator<T, Node, Reference>::next() const noexcept {
		iterator result(*this);
		return ++result;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::reference hash_iterator<T, Node, Reference>::operator*() const noexcept {
		return pos->item;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::value_type* hash_iterator<T, Node, Reference>::operator->() const noexcept {
		return &this->operator*();
	}


	template<typename T, typename Node, typename Reference>
	inline bool hash_iterator<T, Node, Reference>::operator==(const iterator& other) const noexcept {
		return pos == other.pos;
	}


	template<typename T, typename Node, typename Reference>
	inline bool hash_iterator<T, Node, Reference>::operator!=(const iterator& other) const noexcept {
		return pos != other.pos;
	}


	template<typename T, typename Node, typename Reference>
	inline typename hash_iterator<T, Node, Reference>::iterator& hash_iterator<T, Node, Reference>::operator=(const iterator& other) noexcept {
		container = other.container;
		end_ptr = other.end_ptr;
		pos = other.pos;

		return *this;
	}

}
