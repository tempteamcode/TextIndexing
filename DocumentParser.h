#pragma once

#include <iostream>

#include "DocumentExtractor.h"

#include "utility.h"

struct DocumentData_t
{
	std::string DOCNO;
	unsigned int DOCID = 0;
	Date_t DATE;
	// SECTION
	unsigned int LENGTH = 0;
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

typedef std::map<std::string, unsigned int> TFID_t;

inline void print_tags(const DocumentTree_t& tag, std::string indent) {
	indent += "    ";
	std::cout << indent << '<' << tag.name << '>' << std::endl;

	for (auto& tag : tag.tags)
	{
		print_tags(tag, indent);
	}

	if (!tag.words.empty())
	{
		std::cout << indent;
		unsigned int count = 0;
		for (const std::string& word : tag.words)
		{
			std::cout << word << ' ';
			if (++count > 9)
			{
				std::cout << "[...]" << ' ';
				break;
			}
		}
		std::cout << std::endl;
	}

	std::cout << indent << '<' << '/' << tag.name << '>' << std::endl;
}

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
			if (!document.DOCNO.empty()) std::cerr << "/!\\ several DOCNO tags in one document" << std::endl;

			if (tag.words.size() == 2)
			{
				document.DOCNO = tag.words[0] + "-" + tag.words[1];
				tag.words.clear();
			}
		}
		else if (tag.name == "DOCID")
		{
			if (document.DOCID != 0) std::cerr << "/!\\ several DOCID tags in one document" << std::endl;

			if (tag.words.size() == 1)
			{
				if (tryParseInt(tag.words[0], document.DOCID))
				{
					tag.words.clear();
				}
			}
		}
		else if (tag.name == "DATE")
		{
			Date_t date;

			if (tag.tags.size() >= 1)
			{
				auto& subtag = tag.tags[0];

				if (subtag.name == "P" && tryParseDate(subtag.words, date))
				{
					if (!document.DATE.empty()) std::cerr << "/!\\ several dates in one document" << std::endl;

					document.DATE = date;
					subtag.words.clear();

					if (subtag.empty()) tag.tags.erase(tag.tags.begin());
				}
			}

			for (auto& subtag : tag.tags)
			{
				if (subtag.name == "P" && subtag.words.size() == 2 && subtag.words[0] == "Correction" && subtag.words[1] == "Appended")
				{
					subtag.words.clear();
				}
			}
		}
		else if (tag.name == "LENGTH")
		{
			if (tag.tags.size() >= 1 && tag.tags[0].name == "P")
			{
				auto& subtag = tag.tags[0];

				if (subtag.name == "P" && subtag.words.size() == 2)
				{
					unsigned int value;
					if (subtag.words[1] == "words" && tryParseInt(subtag.words[0], value))
					{
						if (document.LENGTH != 0) std::cerr << "/!\\ several lengthes in one document" << std::endl;
						
						document.LENGTH = value;
						subtag.words.clear();
					}
				}
			}
		}
		else if (tag.name == "HEADLINE")
		{
			continue; // TODO: implement
		}
		else if (tag.name == "TEXT")
		{
			std::move(tag.words.begin(), tag.words.end(), std::back_inserter(document.TEXT));
			tag.words.erase(tag.words.begin(), tag.words.end());

			for (auto& subtag : tag.tags)
			{
				if (subtag.name == "P")
				{
					std::move(subtag.words.begin(), subtag.words.end(), std::back_inserter(document.TEXT));
					subtag.words.erase(subtag.words.begin(), subtag.words.end());
				}
				else if (subtag.name == "TABLE")
				{
					subtag.clear(); // TABLEs are ignored
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
			continue; //TODO: implement
		}
		else if (tag.name == "SUBJECT")
		{
			continue; //TODO: implement
		}
		else if (tag.name == "SECTION" || tag.name == "BYLINE" || tag.name == "DATELINE" || tag.name == "TYPE" || tag.name == "CORRECTION-DATE" || tag.name == "CORRECTION")
		{
			tag.clear(); // these tags are (currently) ignored
		}
		else
		{
			std::cerr << "/!\\ unknown root tag '" << tag.name << "'" << std::endl;
		}

		tag.simplify();
		if (!tag.empty())
		{
			std::cerr << "/!\\ unused '" << tag.name << "' data:" << std::endl;

			print_tags(tag, "");
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
