#pragma once

typedef std::map<std::string, std::vector<std::pair<int, int>>> InvertedFile_t;

void invertedFileAdd(InvertedFile_t& IF, int docID, const TFID_t& TFID)
{
	for (const auto& wordfreq : TFID)
	{
		IF[wordfreq.first].push_back({ docID, wordfreq.second });
	}
}
