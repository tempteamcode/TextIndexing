#include <fstream>

#include "InvertedFile.h"


void invertedFileAdd(InvertedFile_t& IF, int docID, const TFID_t& TFID)
{
	for (const auto& wordfreq : TFID)
	{
		IF[wordfreq.first].emplace_back(docID, wordfreq.second);
	}
}

bool IFExport(const InvertedFile_t& IF, const std::string& path)
{
	std::ofstream os(path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!os) return false;

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

		binWrite_vec<DocumentUID_t, Frequency_t>(os, it.second);
	}

	os.seekp(fptr_begin);
	binWrite_map<std::string, unsigned int>(os, addresses);

	return true;
}

bool IFImport(InvertedFile_t& IF, const std::string& path)
{
	std::ifstream is(path, std::ios::in | std::ios::binary);
	if (!is) return false;

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
		binRead_vec<DocumentUID_t, Frequency_t>(is, vec);
	}

	return true;
}

bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words)
{
	std::ifstream is(path, std::ios::in | std::ios::binary);
	if (!is) return false;

	unsigned int count;
	binRead<unsigned int>(is, count);

	std::streampos fptr_begin = is.tellg();

	std::map<std::string, unsigned int> addresses;
	binRead_map<std::string, unsigned int>(is, addresses);

	IF.clear();
	for (const std::string& word : words)
	{
		unsigned int address = addresses[word];
		std::streampos fptr_addr = static_cast<std::streampos>(address);
		is.seekg(fptr_addr + fptr_begin);

		auto& vec = IF[word];
		binRead_vec<DocumentUID_t, Frequency_t>(is, vec);
	}

	return true;
}
