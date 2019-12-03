#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <map>

/**
* Structure représentant une paire d'entiers.
* Ces entiers sont à manipuler comme s'ils étaients des "unsigned int",
* leur type réel (uint16_t, uint32_t, ...) n'est déterminé que selon les besoins de cette application.
**/

struct unsigned_ints
{
	uint16_t first;
	uint16_t second;

	inline unsigned_ints() = default;

	inline unsigned_ints(unsigned int first, unsigned int second)
		: first(first), second(second)
	{}

	inline unsigned_ints(uint32_t values)
		: first(static_cast<uint16_t>(values >> 16)), second (static_cast<uint16_t>(values))
	{}

	/*inline unsigned_ints(uint64_t values)
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

/// Fonctions pour écrire, lire, ou sauter une valeur dans un flux. 
template<typename T>
void binWrite(std::ostream& os, const T& value);
template<typename T>
void binRead(std::istream& is, T& value);
template<typename T>
void binSkip(std::istream& is);

/*
/// Fonctions pour écrire, lire, ou sauter un entier dans un flux.
template<>
void binWrite<unsigned int>(std::ostream& os, const unsigned int& value);
template<>
void binRead<unsigned int>(std::istream& is, unsigned int& value);
template<>
void binSkip<unsigned int>(std::istream& is);

/// Fonctions pour écrire, lire, ou sauter une chaîne de caractères dans un flux.
template<>
void binWrite<std::string>(std::ostream& os, const std::string& text);
template<>
void binRead<std::string>(std::istream& is, std::string& text);
template<>
void binSkip<std::string>(std::istream& is);
*/

/// Fonctions pour écrire, lire, ou sauter un std::vector dans un flux.
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

/// Fonctions pour écrire, lire, ou sauter une std::map dans un flux.
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
