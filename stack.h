#pragma once


#include "vector.h"


#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 


namespace yo {


	template <typename T, class Container = yo::vector<T>>
	class stack {
	public:
		stack       ()              noexcept;

		void push   (const T& item) noexcept;
		void pop    ();

		T top       ();

		bool empty  ()              noexcept;

		size_t size ()              noexcept;
	private:
		Container data;
	};


	template <typename T, class Container>
	inline stack<T, Container>::stack() noexcept {}


	template<typename T, class Container>
	inline void stack<T, Container>::push(const T& item) noexcept {
		data.push_back(item);
	}


	template<typename T, class Container>
	inline void stack<T, Container>::pop() {
		YO_ASSERT_THROW(data.empty(), "Empty stack");
		data.pop_back();
	}


	template<typename T, class Container>
	inline T stack<T, Container>::top() {
		YO_ASSERT_THROW(data.empty(), "Empty stack");
		return data.back();
	}


	template<typename T, class Container>
	inline bool stack<T, Container>::empty() noexcept {
		return data.empty();
	}


	template<typename T, class Container>
	inline size_t stack<T, Container>::size() noexcept {
		return data.size();
	}


}