#include "DocumentParser.h"

#include <iostream>

#include "Tokenizer.h"


void print_tags(const DocumentTree_t& tag, std::string indent)
{
	indent += "    ";
	std::cout << indent << '<' << tag.name << '>' << std::endl;

	for (auto& tag : tag.tags)
	{
		print_tags(tag, indent);
	}

	if (!tag.data.empty())
	{
		std::cout << indent << (tag.data.size() > 30 ? tag.data.substr(0, 27) + "..." : tag.data) << std::endl;
	}

	std::cout << indent << '<' << '/' << tag.name << '>' << std::endl;
}

std::vector<string_view> extractTokens(const std::string& data)
{
	std::vector<string_view> result;

	Tokenizer tokenizer;
	string_view range(data);

	string_view part;

	for (;;)
	{
		try {
			part = tokenizer.extract(data, range);
			result.push_back(part);
			range.begin = part.end;
		}
		catch (custom_exception& e)
		{
			if (e != custom_exception::empty) throw;

			break;
		}
	}

	return result;
}

void extractTokens(const std::string& data, TFID_t& dest)
{
	auto words = extractTokens(data);
	dest.reserve(dest.size() + words.size());
	for (auto& word : words)
	{
		dest[std::string(word)]++;
	}
}

DocumentData_t extractDocumentData(DocumentTree_t& documents)
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

	if (root == nullptr) throw custom_exception::empty;

	DocumentData_t document;

	for (DocumentTree_t& tag : root->tags)
	{
		if (tag.name == "DOCNO")
		{
			if (!document.DOCNO.empty()) std::cerr << "/!\\ several DOCNO tags in one document" << std::endl;

			auto words = extractTokens(tag.data);
			if (words.size() == 2)
			{
				document.DOCNO = std::string(words[0]) + "-" + std::string(words[1]);
				tag.data.clear();
			}
		}
		else if (tag.name == "DOCID")
		{
			auto words = extractTokens(tag.data);
			if (words.size() == 1)
			{
				unsigned int value;
				if (tryParseInt(words[0], value))
				{
					if (document.DOCID != 0) std::cerr << "/!\\ several DOCIDs for one document" << std::endl;
					
					document.DOCID = value;

					tag.data.clear();
				}
			}
		}
		else if (tag.name == "DATE")
		{
			for (auto& subtag : tag.tags)
			{
				if (subtag.name != "P") continue;

				auto words_view = extractTokens(subtag.data);

				if (words_view.size() == 2 && words_view[0] == "Correction" && words_view[1] == "Appended")
				{
					subtag.data.clear();
					continue;
				}
				
				std::vector<std::string> words;
				for (auto& word_view : words_view) words.push_back(std::string(word_view));

				Date_t date;
				if (tryParseDate(words, date))
				{
					if (!document.DATE.empty()) std::cerr << "/!\\ several DATEs for one document" << std::endl;

					document.DATE = date;
					subtag.data.clear();
				}
			}
		}
		else if (tag.name == "LENGTH")
		{
			if (tag.tags.size() >= 1 && tag.tags[0].name == "P")
			{
				auto& subtag = tag.tags[0];
				
				auto words = extractTokens(subtag.data);

				if (subtag.name == "P" && words.size() == 2)
				{
					unsigned int value;
					if (words[1] == "words" && tryParseInt(words[0], value))
					{
						if (document.LENGTH != 0) std::cerr << "/!\\ several lengthes in one document" << std::endl;
						
						document.LENGTH = value;
						subtag.data.clear();
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
			extractTokens(tag.data, document.TEXT);
			tag.data.clear();

			for (auto& subtag : tag.tags)
			{
				if (subtag.name == "P")
				{
					extractTokens(subtag.data, document.TEXT);
					subtag.data.clear();
				}
				else if (subtag.name == "TABLE")
				{
					subtag.clear(); // TABLEs are ignored
				}
			}
		}
		else if (tag.name == "P")
		{
			extractTokens(tag.data, document.TEXT);
			tag.data.clear();
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

void extractDocuments(DocumentTree_t& documents, void(*callback)(DocumentData_t&))
{
	for (;;)
	{
		try {
			DocumentData_t document = extractDocumentData(documents);
			callback(document);
		}
		catch (custom_exception& e)
		{
			if (e != custom_exception::empty) throw;

			break;
		}
	}
}
