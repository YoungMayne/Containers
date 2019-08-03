#pragma once


#include <iostream>

#include "vector.h"
#include "utility.h"


namespace yo {


	class string : public vector<char> {
	public:
		string                          ()                                    noexcept;
		string                          (const char& c)                       noexcept;
		string                          (const char* str)                     noexcept;
		string                          (size_t count, const char& c)         noexcept;

		bool starts_with                (const string& str)const              noexcept;
		bool ends_with                  (const string& str)const              noexcept;

		char* c_str                     ()                                    noexcept;
		const char* c_str               ()const                               noexcept;

		string operator+                (const string& str)const              noexcept;
		string operator+=               (const string& str)                   noexcept;

		size_t length                   ()const                               noexcept;

		friend std::ostream& operator<< (std::ostream& os, const string& str) noexcept;
		friend std::istream& operator>> (std::istream& is, string& str)       noexcept;
	};


	inline string::string() noexcept : vector<char>() {}


	inline string::string(const char& c) noexcept : vector<char>() {
		push_back(c);
	}


	inline string::string(const char* str) noexcept : vector<char>() {
		while (*str != '\0') {
			push_back(*str++);
		}
	}


	inline string::string(size_t count, const char& c) noexcept : vector<char>() {
		while (count-- > 0) {
			push_back(c);
		}
	}


	inline bool string::starts_with(const string& str) const noexcept {
		if (SIZE >= str.size()) {
			return yo::compare(begin(), str.begin(), end(), str.end()) == 0;
		}
		else {
			return false;
		}
	}


	inline bool string::ends_with(const string& str) const noexcept {
		if (SIZE >= str.size()) {
			return yo::compare(end() - str.size(), str.begin(), end(), str.end()) == 0;
		}
		else {
			return false;
		}
	}


	inline char* string::c_str() noexcept {
		return &elems[0];
	}


	inline const char* string::c_str() const noexcept {
		return &elems[0];
	}


	inline string string::operator+(const string& str) const noexcept {
		string result = *this;
		for (const auto& item : str) {
			result.push_back(item);
		}
		return result;
	}


	inline string string::operator+=(const string& str) noexcept {
		*this = *this + str;
		return *this;
	}


	inline size_t string::length() const noexcept {
		return SIZE;
	}


	std::ostream& operator<<(std::ostream& os, const string& str) noexcept {
		for (const auto& item : str) {
			os << item;
		}
		return os;
	}


	inline std::istream& operator>>(std::istream& is, string& str) noexcept {
		std::istream::sentry sentry(is);
		if (!sentry) {
			return is;
		}
		else {
			str.clear();
			while (true) {
				int character = is.get();
				if (character == -1 || isspace(character)) {
					return is;
				}
				else {
					str.push_back(character);
				}
			}
		}
	}


}
