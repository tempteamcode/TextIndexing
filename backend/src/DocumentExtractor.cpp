#include "DocumentExtractor.h"

#include <algorithm>

#include "utility.h"


void DocumentExtractor::parseTagOrData(const std::string& tagordata)
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
				throw custom_exception::parsing_error; // "closing tag not matching opening tag"
			}
			if (node->parent == nullptr)
			{
				throw custom_exception::parsing_error; // "closing document root"
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
		throw custom_exception::parsing_error; // "closing tag missing"
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
