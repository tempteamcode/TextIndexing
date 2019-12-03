#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "DocumentExtractor.h"
#include "Tokenizer.h"
#include "utility.h"

typedef std::unordered_map<std::string, unsigned int> TFC_t;

/// Structure représentant un le contenu d'un document (un article de journal).
struct DocumentData_t
{
	std::string DOCNO;
	unsigned int DOCID = 0;
	Date_t DATE;
	// SECTION
	unsigned int LENGTH = 0;
	TFC_t HEADLINE;
	// BYLINE
	// DATELINE
	TFC_t TEXT;
	std::list<TFC_t> GRAPHICs;
	// TYPE
	TFC_t SUBJECT;
	// CORRECTION-DATE
	// CORRECTION
};

/// Structure représentant le contenu d'un document (un article de journal) destiné à être converti en JSON.
struct DocumentJSON_t
{
	Date_t date;
	std::string title;
	std::vector<std::string> contents;
};


/// Effectue les étapes de "stemming" et "stopword removal" sur un mot. Renvoit false si le mot doit être supprimé.
bool stemming_stopword(std::string& word);

/// Affiche les balises d'un arbre, à n'utiliser qu'à des fins de débuggage uniquement.
void print_tags(const DocumentTree_t& tag, const std::string& indent);

/**
* Extraie les tokens d'une chaîne (assez efficacement car aucune recopie de caractères n'est effectuée).
* Retourne un std::vector contenant des string_view sur tous les tokens extraits.
**/
std::vector<string_view> extractTokens(const std::string& data);

/**
* Extraie les tokens d'une chaîne (assez efficacement car aucune recopie de caractères n'est effectuée).
* Appelle une fonction callback sur chaque token extrait.
**/
template<class callback_t>
void extractTokens(const std::string& data, callback_t callback)
{
	Tokenizer tokenizer;
	string_view range(data);

	for (;;)
	{
		string_view part;
		if (!tokenizer.extract(range, part)) break;

		callback(part);
		range.begin = part.end;
	}
}

/// Extrait un document (un article de journal) d'un arbre de document XML.
void extractDocumentData(DocumentTree_t& document_tree, DocumentData_t& document);

/// Extrait un document (un article de journal) destiné à être converti en JSON d'un arbre de document XML.
void extractDocumentJSON(DocumentTree_t& document_tree, DocumentJSON_t& documentJSON);

/**
* Consomme les documents (articles de journal) d'un arbre de document XML.
* Appelle une fonction callback sur chaque document trouvé, et les supprime progressivement.
**/
template<class callback_t>
void consumeDocuments(DocumentTree_t& documents, callback_t callback)
{
	for (auto it = documents.tags.begin(); it != documents.tags.end(); it = documents.tags.erase(it))
	{
		if ((*it).name == "DOC")
		{
			callback(*it);
		}
	}
}

/// Retourne la représentation en JSON d'un document (un article de journal) destiné à être converti en JSON.
std::ostream& operator <<(std::ostream &os, const DocumentJSON_t& document);
