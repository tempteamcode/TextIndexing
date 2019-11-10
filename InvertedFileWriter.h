#pragma once

#include <map>

/**
* Word entry keeping the frequency of a word in a specific document
**/
struct InvertedFileWordEntry_t {
	unsigned int docID;
	unsigned int frequency;

	InvertedFileWordEntry_t(unsigned int docID, unsigned int frequency)
		: docID(docID), frequency(frequency)
	{}

};

typedef std::map<std::string, std::vector<InvertedFileWordEntry_t>> InvertedFile_t;


inline void invertedFileAdd (InvertedFile_t& IF, int docID, const TFID_t& TFID)
{
	for (const auto& wordfreq : TFID)
	{
		IF[wordfreq.first].emplace_back(docID, wordfreq.second);
	}
}
