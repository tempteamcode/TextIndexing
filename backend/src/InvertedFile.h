/**
 * @file InvertedFile.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief 
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

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
	return documents_in <= 0 ? freqTF(freq) : freqTF(freq) * log(static_cast<float>(documents_total) / static_cast<float>(documents_in));
}

typedef std::unordered_map<std::string, Frequency_t> TFID_t;

TFID_t TFC_to_TFID(const TFC_t& src);

struct DocFreq_t {
	DocumentUID_t docID;
	Frequency_t frequency;
};

typedef std::pair<DocumentUID_t, Frequency_t> InvertedFileEntry_t;
typedef std::vector<InvertedFileEntry_t> InvertedFileRow_t;

/**
 * @brief Inverted File typedef
 * Inverted files are a very common pattern in text indexation.
 * Inverted files maps terms to a vector of documents ID with respective term occurence frequency.
 */
typedef std::map<std::string, InvertedFileRow_t> InvertedFile_t;

/**
 * @brief Append a given entry (Document ID associated with a TF ID) to index in an Inverted File 
 * 
 * @param IF Inverted file to index in
 * @param docID Document ID which identifies the document to be indexed
 * @param TFID Term Frequencies specific to the document to index
 */
void invertedFileAdd(InvertedFile_t& IF, DocumentUID_t docID, const TFID_t& TFID);

/**
 * @brief Exports Inverted file index to specified path
 * 
 * @param IF Inverted File index to serialize/export
 * @param path Path where inverted file will be exported
 * @return Whether if inverted file have bee sucessfully exported
 */
bool IFExport(const InvertedFile_t& IF, const std::string& path);

/**
 * @brief Imports Inverted file index from a specified path
 * 
 * @param[out] IF Reference to Inverted File index to import/deserialize
 * @param[in] path Path where inverted file is imported from
 * @return Whether if inverted file have bee sucessfully imported
 */
bool IFImport(InvertedFile_t& IF, const std::string& path);

bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words);
