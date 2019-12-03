#pragma once

#include <map>
#include <vector>
//#include <iostream>

#include "DocumentParser.h"

#include "files.h"

typedef unsigned int DocumentUID_t;
typedef unsigned_ints Frequency_t;

struct DocFreq_t {
	DocumentUID_t docID;
	Frequency_t frequency;
};

typedef std::unordered_map<std::string, Frequency_t> TFID_t;
typedef std::pair<DocumentUID_t, Frequency_t> InvertedFileEntry_t;
typedef std::vector<InvertedFileEntry_t> InvertedFileRow_t;




/// Renvoit la fréquence "TF" d'un terme dans un document.
inline float freqTF(const Frequency_t& freq)
{
	return freq.second <= 0 ? 0.0f : static_cast<float>(freq.first) / static_cast<float>(freq.second);
}
/// Renvoit la fréquence "TFIDF" d'un terme dans un document, si le nombre de documents total et contenant ce terme sont connus.
inline float freqTFIDF(const Frequency_t& freq, unsigned int documents_in, unsigned int documents_total)
{
	return documents_in <= 0 ? freqTF(freq) : freqTF(freq) * log(static_cast<float>(documents_total) / static_cast<float>(documents_in));
}



TFID_t TFC_to_TFID(const TFC_t& src);


typedef std::map<std::string, InvertedFileRow_t> InvertedFile_t;

/// Ajoute les termes d'un document à l'InvertedFile.
void invertedFileAdd(InvertedFile_t& IF, DocumentUID_t docID, const TFID_t& TFID);

/// Exporte l'InvertedFile dans un fichier.
bool IFExport(const InvertedFile_t& IF, const std::string& path);

/// Importe l'InvertedFile depuis un fichier.
bool IFImport(InvertedFile_t& IF, const std::string& path);

/// Importe une partie d'un InvertedFile depuis un fichier (seulement les entrées de certains mots).
bool IFImportPart(InvertedFile_t& IF, const std::string& path, const std::vector<std::string>& words);
