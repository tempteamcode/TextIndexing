/**
 * @file utility.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief Utility functions, classes and enums.
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 * This file contains various utility functions, classes and enums we use for ou text indexation project.
 */
#pragma once

#include <map>
#include <list>

#include <vector>
#include <string>

/**
 * @enum custom_exception
 * @brief  Custom exception typed enumeration we use in this project
 */
enum class custom_exception
{
	fs_access,
	parsing_error,
	no_tagordata,
	no_token,
};

extern const std::vector<std::string> names_months;
extern const std::vector<std::string> names_weekdays;

/**
 * @struct Date_t
 * @brief Date_t structure represents a simple year/month/day/weekday date type with typed enums for months and weekdays.
 * 
 * Date_t structures can be parsed using tryParseDate(const std::vector<std::string>&, Date_t&) utility function.
 * @todo: make Date_t const/immutable? (see https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rconst-immutable)
 */
struct Date_t
{
	enum class month_t : unsigned char {unknown = 0, january, february, march, april, may, june, july, august, september, october, november, december};
	enum class weekday_t : unsigned char {unknown = 0, monday, tuesday, wednesday, thursday, friday, saturday, sunday};

	unsigned short year = 0;
	month_t month = month_t::unknown;
	unsigned char day = 0;
	weekday_t weekday = weekday_t::unknown;

	bool empty() const
	{
		return ((year | static_cast<unsigned char>(month) | day | static_cast<unsigned char>(weekday)) == 0);
	}

	operator std::string() const;
};

/**
 * @class string_view
 * @brief Conveniance class warping a std::string into an immutable view of it
 * @todo Use C++ 17 std::string_view instead?
 */
class string_view
{
public:
	std::string::const_iterator begin;
	std::string::const_iterator end;
	
	string_view() = default;

	string_view(const std::string& str)
		: begin(str.cbegin()), end(str.cend())
	{}
	
	string_view(std::string::const_iterator begin, std::string::const_iterator end)
		: begin(begin), end(end)
	{}

	inline std::string str() const
	{
		return std::string(begin, end);
	}

	inline explicit operator std::string() const
	{
		return str();
	}
};

/**
 * @remark only suports regular/english letters, won't consider letters like 'é', 'ç', ... to be alpha characters
 */
inline bool isCharAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool isCharNumeric(char c) {
	return (c >= '0' && c <= '9');
}

inline bool streq(const char* str1, const char* str2)
{
	for (;;)
	{
		if (*str1 != *str2) return false;
		if (*str1 == '\0') return true;
		str1++; str2++;
	}
}

/**
 * @brief == operator overload
 * @relatesalso string_view
 */
bool operator ==(const string_view& str_view, const char* text);

/**
 * @brief Returns a string_view to a striped version (removes all preceding or trailing whitespaces) of a given std::string const-ref.
 * @relatesalso string_view
 */
string_view strstrip(const std::string& text);

/**
 * @brief Lower case a given std:string reference
 * @remark only suports regular/english letters, won't lower case to letters like 'é', 'ç', ...
 * @param text std::string to be transformed to lower case 
 */
inline void toLowerCase(std::string& text)
{
	for (unsigned int i = 0, len = text.length(); i < len; i++)
	{
		char c = text[i];
		if (c >= 'A' && c <= 'Z')
		{
			text[i] = c - 'A' + 'a';
		}
	}
}

/**
 * @brief Upper case a given std:string reference
 * @remark only suports regular/english letters, won't lower case to letters like 'é', 'ç', ...
 * @param text std::string to be transformed to upper case 
 */
inline void toUpperCase(std::string& text)
{
	for (unsigned int i = 0, len = text.length(); i < len; i++)
	{
		char c = text[i];
		if (c >= 'a' && c <= 'z')
		{
			text[i] = c - 'a' + 'A';
		}
	}
}

/**
 * @brief Try to parse an unsigned integer value from a given string_view
 * 
 * @param[in] chars string_view to be parsed
 * @param[out] value A reference to the parsed unsigned integer value
 * @return A boolean indicating whether if a unsigned int value have been sucessfully parsed
 * @relatesalso string_view
 */
bool tryParseInt(const string_view& chars, unsigned int& value);

/**
 * @brief Try to parse an unsigned integer value from a given std::string
 * 
 * @param[in] chars std::string to be parsed.
 * @param[out] value A reference to the parsed unsigned integer value
 * @return A boolean indicating whether if a unsigned int value have been sucessfully parsed
 */
bool tryParseInt(const std::string& chars, unsigned int& value);

/**
 * @brief Parses a Date_t date from a given vector of string words
 * 
 * @param[in] words Vector of words to be parsed
 * @param[out] date A reference to the parsed Date_t date
 * @return A boolean indicating whether if a Date_t date have been sucessfully parsed
 * @relatesalso Date_t
 */
bool tryParseDate(const std::vector<std::string>& words, Date_t& date);

/**
 * @brief Joins several strings into one with linear complexity
 * @param data 
 * @param sep Separator string
 * @return The resulting joined std::string
 * @remark The resulting string is allocated with its final size to avoid reallocation when appending each substring.
 */
std::string stringJoin(const std::vector<std::string>& data, const std::string& sep);
std::string stringJoin(const std::list<std::string>& data, char sep = ' ');

/**
 * @brief Template function returning the intersection of two given std::map. Key and Value template parameters of boths std::map should be the same.
 * 
 * @tparam Key, Value Key and Value typenames stored by input std:map lhs and rhs
 * @tparam MergedValue, Aggregator Aggregator template parameter defines how two values of type Value are agregated into a value of type MergedValue
 * @param lhs, rhs Input maps to be merged
 * @return std::map<Key,MergedValue> map intersection which associates common lhs and rhs keys with an aggregated value of type MergedValue
 */
template <typename Key, typename Value, typename MergedValue, class Aggregator>
std::map<Key,MergedValue> mapIntersect(const std::map<Key,Value>& lhs, const std::map<Key,Value>& rhs)
{
	std::map<Key,MergedValue> result;
	for (auto itl = lhs.begin(), itr = rhs.begin(), endl = lhs.end(), endr = rhs.end(); itl != endl && itr != endr; )
	{
		if (itl->first == itr->first)
		{
			result[itl->first] = Aggregator(itl->second, itr->second);
			++itl; ++itr;
		}
		else
		{
			if (itl->first < itr->first)
				++itl;
			else
				++itr;
		}
	}
	return result;
}

/**
 * @brief Template function returning the intersection of two given std::map. Key and Value template parameters of boths std::map should be the same.
 * 
 * @tparam Key, Value Key and Value typenames stored by input std:map lhs and rhs
 * @tparam MergedValue, Aggregator_t Aggregator_t typename must be a functor or function pointer type returning a MergedValue type (or a return type impliclty castable to MergedValue type).
 * @param lhs, rhs Input maps to be merged
 * @param aggregator Aggregation function pointer or functor wich takes Value values from lhs and rhs and returns an aggregated value of MergedValue type
 * @return std::map<Key,MergedValue> map intersection which associates common lhs and rhs keys with an aggregated value of type MergedValue
 */
template <typename Key, typename Value, typename MergedValue, class Aggregator_t>
std::map<Key, MergedValue> mapIntersection(const std::map<Key, Value>& lhs, const std::map<Key, Value>& rhs, Aggregator_t aggregator)
{
	std::map<Key, MergedValue> result;
	auto endl = lhs.cend();
	auto endr = rhs.cend();
	for (auto itl = lhs.cbegin(), itr = rhs.cbegin();;)
	{
		if (itl->first == itr->first)
		{
			result[itl->first] = aggregator(itl->second, itr->second);
			if (++itl == endl || ++itr == endr) break;
		}
		else if (itl->first < itr->first)
		{
			if (++itl == endl) break;
		}
		else
		{
			if (++itr == endr) break;
		}
	}
	return result;
}

/**
 * @brief Template function returning the union of two given std::map. Key and Value template parameters of boths std::map should be the same.
 * 
 * @tparam Key, Value Key and Value typenames stored by input std:map lhs and rhs
 * @tparam MergedValue, Aggregator_t Aggregator_t typename must be a functor or function pointer type returning a MergedValue type (or a return type impliclty castable to MergedValue type).
 * @tparam AggregatorUnique_t Function pointer or functor typename, see aggregatorUnique param
 * @param lhs, rhs Input maps to be merged
 * @param aggregator Aggregation function pointer or functor (Aggregator_t) wich takes Value values from lhs and rhs with the same keys and returns an aggregated value of MergedValue type
 * @param aggregatorUnique Function pointer or functor (AggregatorUnique_t) which is used during map union when a Key value only occurs in lhs or rhs, but not both (Aggregator_t aggregator called instead)
 * @return std::map<Key,MergedValue> map union which contains all keys occuring in lhs and rhs, mapping these keys to aggregated MergedValue from aggregator or aggregatorUnique
 */
template <typename Key, typename Value, typename MergedValue, class Aggregator_t, class AggregatorUnique_t>
std::map<Key, MergedValue> mapUnion(const std::map<Key, Value>& lhs, const std::map<Key, Value>& rhs, Aggregator_t aggregator, AggregatorUnique_t aggregatorUnique)
{
	std::map<Key, MergedValue> result;
	auto itl = lhs.cbegin(), endl = lhs.cend();
	auto itr = rhs.cbegin(), endr = rhs.cend();
	for (;;)
	{
		if (itl->first == itr->first)
		{
			result[itl->first] = aggregator(itl->second, itr->second);
			++itl;
			++itr;
			if (itl == endl || itr == endr) break;
		}
		else if (itl->first < itr->first)
		{
			result[itl->first] = aggregatorUnique(itl->second);
			if (++itl == endl) break;
		}
		else if (itr->first < itl->first)
		{
			result[itr->first] = aggregatorUnique(itr->second);
			if (++itr == endr) break;
		}
	}
	for (; itl != endl; ++itl) result[itl->first] = aggregatorUnique(itl->second);
	for (; itr != endr; ++itr) result[itr->first] = aggregatorUnique(itr->second);
	return result;
}
