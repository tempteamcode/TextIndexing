#include "Tokenizer.h"

#include "utility.h"


Tokenizer::Tokenizer() {
	initializeLUTs();
	initializeSeparators();
}

void Tokenizer::initializeLUTs() {
	for (char c = '0'; c <= '9'; c++)
		isCharValidText[(int)c] = true;
	for (char c = 'a'; c <= 'z'; c++)
		isCharValidText[(int)c] = true;
	for (char c = 'A'; c <= 'Z'; c++)
		isCharValidText[(int)c] = true;

	// for (char c : {"-", ":", "?", "!", "(", ")", "[", "]", "\"", "'", ":", " ", "\n", "\t"})
	//	isCharValidToken[c] = true;
}

void Tokenizer::initializeSeparators()
{
	separators = {". ", "; ", ", ", "-", ":", "?", "!", "(", ")", "[", "]", "\"", "'", ":", " ", "\n", "\t"};
}

inline bool starts_with(const std::string& fullstr, const std::string& substr)
{
	return (fullstr.substr(0, substr.length()) == substr);
}

bool Tokenizer::isNextTokenASeparator(std::istream& is, std::string& separator) {
	// gets the length of the longest separator
	unsigned int charnb = 0;
	for (const std::string& sep : separators)
		if (sep.length() > charnb) charnb = sep.length();

	// gets the next chars of the stream
	std::string streamchars;
	for (unsigned int i = 0; i < charnb; i++)
	{
		char c;
		if (!is.get(c)) break;
		streamchars.push_back(c);
	}
	charnb = streamchars.length();
	while (charnb --> 0) is.unget();
	//TODO: avoid using unget since it's not guaranteed to succeed

	// checks if the next chars of the stream start with a separator
	for (const std::string& sep : separators)
	{
		if (starts_with(streamchars, sep))
		{
			separator = sep;
			return true;
		}
	}
	return false;
}

Token_t Tokenizer::extractToken(std::istream & is)
{
	if (!is || (is.peek() == EOF)) throw custom_exception::no_token;

	{
		std::ostringstream tokenSs;

		for (char c; (c = is.peek()) != EOF; tokenSs << c)
		{
			std::string separator;
			if (!isCharValidText[(int)c] && isNextTokenASeparator(is, separator))
			{
				is.ignore(separator.length());
				return Token_t(separator, Token_t::SEPARATOR);
			}

			is.ignore(1);
		}
		auto tokenStr = tokenSs.str();

		// Add something to separate pure literal from numbers, or meaningful numbers
		return Token_t( tokenStr, Token_t::LITERAL );
	}
}


std::string PreprocessorTagsData::separateTagsData(std::istream& is)
{
	std::string result;

	if (!is || status == status_t::eof) throw custom_exception::no_tagordata;

	do {

	if (status == status_t::tag)
	{
		std::string tag;
		getline(is, tag, '>');
		status = status_t::data;
		return '<' + tag + '>';
	}
	else if (status == status_t::data)
	{
		for (;;)
		{
			std::string text;
			getline(is, text, '<');
			bool reachedEOF = is.eof();
			result += text;

			int next = is.peek();
			if (next != EOF && (isCharAlpha(static_cast<char>(next)) || static_cast<char>(next) == '/'))
			{
				status = status_t::tag;
				break;
			}

			if (!reachedEOF) result += '<';
			if (next == EOF)
			{
				status = status_t::eof;
				return result;
			}
		}
	}

	} while (result.empty());

	return result;
}

bool Tokenizer::extract(const std::string& data, const string_view& range, string_view& result)
{
	std::string::const_iterator pos = range.begin;
	for (;;)
	{
		if (pos == range.end) return false; // no token
		if (isCharValidText[(int)(*pos)]) break;
		if (isCharValidToken[(int)(*pos)])
		{
			result.begin = pos;
			result.end = pos + 1;
			return true;
		}

		++pos;
	}

	result.begin = pos;
	for (;;)
	{
		++pos;
		if (pos == range.end) break;
		if (!isCharValidText[(int)(*pos)]) break;
	}
	result.end = pos;

	return true;
}
