#pragma once

#include <string>
#include <sstream>

class Tokenizer
{
public:
	Tokenizer();

	std::string extract_token(std::istream& is);

private:
	bool char_is_text[256] = {};
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

inline std::string Tokenizer::extract_token(std::istream& is)
{
	std::ostringstream token;

	char c = { 0 };
	while (is.get(c) && !char_is_text[c] && c != '<');
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
