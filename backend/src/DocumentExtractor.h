#pragma once

#include <list>
#include <string>

/// Racine ou balise d'un arbre contenant des documents, ayant un nom, des sous-balises, et des données.
struct DocumentTree_t
{
	DocumentTree_t* parent;
	std::string name;
	std::list<DocumentTree_t> tags;
	std::string data;

	void simplify();
	bool empty() const;
	void clear();

    DocumentTree_t() = default;
	DocumentTree_t(const DocumentTree_t&) = default;
    DocumentTree_t & operator=(const DocumentTree_t&) = default;
};

/** Cette classe permet de reconstruire le DocumentTree_t décrit par un texte (au format XML).
* Pour ceci, il faut utiliser plusieurs fois sa méthode "parseTagOrData" en lui fournissant à chaque appel soit une balise seule, soit des données sans balises.
* (la classe PreprocessorTagsData du Tokenizer permet de séparer les balises des données d'un document XML)
* La méthode "getDocument" retourne ensuite l'arbre complet généré.
**/
class DocumentExtractor
{
public:
	void parseTagOrData(const std::string& tagordata);

	DocumentTree_t& getDocument();
	void clearDocument();

private:
	DocumentTree_t tree;
	DocumentTree_t* node = &tree;

	std::list<std::string> part;
};
