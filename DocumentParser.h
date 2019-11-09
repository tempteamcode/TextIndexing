#pragma once

#include "DocumentExtractor.h"

struct DocumentData_t
{
	std::string DOCNO;
	unsigned int DOCID;
	std::string DATE;
	// SECTION
	unsigned int LENGTH;
	std::vector<std::string> HEADLINE;
	// BYLINE
	// DATELINE
	std::vector<std::string> TEXT;
	std::vector<std::vector<std::string>> GRAPHICs;
	// TYPE
	std::vector<std::string> SUBJECT;
	// CORRECTION-DATE
	// CORRECTION
};

unsigned int parseInt_nochecks(const std::string& chars)
{
	unsigned int value = 0;

	for (char c : chars)
	{
		value = value * 10 + (c - '0');
	}

	return value;
}

typedef std::map<std::string, unsigned int> TFID_t;

inline DocumentData_t extractDocumentData(DocumentTree_t& documents)
{
	DocumentTree_t* root = nullptr;

	for (DocumentTree_t& tag : documents.tags)
	{
		if (tag.name == "DOC")
		{
			root = &tag;
			break;
		}
	}

	if (root == nullptr) throw 0; // no document

	DocumentData_t document;

	for (DocumentTree_t& tag : root->tags)
	{
		if (tag.name == "DOCNO")
		{
			if (!document.DOCNO.empty()) std::cerr << "several DOCNO tags in one document" << std::endl;

			if (tag.words.size() == 2)
			{
				document.DOCNO = tag.words[0] + "-" + tag.words[1];
				tag.words.clear();
			}
		}
		else if (tag.name == "DOCID")
		{
			if (tag.words.size() == 1)
			{
				document.DOCID = parseInt_nochecks(tag.words[0]);
				tag.words.clear();
			}
		}
		else if (tag.name == "DATE")
		{
			if (!document.TEXT.empty()) std::cerr << "several DATE tags in one document" << std::endl;
			
			if (tag.tags.size() >= 1 && tag.tags[0].name == "P")
			{
				auto& words = tag.tags[0].words;
				
				for (const std::string& word : tag.words)
				{
					document.DATE += word + ' ';
				}
				if (!document.DATE.empty())
					document.DATE.resize(document.DATE.length() - 1);
				
				tag.tags.erase(tag.tags.begin());
			}

			if (!tag.tags.empty()) continue;
		}
		else if (tag.name == "LENGTH")
		{
			if (tag.tags.size() == 1 && tag.tags[0].name == "P")
			{
				auto& words = tag.tags[0].words;
				if (words.size() == 2 && words[1] == "words")
				{
					document.LENGTH = parseInt_nochecks(words[0]);
					tag.tags.clear();
				}
			}
		}
		else if (tag.name == "HEADLINE")
		{
			continue;
		}
		else if (tag.name == "TEXT")
		{
			// if (!document.TEXT.empty()) std::cerr << "several TEXT tags in one document" << std::endl; // this happens

			std::move(tag.words.begin(), tag.words.end(), std::back_inserter(document.TEXT));
			tag.words.erase(tag.words.begin(), tag.words.end());

			for (auto it = tag.tags.begin(); it != tag.tags.end(); it = tag.tags.erase(it))
			{
				auto& subtag = (*it);
				
				if (subtag.name == "TABLE") continue;

				if (subtag.name == "P")
				{
					std::move(tag.words.begin(), tag.words.end(), std::back_inserter(document.TEXT));
					tag.words.erase(tag.words.begin(), tag.words.end());
				}
				else
				{
					std::cerr << "unknown TEXT subtag " << subtag.name << std::endl;
				}
			}
		}
		else if (tag.name == "P")
		{
			std::move(tag.words.begin(), tag.words.end(), std::back_inserter(document.TEXT));
			tag.words.erase(tag.words.begin(), tag.words.end());
		}
		else if (tag.name == "GRAPHIC")
		{
			continue;
		}
		else if (tag.name == "SUBJECT")
		{
			continue;
		}
		else if (tag.name == "SECTION" || tag.name == "BYLINE" || tag.name == "DATELINE" || tag.name == "TYPE" || tag.name == "CORRECTION-DATE" || tag.name == "CORRECTION")
		{
			tag.tags.clear();
			tag.words.clear();
		}
		else
		{
			std::cerr << "unknown root tag '" << tag.name << "'" << std::endl;
		}

		if (!tag.tags.empty())
		{
			std::cerr << "unknown " << tag.name << " subtag " << tag.tags[0].name << std::endl;
		}
	}

	for (auto it = documents.tags.begin(); it != documents.tags.end(); ++it)
	{
		if (&(*it) == root)
		{
			documents.tags.erase(it);
			break;
		}
	}

	return document;
}

inline void extractDocuments(DocumentTree_t& documents, void (*callback)(DocumentData_t& document))
{
	for (;;)
	{
		try {
			DocumentData_t document = extractDocumentData(documents);
			callback(document);
		}
		catch (int nodocument)
		{
			(void) nodocument;
			break;
		}
	}
}

TFID_t produceTFID(const DocumentData_t& document)
{
	TFID_t TFID;

	for (const std::string& word : document.TEXT)
		TFID[word]++;

	return TFID;
}
