/**
 * @file files.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief Utility functions to read/write from/to files and defaines unsigned_ints structure.
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 * This header file defines the unsigned_ints, and various template function which makes easier to read and write files using std:map-s or std::vector-s of std::pair-s.
 * unsigned_ints stores two uint16_t can be constructed/casted from/to a uint32_t value, interpreting uint32_t's 16 first bits as the first value and the remaining bits as the second value.
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <map>

/**
 * @struct unsigned_ints
 * @todo: add brief description and replace this struct with a simpler appproach using bitshifts; e.g: (same memory usage, potentially less copies, cleanner code)
 * 	struct unsigned_ints {
 * 		union {
 * 			struct {
 *				uint32_t first : 16;
 *				uint32_t second : 16;
 *			};
 *			uint32_t values;
 * 		};
 *  };
 *  Or make unsigned_ints actually be a std::pair<uint16_t> (adding ctr overload for uint32_t input + operator uint32_t() cast operator)
 */
struct unsigned_ints
{
	uint16_t first;
	uint16_t second;

	inline unsigned_ints() = default;

	inline unsigned_ints(unsigned int first, unsigned int second)
		: first(first), second(second)
	{}

	/**
	 * @brief Construct a new unsigned ints object from a uint32_t interpreted as two uint16_t concatenated
	 */
	inline unsigned_ints(uint32_t values)
		: first(static_cast<uint16_t>(values >> 16)), second (static_cast<uint16_t>(values))
	{}

	/*
	/**
	 * @brief Construct a new unsigned ints object from a uint64_t interpreted as two uint32_t concatenated
	 * @todo It may be preferable to use explicit keyword as this constructor would be unintuitive when implicitly called or called as cast operator
	 *//*
	inline unsigned_ints(uint64_t values)
		: first(static_cast<uint32_t>(values >> 32)), second(static_cast<uint32_t>(values))
	{}*/

	inline operator uint32_t()
	{
		return static_cast<uint32_t>(first) << 16 | static_cast<uint32_t>(second);
	}

	/*inline operator uint64_t()
	{
		return static_cast<uint64_t>(first) << 32 | static_cast<uint64_t>(second);
	}*/
};

template<typename T>
void binWrite(std::ostream& os, const T& value);
template<typename T>
void binRead(std::istream& is, T& value);
template<typename T>
void binSkip(std::istream& is);

/*
template<>
void binWrite<unsigned int>(std::ostream& os, const unsigned int& value);
template<>
void binRead<unsigned int>(std::istream& is, unsigned int& value);
template<>
void binSkip<unsigned int>(std::istream& is);

template<>
void binWrite<std::string>(std::ostream& os, const std::string& text);
template<>
void binRead<std::string>(std::istream& is, std::string& text);
template<>
void binSkip<std::string>(std::istream& is);
*/

template<typename T1, typename T2>
void binWrite_vec(std::ostream& os, const std::vector<std::pair<T1, T2>>& vec)
{
	unsigned int length = vec.size();
	binWrite<unsigned int>(os, length);
	for (const auto& pair : vec)
	{
		binWrite<T1>(os, pair.first);
		binWrite<T2>(os, pair.second);
	}
}
template<typename T1, typename T2>
void binRead_vec(std::istream& is, std::vector<std::pair<T1, T2>>& vec)
{
	unsigned int length; binRead<unsigned int>(is, length);
	vec.clear();
	for (unsigned int i = 0; i < length; i++)
	{
		T1 key; binRead<T1>(is, key);
		T2 val; binRead<T2>(is, val);
		vec.emplace_back(key, val);
	}
}
template<typename T1, typename T2>
void binSkip_vec(std::istream& is)
{
	unsigned int length; binRead<unsigned int>(is, length);
	for (unsigned int i = 0; i < length; i++)
	{
		binSkip<T1>(is);
		binSkip<T2>(is);
	}
}

template<typename TK, typename TV>
void binWrite_map(std::ostream& os, const std::map<TK, TV>& map)
{
	unsigned int length = map.size();
	binWrite<unsigned int>(os, length);
	for (const auto& pair : map)
	{
		binWrite<TK>(os, pair.first);
		binWrite<TV>(os, pair.second);
	}
}
template<typename TK, typename TV>
void binRead_map(std::istream& is, std::map<TK, TV>& map)
{
	unsigned int length; binRead<unsigned int>(is, length);
	map.clear();
	for (unsigned int i = 0; i < length; i++)
	{
		TK key; binRead<TK>(is, key);
		TV val; binRead<TV>(is, val);
		map[key] = val;
	}
}
template<typename TK, typename TV>
void binSkip_map(std::istream& is)
{
	unsigned int length; binRead<unsigned int>(is, length);
	for (unsigned int i = 0; i < length; i++)
	{
		binSkip<TK>(is);
		binSkip<TV>(is);
	}
}
