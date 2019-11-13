#include <fstream>
#include <iostream>

#include "Files.h"
#include "Tokenizer.h"
#include "DocumentExtractor.h"
#include "DocumentParser.h"
#include "InvertedFile.h"


void print_file_chunks(const std::string& path)
{
	PreprocessorTagsData preproc;

	std::ifstream is(path);
	if (!is) throw custom_exception::fs_access;

	while (is) {
		std::string tagordata = preproc.separateTagsData(is);
		std::cout << tagordata << std::endl;
	}
}

void print_file_documents(const std::string& path)
{
	PreprocessorTagsData preproc;
	DocumentExtractor docextractor;

	std::ifstream is(path);
	if (!is) throw custom_exception::fs_access;

	while (is) {
		std::string tagordata = preproc.separateTagsData(is);
		docextractor.parseTagOrData(tagordata);
	}

	extractDocuments(docextractor.getDocument(), [] (DocumentData_t& document) {
		std::cout << document.DOCNO << std::endl;
	});
}

template<class callback_t>
void process_file(const std::string& path, callback_t callback)
{
	PreprocessorTagsData preproc;
	DocumentExtractor docextractor;

	std::ifstream is(path);
	if (!is) throw custom_exception::fs_access;

	while (is) {
		std::string tagordata = preproc.separateTagsData(is);
		docextractor.parseTagOrData(tagordata);
	}

	extractDocuments(docextractor.getDocument(), callback);
}


bool makeInvertedFile(InvertedFile_t& IF)
{
	std::ifstream inputs("inputs.txt");
	if (!inputs)
	{
		std::cerr << "FATAL ERROR: Unable to generate the InvertedFile." << std::endl;
		std::cerr << "Reason: could not find 'inputs.txt' containing the list of input files." << std::endl;
		return false;
	}

	std::cout << "Parsing the input files..." << std::endl;

	for (std::string path; std::getline(inputs, path);)
	{
		if (path.empty()) continue;

		std::cout << path << std::endl;

		try {
			process_file(path, [&] (DocumentData_t& document) {
				invertedFileAdd(IF, document.DOCID, document.TEXT);
			});
		}
		catch (custom_exception& e)
		{
			switch (e)
			{
			case custom_exception::fs_access:
				std::cerr << "ERROR: file not found / access denied" << std::endl; break;
			case custom_exception::parsing_error:
				std::cerr << "ERROR: invalid tag structure" << std::endl; break;
			}
		}
	}

	std::cout << std::endl;

	return true;
}

int main(int argc, char * argv[])
{
	InvertedFile_t IF;

	try
	{
		if (IFImport(IF, "InvertedFile.bin"))
		{
			std::cout << "Loaded InvertedFile.bin successfully." << std::endl;
		}
		else
		{
			if (!makeInvertedFile(IF)) return 1;

			if (!IFExport(IF, "InvertedFile.bin"))
			{
				std::cout << "FATAL ERROR: Unable to save InvertedFile in 'InvertedFile.bin'." << std::endl;
				return 1;
			}

			std::cout << "Generated InvertedFile.bin successfully." << std::endl;
		}
		std::cout << std::endl;

		std::cout << "InvertedFile contains " << IF.size() << " entries." << std::endl;

		return 0;
	}
	catch (custom_exception&)
	{

	}
	catch (...)
	{

	}
}
