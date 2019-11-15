#pragma once

#include <map>
#include <vector>

#include "DocumentParser.h"

#include "files.h"

typedef unsigned int DocumentUID_t;

/**
* Word entry keeping the frequency of a word in a specific document
**/
struct InvertedFileWordEntry_t {
	DocumentUID_t docID;
	unsigned int frequency;

	InvertedFileWordEntry_t(unsigned int docID, unsigned int frequency)
		: docID(docID), frequency(frequency)
	{}

};

typedef unsigned int Frequency_t;
typedef std::pair<DocumentUID_t, Frequency_t> InvertedFileEntry_t;
typedef std::vector<InvertedFileEntry_t> InvertedFileRow_t;

typedef std::map<std::string, InvertedFileRow_t> InvertedFile_t;


void invertedFileAdd(InvertedFile_t& IF, int docID, const TFID_t& TFID);

bool IFExport(const InvertedFile_t& IF, const std::string& path);

bool IFImport(InvertedFile_t& IF, const std::string& path);

bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words);
