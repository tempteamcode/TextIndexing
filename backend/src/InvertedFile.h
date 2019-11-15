#pragma once

#include <map>
#include <vector>

#include "DocumentParser.h"

#include "files.h"

typedef unsigned int DocumentUID_t;
typedef unsigned int Frequency_t;

struct DocFreq_t {
	DocumentUID_t docID;
	Frequency_t frequency;
};

typedef std::pair<DocumentUID_t, Frequency_t> InvertedFileEntry_t;
typedef std::vector<InvertedFileEntry_t> InvertedFileRow_t;

typedef std::map<std::string, InvertedFileRow_t> InvertedFile_t;


void invertedFileAdd(InvertedFile_t& IF, int docID, const TFID_t& TFID);

bool IFExport(const InvertedFile_t& IF, const std::string& path);

bool IFImport(InvertedFile_t& IF, const std::string& path);

bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words);
