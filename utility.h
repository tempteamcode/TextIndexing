#pragma once

#include <vector>
#include <string>

static const std::vector<std::string> names_months = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
static const std::vector<std::string> names_weekdays = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};

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

void toLowerCase(std::string& text)
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

inline bool tryParseInt(const std::string& chars, unsigned int& value)
{
	value = 0;

	for (char c : chars)
	{
		if (c < '0' || c > '9') return false;

		value = value * 10 + (c - '0');
	}

	return true;
}

inline bool tryParseDate(const std::vector<std::string>& words, Date_t& date)
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
