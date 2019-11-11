#pragma once

#include <list>
#include <vector>
#include <string>

enum class custom_exception
{
	fs_access,
	parsing_error,
	no_tagordata,
	no_token,
};

extern const std::vector<std::string> names_months;
extern const std::vector<std::string> names_weekdays;

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
};

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

inline bool isCharAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool isCharNumeric(char c) {
	return (c >= '0' && c <= '9');
}

bool operator ==(const string_view& str_view, const char* text);

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

bool tryParseInt(const string_view& chars, unsigned int& value);

bool tryParseInt(const std::string& chars, unsigned int& value);

bool tryParseDate(const std::vector<std::string>& words, Date_t& date);

/**
* joins several strings into one with linear complexity
* the resulting string is allocated with its final size to avoid reallocation when appending each substring
**/
std::string stringJoin(const std::list<std::string>& data, char sep = ' ');
