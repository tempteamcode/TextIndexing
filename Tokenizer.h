#pragma once

#include <string>
#include <sstream>

#include "utility.h"

/**
* separate tags from data
**/
class PreprocessorTagsData
{
public:
	std::string separateTagsData(std::istream& is);

private:
	enum class status_t { data, tag, eof } status = status_t::data;
};

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

/**
* extract the tokens of a string
**/
class Tokenizer
{
public:
	Tokenizer();

	/**
	 * extract a token from a stream
	 * @param is the stream
	 * @return the first token of the stream
	 */
	Token_t extractToken(std::istream & is);

	bool Tokenizer::extract(const std::string& data, const string_view& range, string_view& result);

	bool isNextTokenASeparator(std::istream & is, std::string & sep);

private:
	std::vector<std::string> separators;

	/// Look up table of valid litteral tokens
	bool isValidTextChar[256] = {};
	bool isValidSingleChar[256] = {};

	void initializeLUTs();
	void initializeSeparators();
};
