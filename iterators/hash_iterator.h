#pragma once


namespace yo {


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	class hash_iterator {
	public:
		typedef T                                                                value;
		typedef Pointer                                                          pointer;
		typedef Reference                                                        reference;
		typedef hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>     iterator;
	public:
		hash_iterator        ()                                                  noexcept;
		hash_iterator        (const ContainerPtr container, const Iterator& end) noexcept;
		hash_iterator        (const ContainerPtr container, const Iterator& it,
			                  const Iterator& begin, const Iterator& end)        noexcept;
		hash_iterator        (const iterator& other)                             noexcept;

		iterator next        ()const                                             noexcept;
		iterator prev        ()const                                             noexcept;

		iterator& operator++ ()                                                  noexcept;
		iterator operator++  (int)                                               noexcept;

		iterator& operator-- ()                                                  noexcept;
		iterator operator--  (int)                                               noexcept;

		reference operator*  ()const                                             noexcept;
		value* operator->    ()const                                             noexcept;

		bool operator==      (const iterator& other)const                        noexcept;
		bool operator!=      (const iterator& other)const                        noexcept;

		iterator& operator=  (const iterator& other)                             noexcept;
	public:
		ContainerPtr container;
		Iterator     begin;
		Iterator     end;
		Iterator     it;
	};


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::hash_iterator() noexcept {}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::hash_iterator(const ContainerPtr container, const Iterator& end) noexcept {
		this->begin = container->begin();
		this->container = container;

		while(this->container->empty() && this->container->end() != end) {
			++this->container;
		}

		this->end = end;
		this->it = this->container->begin();
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::
	hash_iterator(const ContainerPtr container, const Iterator& it, const Iterator& begin, const Iterator& end) noexcept {
		this->container = container;
		this->begin = begin;
		this->end = end;
		this->it = it;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::hash_iterator(const iterator& other) noexcept {
		*this = other;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator
	hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::next() const noexcept {
		iterator result(*this);
		return ++result;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator
	hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::prev() const noexcept {
		iterator result(*this);
		return --result;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator& 
	yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator++() noexcept {
		if (++it == container->end()) {
			while (it != end) {
				it = (++container)->begin();
				if (!container->empty()) {
					break;
				}
			}
		}
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator
	yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator++(int) noexcept {
		iterator temp(*this);
		operator++();
		return temp;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator&
	hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator--() noexcept {
		if (it-- == container->begin()) {
			while (it != begin) {
				it = --(--container)->end();
				if (!container->empty()) {
					break;
				}
			}
		}
		return *this;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator 
	hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator--(int) noexcept {
		iterator temp(*this);
		operator--();
		return temp;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::reference
	yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator*() const noexcept {
		return *it;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::value*
	yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator->() const noexcept {
		return &(*it);
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline bool yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator==(const iterator& other) const noexcept {
		return it == other.it;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline bool yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator!=(const iterator& other) const noexcept {
		return it != other.it;
	}


	template<typename T, typename Pointer, typename Reference, typename ContainerPtr, typename Iterator>
	inline typename yo::hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::iterator&
	hash_iterator<T, Pointer, Reference, ContainerPtr, Iterator>::operator=(const iterator& other) noexcept {
		container = other.container;
		begin = other.begin;
		end = other.end;
		it = other.it;

		return *this;
	}


}
