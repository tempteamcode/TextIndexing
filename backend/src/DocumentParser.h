#pragma once

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

bool extractDocumentData(DocumentTree_t& documents, DocumentData_t& document);

template<class callback_t>
void extractDocuments(DocumentTree_t& documents, callback_t callback)
{
	DocumentData_t document;

	for (;;)
	{
		if (!extractDocumentData(documents, document)) break;

		callback(document);
	}
}
