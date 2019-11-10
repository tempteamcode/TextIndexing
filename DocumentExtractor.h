#pragma once

#include <list>
#include <string>

struct DocumentTree_t
{
	DocumentTree_t* parent;
	std::string name;
	std::list<DocumentTree_t> tags;
	std::string data;

	void simplify();
	bool DocumentTree_t::empty() const;
	void clear();

    DocumentTree_t() = default;
	DocumentTree_t(const DocumentTree_t&) = default;
    DocumentTree_t & operator=(const DocumentTree_t&) = default;
};

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
