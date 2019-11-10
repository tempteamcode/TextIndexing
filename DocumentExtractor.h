#pragma once

#include <map>
#include <vector>
#include <string>

struct DocumentTree_t
{
	DocumentTree_t* parent;
	std::string name;
	std::vector<DocumentTree_t> tags;
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
	void parseTagOrData(const std::string& chunk);

	DocumentTree_t& getDocument();
	void clearDocument();

private:
	DocumentTree_t tree;
	DocumentTree_t* node = &tree;

	std::vector<std::string> part;
};

inline void DocumentExtractor::parseTagOrData(const std::string& tagordata)
{
	if (tagordata[0] == '<')
	{
		const std::string& tag = tagordata;

		node->data += stringJoin(part);
		part.clear();

		int closing = (tag[1] == '/') ? 1 : 0;
		std::string tagname = tag.substr(1 + closing, tag.length() - 2 - closing);
		auto pos = tagname.find(' ');
		if (pos != std::string::npos) tagname.resize(pos);

		if (tag.length() > 1 && tag[1] == '/')
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
		const std::string& data = tagordata;

		part.push_back(data);
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

	for (char c : data)
	{
		if (c != ' ' && c != '\n')
		{
			return;
		}
	}
	data.clear();
}

bool DocumentTree_t::empty() const
{
	return (tags.empty() && data.empty());
}

void DocumentTree_t::clear()
{
	tags.clear();
	data.clear();
}

void DocumentExtractor::clearDocument()
{
	tree.clear();
	node = &tree;
}
