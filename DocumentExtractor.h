#pragma once

#include <map>
#include <vector>
#include <string>

struct DocumentTree_t
{
	DocumentTree_t* parent;
	std::string name;
	std::vector<DocumentTree_t> tags;
	std::vector<std::string> words;

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
	void parseToken(const std::string & token);

	DocumentTree_t& getDocument();
	void clearDocument();

private:
	DocumentTree_t tree;
	DocumentTree_t* node = &tree;
};

void DocumentExtractor::parseToken(const std::string& token)
{
	if (token[0] == '<')
	{
		int closing = (token[1] == '/') ? 1 : 0;
		std::string tagname = token.substr(1 + closing, token.length() - 2 - closing);
		auto pos = tagname.find(' ');
		if (pos != std::string::npos) tagname.resize(pos);

		if (token.length() > 1 && token[1] == '/')
		{
			if (tagname != node->name)
			{
				throw "closing tag not matching opening tag";
			}
			if (node->parent == nullptr)
			{
				throw "closing document root";
			}

			node = node->parent;
		}
		else
		{
			node->tags.push_back(DocumentTree_t());
			DocumentTree_t* child = &*(node->tags.rbegin());
			child->parent = node;
			child->name = tagname;
			node = child;
		}
	}
	else
	{
		node->words.push_back(token);
	}
}

DocumentTree_t& DocumentExtractor::getDocument()
{
	if (node != &tree)
	{
		throw "closing tag missing";
	}

	return tree;
}

void DocumentTree_t::simplify()
{
	for (auto& tag : tags) tag.simplify();

	tags.erase(std::remove_if(tags.begin(), tags.end(), [] (const DocumentTree_t& tag) {
		return tag.empty();
	}), tags.end());
}

bool DocumentTree_t::empty() const
{
	return (tags.empty() && words.empty());
}

void DocumentTree_t::clear()
{
	tags.clear();
	words.clear();
}

void DocumentExtractor::clearDocument()
{
	tree.clear();
	node = &tree;
}
