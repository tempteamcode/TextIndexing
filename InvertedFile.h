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

typedef std::pair<unsigned int, unsigned int> InvertedFileEntry_t;

typedef std::map<std::string, std::vector<InvertedFileEntry_t>> InvertedFile_t;


inline void invertedFileAdd (InvertedFile_t& IF, int docID, const TFID_t& TFID)
{
	for (const auto& wordfreq : TFID)
	{
		IF[wordfreq.first].emplace_back(docID, wordfreq.second);
	}
}

inline void IFExport(const InvertedFile_t& IF, const std::string& path)
{
	std::ofstream os(path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!os) throw custom_exception::fs_access;

	unsigned int count = IF.size();
	binWrite<unsigned int>(os, count);

	std::map<std::string, unsigned int> addresses;

	for (auto& it : IF)
	{
		addresses[it.first] = 0;
	}
	
	std::streampos fptr_begin = os.tellp();
	binWrite_map<std::string, unsigned int>(os, addresses);

	for (auto& it : IF)
	{
		std::streampos fptr_addr = os.tellp();
		addresses[it.first] = static_cast<unsigned int>(fptr_addr - fptr_begin);

		binWrite_vec<unsigned int, unsigned int>(os, it.second);
	}

	os.seekp(fptr_begin);
	binWrite_map<std::string, unsigned int>(os, addresses);
}

inline void IFImport(InvertedFile_t& IF, const std::string& path)
{
	std::ifstream is(path, std::ios::in | std::ios::binary);
	if (!is) throw custom_exception::fs_access;
	
	unsigned int count;
	binRead<unsigned int>(is, count);

	std::streampos fptr_begin = is.tellg();

	std::map<std::string, unsigned int> addresses;
	binRead_map<std::string, unsigned int>(is, addresses);

	IF.clear();
	for (auto& it : addresses)
	{
		std::streampos fptr_addr = static_cast<std::streampos>(it.second);
		is.seekg(fptr_addr + fptr_begin);

		auto& vec = IF[it.first];
		binRead_vec<unsigned int, unsigned int>(is, vec);
	}
}
