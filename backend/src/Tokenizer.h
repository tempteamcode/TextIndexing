#pragma once

#include <string>
#include <sstream>

#include "utility.h"

/**
* Préprocesseur XML permettant de séparer les balises de leur contenu.
* Pour l'utiliser, il faut faire plusieurs appels successifs à la méthode "separateTagsData" en lui fournissant un flux XML.
* Si le flux commence par une balise, cette balise sera retournée, sinon, toutes les données jusqu'à la prochaine balise seront retournées.
**/
class PreprocessorTagsData
{
public:
	std::string separateTagsData(std::istream& is);

private:
	enum class status_t { data, tag, eof } status = status_t::data;
};

/// représentation d'un token quelconque (mot, séparateur, nombre)
struct Token_t {
	
	enum TokenType_t {
		SEPARATOR,
		LITERAL,
		NUMBER,
		MONEY,
		PERCENTAGE,
	};
	
	std::string word;
	double value;
	TokenType_t type;
	
	Token_t(const std::string& word, TokenType_t type)
		: word(word), type(type)
	{}

	Token_t(const std::string& number, double value, TokenType_t type)
		: word(number), value(value), type(type)
	{}

};

/**
* La classe Tokenizer permet d'extraire les token d'un flux de texte.
* Des appels successifs à la méthode "extractToken" retourneront les tokens successifs d'une chaîne de caractères.
* Un appel à la méthode "extract" retournera une string_view sur le premier token d'une chaîne de caractères.
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

	bool extract(const string_view& data, string_view& result);

	bool isNextTokenASeparator(std::istream& is, std::string& separator);

private:
	/// look up table of valid litteral characters
	bool isCharValidText[256] = {};
	/// look up table of valid single char tokens
	bool isCharValidToken[256] = {};
	/// array of valid separators
	std::vector<std::string> separators;

	void initializeLUTs();
	void initializeSeparators();
};
