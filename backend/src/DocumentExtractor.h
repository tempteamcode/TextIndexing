/**
 * @file DocumentExtractor.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief Defines a DocumentExtractor abstraction class and a DocumentTree_t Structure
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 */
#pragma once

#include <list>
#include <string>

/**
 * @struct DocumentTree_t
 * DocumentTree_t structure stores a document tree resulting from DocumentExtractor
 */
struct DocumentTree_t
{
	/** Pointer to parent node/document/tag */
	DocumentTree_t* parent;
	std::string name;
	std::list<DocumentTree_t> tags;
	std::string data;

	void simplify();
	bool empty() const;
	void clear();

    DocumentTree_t() = default;
	DocumentTree_t(const DocumentTree_t&) = default;
    DocumentTree_t & operator=(const DocumentTree_t&) = default;
};

/**
 * @class DocumentExtractor
 * @brief Stores a DocumentTree_t tree which is extracted/parsed from successive calls to parseTagOrData
 */
class DocumentExtractor
{
public:
	void parseTagOrData(const std::string& tagordata);

	/**
	 * Get the extracted DocumentTree_t tree
	 * @return DocumentTree_t tree reference
	 * @todo dangerous getter returning reference to private attribute, little refactoring needed?
	 */
	DocumentTree_t& getDocument();
	void clearDocument();

private:
	DocumentTree_t tree;
	DocumentTree_t* node = &tree;

	std::list<std::string> part;
};
