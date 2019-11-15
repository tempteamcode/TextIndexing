#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

template<typename T>
void binWrite(std::ostream& os, const T& value);

template<typename T>
void binRead(std::istream& is, T& value);

template<typename T>
void binSkip(std::istream& is);

template<>
inline void binWrite<unsigned int>(std::ostream& os, const unsigned int& value)
{
	unsigned char bits[4];
	for (int b = 0; b < 4; b++)
	{
		bits[b] = (value >> (8*b)) & 0xFF;
	}
	os.write(reinterpret_cast<char*>(&bits[0]), 4);
}
template<>
inline void binRead<unsigned int>(std::istream& is, unsigned int& value)
{
	unsigned char bits[4];
	is.read(reinterpret_cast<char*>(&bits[0]), 4);
	value = (((bits[3]) * 256 + bits[2]) * 256 + bits[1]) * 256 + bits[0];
}
template<>
inline void binSkip<unsigned int>(std::istream& is)
{
	is.ignore(4);
}

template<>
inline void binWrite<std::string>(std::ostream& os, const std::string& text)
{
	unsigned int length = text.length();
	binWrite<unsigned int>(os, length);
	if (length > 0) os.write(text.c_str(), length);
}
template<>
inline void binRead<std::string>(std::istream& is, std::string& text)
{
	unsigned int length; binRead(is, length);
	if (length > 0)
	{
		char* buffer = new char[length + 1];
		is.read(buffer, length);
		buffer[length] = '\0';
		text = buffer;
		delete[] buffer;
	}
	else text.clear();
}
template<>
inline void binSkip<std::string>(std::istream& is)
{
	unsigned int length; binRead(is, length);
	if (length > 0) is.ignore(length);
}

template<typename T1, typename T2>
inline void binWrite_vec(std::ostream& os, const std::vector<std::pair<T1, T2>>& vec)
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
inline void binRead_vec(std::istream& is, std::vector<std::pair<T1, T2>>& vec)
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
inline void binSkip_vec(std::istream& is)
{
	unsigned int length; binRead<unsigned int>(is, length);
	for (unsigned int = 0; i < length; i++)
	{
		binSkip<T1>(is);
		binSkip<T2>(is);
	}
}

template<typename TK, typename TV>
inline void binWrite_map(std::ostream& os, const std::map<TK, TV>& map)
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
inline void binRead_map(std::istream& is, std::map<TK, TV>& map)
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
inline void binSkip_map(std::istream& is)
{
	unsigned int length; binRead<unsigned int>(is, length);
	for (unsigned int = 0; i < length; i++)
	{
		binSkip<TK>(is);
		binSkip<TV>(is);
	}
}
