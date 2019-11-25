#pragma once

#include <map>
#include <vector>
//#include <iostream>

#include "DocumentParser.h"

#include "files.h"

typedef unsigned int DocumentUID_t;

typedef unsigned_ints Frequency_t;

inline float freqTF(const Frequency_t& freq)
{
	return freq.second <= 0 ? 0.0f : static_cast<float>(freq.first) / static_cast<float>(freq.second);
}
inline float freqTFIDF(const Frequency_t& freq, unsigned int documents_in, unsigned int documents_total)
{
	return documents_in <= 0 ? freqTF(freq) : freqTF(freq) * log(documents_total / documents_in);
}

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
