#pragma once

#include <string>
#include <sstream>

#include "utility.h"


struct Token_t {
	
	enum TokenType_t {
		SEPARATOR,
		LITERAL,
		NUMBER,
		MONEY,
		PERCENTAGE,
	};
	
	enum NumberUnit_t : char {
		none = ' ',
		percentage = '%',
		dollar = '$',
		euro = '€',
	};

	std::string word;
	double numberValue;
	
	NumberUnit_t numberUnit;

	TokenType_t type;

	Token_t(std::string & word, TokenType_t type)
		: word(word), type(type) 
	{}

	Token_t(double numberValue, NumberUnit_t numberUnit = none, TokenType_t type = NUMBER)
		: numberValue(numberValue), numberUnit(numberUnit), type(type)
	{}

};

class string_view;

/**
* Extract the tokens of a string.
**/
class Tokenizer
{
public:
	Tokenizer();

	/**
	 * Extract a token from a stream
	 * @param is the stream
	 * @return the first token of the stream
	 */
	Token_t extractToken(std::istream & is);

	string_view Tokenizer::extract(const std::string& data, const string_view& range);

	bool isNextTokenASeparator(std::istream & is, std::string & sep);

private:
	std::vector<std::string> separators;

	/// Look up table of valid litteral tokens
	bool isValidTextChar[256] = {};
	bool isValidSingleChar[256] = {};

	void initializeLUTs();
	void initializeSeparators();
};



// ========= Inline definitions

inline bool isCharAlpha(const char c) {
	return ( 'a' <= c && 'z' >= c ) || ( 'A' <= c && 'Z' >= c );
}

inline Tokenizer::Tokenizer() {
	initializeLUTs();
	initializeSeparators();
}

inline void Tokenizer::initializeLUTs() {
	for (char c = '0'; c <= '9'; c++)
		isValidTextChar[c] = true;
	for (char c = 'a'; c <= 'z'; c++)
		isValidTextChar[c] = true;
	for (char c = 'A'; c <= 'Z'; c++)
		isValidTextChar[c] = true;
}

inline void Tokenizer::initializeSeparators()
{
	separators = {". ", "; ", ", ", "-", ":", "?", "!", "(", ")", "[", "]", "\"", "'", ":", " ", "\n", "\t"};
}

inline bool Tokenizer::isNextTokenASeparator( std::istream & is, std::string & firstMatchingSep ) {
	
	for( auto & sep : separators ) {

		unsigned int sepCharCount = 0;
		bool matching = true;

		for( char & sepChar : sep ) {
			char streamChar;
			if( is.get( streamChar ) ) {
				matching = false;
				break;
			};
			sepCharCount++;
			if( streamChar != sepChar ) {
				matching = false; 
				break;
			};
		}
		
		while( sepCharCount-- >= 0 ) is.unget();

		if( !matching ) continue;
		
		// Separator matched		
		firstMatchingSep = sep;
		return true;
	}

	return false;

}

inline Token_t Tokenizer::extractToken(std::istream & is) {

	// Check for previously registered separators
	{
		std::string tokenStr;
		if( isNextTokenASeparator( is, tokenStr ) ) {
			is.ignore( tokenStr.length() );
			return Token_t(tokenStr, Token_t::SEPARATOR);
		}
	}

	char c;
	if (!is || !is.get(c)) throw 0; // no token

	{
		std::ostringstream tokenSs;
		tokenSs << c;
		while( c = is.peek() ) {
			if( isValidTextChar[c] ) {
				tokenSs << c;
				is.get(c);
				continue;
			}
			std::string tokenStr;
			if( isNextTokenASeparator( is, tokenStr ) ) {
				is.ignore( tokenStr.length() );
				return Token_t( tokenStr, Token_t::SEPARATOR );
			} else {
				tokenSs << c;
				is.get(c);
				continue;
			}

		}
		auto tokenStr = tokenSs.str();

		// Add something to separate pure literal from numbers, or meaningful numbers
		return Token_t( tokenStr, Token_t::LITERAL );
	}
}


class PreprocessorTagsData
{
public:
	std::string separateTagsData(std::istream& is);

private:
	enum class status_t { data, tag, eof } status = status_t::data;
};

inline std::string PreprocessorTagsData::separateTagsData(std::istream& is)
{
	std::string result;

	if (!is || status == status_t::eof) throw 0; // no tag or data

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

			char next;
			next = is.peek();
			if (next != EOF && (isCharAlpha(next) || next == '/'))
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

inline string_view Tokenizer::extract(const std::string& data, const string_view& range)
{
	string_view result;

	std::string::const_iterator pos = range.begin;
	for (;;)
	{
		if (pos == range.end) throw 0; // no token
		if (isValidTextChar[*pos]) break;
		if (isValidSingleChar[*pos])
		{
			result.begin = pos;
			result.end = pos + 1;
			return result;
		}

		++pos;
	}

	result.begin = pos;
	for (;;)
	{
		++pos;
		if (pos == range.end) break;
		if (!isValidTextChar[*pos]) break;
	}
	result.end = pos;

	return result;
}
