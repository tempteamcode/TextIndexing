#pragma once

#include <map>
#include <vector>

#include "DocumentParser.h"

#include "files.h"

typedef unsigned int DocumentUID_t;
typedef float Frequency_t;

typedef std::unordered_map<std::string, Frequency_t> TFID_t;

TFID_t TFC_to_TFID(const TFC_t& src);

struct DocFreq_t {
	DocumentUID_t docID;
	Frequency_t frequency;
};

typedef std::pair<DocumentUID_t, Frequency_t> InvertedFileEntry_t;
typedef std::vector<InvertedFileEntry_t> InvertedFileRow_t;

typedef std::map<std::string, InvertedFileRow_t> InvertedFile_t;

void invertedFileAdd(InvertedFile_t& IF, DocumentUID_t docID, const TFID_t& TFID);

bool IFExport(const InvertedFile_t& IF, const std::string& path);

bool IFImport(InvertedFile_t& IF, const std::string& path);

bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words);
