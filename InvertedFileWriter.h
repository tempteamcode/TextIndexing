#pragma once

/**
* Word entry keeping the frequency of a word in a specific document
**/
struct InvertedFileWordEntry {

	int docID;
	unsigned int frequency;

	InvertedFileWordEntry(int docID = -1, unsigned int frequency = 0)
		: docID(docID), frequency(frequency)
	{}

};


typedef std::map< std::string, std::vector< InvertedFileWordEntry > > InvertedFile_t;

void invertedFileAdd( InvertedFile_t& IF, int docID, const TFID_t& TFID )
{
	for (const auto& wordfreq : TFID)
	{
		IF[wordfreq.first].emplace_back( docID, wordfreq.second );
	}
}
