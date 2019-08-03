#pragma once

#ifndef YO_ASSERT_THROW
#define YO_ASSERT_THROW(statement, message) if(statement == true) throw message
#endif 

#include "list.h"


namespace yo {


	template<typename T, class Container = yo::list<T>>
	class queue {
	public:
		queue       ()              noexcept;

		void push   (const T& item) noexcept;
		void pop    ();

		T front     ()const;
		T back      ()const;

		bool empty  ()const         noexcept;

		size_t size ()const         noexcept;
	private:
		Container data;
	};


	template<typename T, class Container>
	inline queue<T, Container>::queue() noexcept {}


	template<typename T, class Container>
	inline void queue<T, Container>::push(const T& item) noexcept {
		data.push_back(item);
	}


	template<typename T, class Container>
	inline void queue<T, Container>::pop() {
		YO_ASSERT_THROW(data.empty(), "Empty queue");
		data.pop_front();
	}


	template<typename T, class Container>
	inline T queue<T, Container>::front() const {
		YO_ASSERT_THROW(data.empty(), "Empty queue");
		return data.front();
	}


	template<typename T, class Container>
	inline T queue<T, Container>::back() const {
		YO_ASSERT_THROW(data.empty(), "Empty queue");
		return data.back();
	}


	template<typename T, class Container>
	inline bool queue<T, Container>::empty() const noexcept {
		return data.empty();
	}


	template<typename T, class Container>
	inline size_t queue<T, Container>::size() const noexcept {
		return data.size();
	}


}
