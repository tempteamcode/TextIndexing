#include <fstream>
#include <iostream>

#include "Files.h"
#include "Tokenizer.h"
#include "DocumentExtractor.h"
#include "DocumentParser.h"
#include "InvertedFile.h"
#include "search.h"


template <class callback_t>
void file_tagsdata_process(const std::string& path, callback_t callback)
{
	PreprocessorTagsData preproc;

	std::ifstream is(path);
	if (!is) throw custom_exception::fs_access;

	while (is) {
		std::string tagordata = preproc.separateTagsData(is);
		callback(tagordata);
	}
}

void file_tagsdata_print(const std::string& path)
{
	file_tagsdata_process(path, [] (std::string& tagordata) {
		std::cout << tagordata << std::endl;
	});
}

template<class callback_t>
void file_documents_process(const std::string& path, callback_t callback)
{
	DocumentExtractor docextractor;

	file_tagsdata_process(path, [&] (std::string& tagordata) {
		docextractor.parseTagOrData(tagordata);
	});

	extractDocuments(docextractor.getDocument(), callback);
}

void file_documents_print(const std::string& path)
{
	file_documents_process(path, [] (DocumentData_t& document) {
		std::cout << document.DOCNO << std::endl;
	});
}


bool makeInvertedFile(InvertedFile_t& IF)
{
	std::ifstream inputs("inputs.txt");
	if (!inputs)
	{
		std::cerr << "FATAL ERROR: Unable to generate the InvertedFile." << std::endl;
		std::cerr << "Details: could not find 'inputs.txt' containing the list of input files." << std::endl;
		return false;
	}

	std::cout << std::endl;
	std::cout << "Parsing the input files..." << std::endl;

	for (std::string path; std::getline(inputs, path);)
	{
		if (path.empty()) continue;

		std::cout << path << std::endl;

		try {
			file_documents_process(path, [&] (DocumentData_t& document) {
				TFID_t document_TFID = TFC_to_TFID(document.TEXT);
				invertedFileAdd(IF, document.DOCID, document_TFID);
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
	try
	{
		InvertedFile_t IF;

		/*
		IFImportPart(IF, "InvertedFile.bin", std::vector<std::string>({ "hello", "Steve" }));
		auto resultsConjunction = resultsOrder(searchNaive(IF, aggregate_maps_AND_min), 10);
		auto resultsDisjunction = resultsOrder(searchNaive(IF, aggregate_maps_OR_max), 10);

		return 0;
		*/

		std::cout << "Importing 'InvertedFile.bin'..." << std::endl;
		if (IFImport(IF, "InvertedFile.bin"))
		{
			std::cout << "Imported 'InvertedFile.bin' successfully." << std::endl;
		}
		else
		{
			std::cout << "Unable to import 'InvertedFile.bin'." << std::endl;
			std::cout << std::endl;

			std::cout << "Generating the InvertedFile..." << std::endl;
			if (!makeInvertedFile(IF)) return 1;
			std::cout << "Generated the InvertedFile successfully." << std::endl;
			std::cout << std::endl;

			std::cout << "Exporting 'InvertedFile.bin'..." << std::endl;
			if (!IFExport(IF, "InvertedFile.bin"))
			{
				std::cout << "FATAL ERROR: Unable to export the InvertedFile in 'InvertedFile.bin'." << std::endl;
				return 1;
			}
			std::cout << "Exported 'InvertedFile.bin' successfully." << std::endl;
		}
		std::cout << std::endl;

		std::cout << "InvertedFile contains " << IF.size() << " entries." << std::endl;

		return 0;
	}

	catch (...)
	{
		std::cout << std::endl;

		std::cerr << "FATAL ERROR: uncaught exception." << std::endl;

		try
		{
			/*re*/throw;
		}
		catch (const custom_exception& e)
		{
			switch (e)
			{
			case custom_exception::fs_access:
				std::cerr << "Details: error on file i/o operations." << std::endl;
				break;
			case custom_exception::parsing_error:
				std::cerr << "Details: incorrect file structure." << std::endl;
				break;
			default:
				break;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Details: " << e.what() << std::endl;
		}
		catch (...)
		{
		}

		std::cout << "Aborting." << std::endl;
		return 1;
	}
}
