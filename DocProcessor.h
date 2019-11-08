#pragma once

#include "DocExtractor.h"

typedef std::map<std::string, unsigned int> TFID_t;

TFID_t produceTFID(const DocumentData& document)
{
	TFID_t TFID;

	for (const std::string& word : document.TEXT)
		TFID[word]++;

	return TFID;
}
