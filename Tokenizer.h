#pragma once

#include <string>
#include <sstream>


/**
* Extract the tokens of a string.
**/
class Tokenizer
{
public:
	Tokenizer();

	std::string extract(std::istream& is);

private:
	/// Look up table of valid litteral tokens
	bool char_is_text[256] = {};
};

struct Token {
	
	enum TokenType {
		LITERAL,
		NUMBER,
		MONEY,
		PERCENTAGE,
		PUNCTUATION,
		TAG
	};

	std::string word;
	double numberValue;
	char numberUnit;

	TokenType type;

	Token(std::string & word, TokenType type)
		: word(word), type(type) 
	{}

	Token(double numberValue, char numberUnit = ' ', TokenType type = NUMBER)
		: numberValue(numberValue), numberUnit(numberUnit), type(type)
	{}

};





inline Tokenizer::Tokenizer()
{
	for (char c = '0'; c <= '9'; c++)
		char_is_text[c] = true;

	for (char c = 'a'; c <= 'z'; c++)
		char_is_text[c] = true;

	for (char c = 'A'; c <= 'Z'; c++)
		char_is_text[c] = true;
}

inline std::string Tokenizer::extract( std::istream& is )
{
	std::ostringstream token;

	char c = { 0 };
	while (is.get(c) && !char_is_text[c] && c != '<')
	{
		if (c == ';') return std::string{ c };
	}
	if (is)
	{
		if (c == '<')
		{
			for (;;)
			{
				token << c;
				if (c == '>') break;
				if (!is.get(c)) break;
			}
		}
		else
		{
			for (;;)
			{
				token << c;
				if (!is.get(c)) break;
				if (!char_is_text[c]) break;
			}
		}
	}

	return token.str();
}
