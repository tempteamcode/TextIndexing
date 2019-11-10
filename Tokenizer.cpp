#include "Tokenizer.h"

#include "utility.h"


Tokenizer::Tokenizer() {
	initializeLUTs();
	initializeSeparators();
}

void Tokenizer::initializeLUTs() {
	for (char c = '0'; c <= '9'; c++)
		isValidTextChar[c] = true;
	for (char c = 'a'; c <= 'z'; c++)
		isValidTextChar[c] = true;
	for (char c = 'A'; c <= 'Z'; c++)
		isValidTextChar[c] = true;
}

void Tokenizer::initializeSeparators()
{
	separators = {". ", "; ", ", ", "-", ":", "?", "!", "(", ")", "[", "]", "\"", "'", ":", " ", "\n", "\t"};
}

bool Tokenizer::isNextTokenASeparator( std::istream & is, std::string & firstMatchingSep ) {
	
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

Token_t Tokenizer::extractToken(std::istream & is) {

	// Check for previously registered separators
	{
		std::string tokenStr;
		if( isNextTokenASeparator( is, tokenStr ) ) {
			is.ignore( tokenStr.length() );
			return Token_t(tokenStr, Token_t::SEPARATOR);
		}
	}

	char c;
	if (!is || !is.get(c)) throw custom_exception::empty; // no token

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


std::string PreprocessorTagsData::separateTagsData(std::istream& is)
{
	std::string result;

	if (!is || status == status_t::eof) throw custom_exception::empty; // no tag or data

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

bool Tokenizer::extract(const std::string& data, const string_view& range, string_view& result)
{
	std::string::const_iterator pos = range.begin;
	for (;;)
	{
		if (pos == range.end) return false; // no token
		if (isValidTextChar[*pos]) break;
		if (isValidSingleChar[*pos])
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
		if (!isValidTextChar[*pos]) break;
	}
	result.end = pos;

	return true;
}
