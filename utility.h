#pragma once


namespace yo {


	template<typename T>
	void swap(T& a, T& b) {
		T temp = a;
		a = b;
		b = temp;
	}


	template<typename Iterator>
	short compare(Iterator lbegin, Iterator rbegin, const Iterator& lend, const Iterator& rend) {
		while (lbegin != lend && rbegin != rend) {
			if (*lbegin > *rbegin) {
				return 1;
			}
			if (*lbegin++ < *rbegin++) {
				return -1;
			}
		}
		return 0;
	}


	template<typename Iterator, typename T>
	Iterator find_first(Iterator first, const Iterator& last, const T& value) {
		while (first != last) {
			if (*first == value) {
				return first;
			}
			++first;
		}
		return last;
	}


}
