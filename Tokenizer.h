#pragma once

#include <string>
#include <sstream>


struct Token {
	
	enum TokenType {
		LITERAL,
		NUMBER,
		MONEY,
		PERCENTAGE,
		SEPARATOR,
		TAG,
		ENDOFFILE
	};

	std::string word;
	double numberValue;
	char numberUnit;

	TokenType type;

	Token( )
		: word(""), type(ENDOFFILE)
	{}

	Token(std::string & word, TokenType type)
		: word(word), type(type) 
	{}

	Token(double numberValue, char numberUnit = ' ', TokenType type = NUMBER)
		: numberValue(numberValue), numberUnit(numberUnit), type(type)
	{}

};

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
	Token extractToken( std::istream & is );

	std::string extract(std::istream& is);

	bool isValidTextChar( const char c );
	bool isAlpha( const char c );

	bool isNextTokenASeparator( std::istream & is, std::string & sep );

private:
	std::vector< std::string > separators;
	
	/// Look up table of valid litteral tokens
	bool validTextCharLUT[256] = {};
	void initializeLUT( );
	void initializeSeparators();
};



// ========= Inline definitions

inline Tokenizer::Tokenizer() {
	initializeLUT();
}

inline void Tokenizer::initializeLUT() {
	for( char c = '0'; c <= '9'; c++ )
		validTextCharLUT[c] = true;
	for( char c = 'a'; c <= 'z'; c++ )
		validTextCharLUT[c] = true;
	for( char c = 'A'; c <= 'Z'; c++ )
		validTextCharLUT[c] = true;
}

inline void Tokenizer::initializeSeparators() {

	separators.clear();

	// Two chars seps
	separators.push_back( ". ");
	separators.push_back( "; ");
	separators.push_back( ", ");

	// Single char seps
	separators.push_back( "-" );
	separators.push_back( ":" );
	separators.push_back( "?" );
	separators.push_back( "!" );
	separators.push_back( "(" );
	separators.push_back( ")" );
	separators.push_back( "[" );
	separators.push_back( "]" );	
	separators.push_back( "\"" );
	separators.push_back( "'" );
	separators.push_back( ":" );
	separators.push_back( " " );
	separators.push_back( "\n" );
	separators.push_back( "\t" );

}

inline bool Tokenizer::isValidTextChar( const char c ) {
	return validTextCharLUT[ c ];
}

inline bool Tokenizer::isAlpha( const char c ) {
	return ( 'a' <= c && 'z' >= c ) || ( 'A' <= c && 'Z' >= c );
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

inline Token Tokenizer::extractToken( std::istream & is ) {

	// Check for previously registered separators
	{
		std::string tokenStr;
		if( isNextTokenASeparator( is, tokenStr ) ) {
			is.ignore( tokenStr.length() );
			return Token( tokenStr, Token::SEPARATOR );
		}
	}

	char c = { 0 };
	if( !is || !is.get(c) ) return Token();

	// If it's a tag opener
	if( c == '<' ) {

		std::ostringstream tokenSs;
		
		auto nextChar = is.peek();
		if( !isAlpha( nextChar )  ) 
			return Token(c, Token::SEPARATOR );

		while( is.get(c) && (isValidTextChar(c) || c == '-') ) {
			tokenSs << c;
		}

		while( is.get(c) && c != '>' ){} // Skip unuseful token in tag element
		auto tokenStr = tokenSs.str(); 
		return Token( tokenStr, Token::TAG );

	}

	{
		std::ostringstream tokenSs;
		tokenSs << c;
		while( c = is.peek() ) {
			if( isValidTextChar(c) ) {
				tokenSs << c;
				is.get(c);
				continue;
			}
			std::string tokenStr;
			if( isNextTokenASeparator( is, tokenStr ) ) {
				is.ignore( tokenStr.length() );
				return Token( tokenStr, Token::SEPARATOR );
			} else {
				tokenSs << c;
				is.get(c);
				continue;
			}

		}
		auto tokenStr = tokenSs.str();

		// Add something to separate pure literal from numbers, or meaningful numbers
		return Token( tokenStr, Token::LITERAL );
	}
}


inline std::string Tokenizer::extract(std::istream& is)
{
	char c;

	while (is.get(c) && !isValidTextChar(c)) {
		if (c == '<') break;
		if (c == ';') return std::string{c};
	}

	if (!is) return std::string{};
	
	std::ostringstream token;
	
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
			if (!isValidTextChar(c)) break;
		}
	}

	return token.str();
}
