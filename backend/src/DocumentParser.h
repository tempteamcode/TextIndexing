#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "DocumentExtractor.h"
#include "Tokenizer.h"
#include "utility.h"

typedef std::unordered_map<std::string, unsigned int> TFC_t;

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

struct DocumentJSON_t
{
	Date_t date;
	std::string title;
	std::vector<std::string> contents;
};


bool stemming_stopword(std::string& word);
 
void print_tags(const DocumentTree_t& tag, std::string indent);

std::vector<string_view> extractTokens(const std::string& data);

template<class callback_t>
void extractTokens(const std::string& data, callback_t callback)
{
	Tokenizer tokenizer;
	string_view range(data);

	for (;;)
	{
		string_view part;
		if (!tokenizer.extract(data, range, part)) break;

		callback(part);
		range.begin = part.end;
	}
}

void extractDocumentData(DocumentTree_t& document_tree, DocumentData_t& document);

void extractDocumentJSON(DocumentTree_t& document_tree, DocumentJSON_t& documentJSON);

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
