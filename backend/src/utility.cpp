#include "utility.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

const std::vector<std::string> names_months = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
const std::vector<std::string> names_weekdays = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

Date_t::operator std::string() const
{
	std::ostringstream formatted;
	formatted << std::setfill('0') << std::setw(4) << year;
	formatted << std::setfill('0') << std::setw(2) << int(month);
	formatted << std::setfill('0') << std::setw(2) << day;
	return formatted.str();
}

bool operator ==(const string_view& str_view, const char* text)
{
	auto it = str_view.begin;
	for (;;)
	{
		bool text_end = (*text == '\0');
		bool view_end = (it == str_view.end);
		if (text_end || view_end) return (text_end && view_end);
		if ((*it++) != (*text++)) return false;
	}
}

bool tryParseInt(const string_view& chars, unsigned int& value)
{
	value = 0;

	for (auto it = chars.begin; it != chars.end; ++it)
	{
		char c = (*it);
		if (c < '0' || c > '9') return false;

		value = value * 10 + (c - '0');
	}

	return true;
}

bool tryParseInt(const std::string& chars, unsigned int& value)
{
	value = 0;

	for (char c : chars)
	{
		if (c < '0' || c > '9') return false;

		value = value * 10 + (c - '0');
	}

	return true;
}

bool tryParseDate(const std::vector<std::string>& words, Date_t& date)
{
	if (words.size() < 4) return false;

	std::string name_month = words[0]; toLowerCase(name_month);
	auto it_month = std::find(names_months.cbegin(), names_months.cend(), name_month);
	if (it_month == names_months.cend()) return false;
	date.month = static_cast<Date_t::month_t>(it_month - names_months.cbegin() + 1);

	unsigned int day_number;
	if (!tryParseInt(words[1], day_number)) return false;
	date.day = static_cast<unsigned char>(day_number);

	unsigned int year_number;
	if (!tryParseInt(words[2], year_number)) return false;
	date.year = static_cast<unsigned short>(year_number);
	
	std::string name_weekday = words[3]; toLowerCase(name_weekday);
	auto it_weekday = std::find(names_weekdays.cbegin(), names_weekdays.cend(), name_weekday);
	if (it_weekday == names_weekdays.cend()) return false;
	date.weekday = static_cast<Date_t::weekday_t>(it_weekday - names_weekdays.cbegin() + 1);

	return true;
}

std::string stringJoin(const std::vector<std::string>& data, const std::string& sep)
{
	size_t totalsize = (data.size() + 1) * sep.size();
	for (const std::string& str : data) totalsize += str.size();

	std::string result;
	result.reserve(totalsize);

	for (const std::string& str : data)
	{
		result += str;
		result += sep;
	}

	if (!result.empty()) result.resize(result.size() - 1 * sep.size());
	return result;
}

std::string stringJoin(const std::list<std::string>& data, char sep)
{
	size_t totalsize = data.size() + 1;
	for (const std::string& str : data) totalsize += str.size();
	
	std::string result;
	result.reserve(totalsize);

	for (const std::string& str : data)
	{
		result += str;
		result += sep;
	}

	if (!result.empty()) result.resize(result.size() - 1);
    return result;
}
