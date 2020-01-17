#pragma once

#include <cctype>
#include <climits>

#include <algorithm>
#include <locale>

#include "os_macros.hpp"

static inline bool ends_with(std::string const &value, std::string const &ending) {
	return ending.size() > value.size() ? false : std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static inline bool starts_with(const std::string &mainStr, const std::string &toMatch) {
	return mainStr.find(toMatch) == 0;
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return ch >= 0 && ch <= UCHAR_MAX ? !std::isspace(ch) : true; }));
}

static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return ch >= 0 && ch <= UCHAR_MAX ? !std::isspace(ch) : true; }).base(), s.end());
}

static inline void trim(std::string &s) {
	rtrim(s);
	ltrim(s);
}

static inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

static inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

static inline std::string trim_copy(std::string s) {
	rtrim(s);
	ltrim(s);
	return s;
}

#if windows
#include <codecvt>
#include <string>

static inline std::wstring utf8_to_wstring(const std::string &str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

static inline std::string wstring_to_utf8(const std::wstring &str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
#endif