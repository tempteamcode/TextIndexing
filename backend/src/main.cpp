#include <fstream>
#include <iostream>

#include "files.h"
#include "Tokenizer.h"
#include "DocumentExtractor.h"
#include "DocumentParser.h"
#include "InvertedFile.h"
#include "search.h"
#include "utility.h"


typedef unsigned_ints FileDocumentUID_t;

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

	consumeDocuments(docextractor.getDocument(), callback);
}

void file_documents_print(const std::string& path)
{
	file_documents_process(path, [] (DocumentTree_t& document_tree) {
		DocumentData_t document;
		extractDocumentData(document_tree, document);
		std::cout << document.DOCNO << std::endl;
	});
}


bool loadInputFiles(std::vector<std::string>& input_files)
{
	std::ifstream inputs("inputs.txt");
	if (!inputs) return false;

	input_files.clear();

	std::string path;
	for (;std::getline(inputs, path);)
	{
		if (path.empty()) continue;

		input_files.push_back(path);
	}

	return true;
}

bool loadDocumentJSON(const std::vector<std::string>& input_files, FileDocumentUID_t ID, DocumentJSON_t& documentJSON)
{
	const std::string& input_file = input_files[ID.first];

	unsigned int documentID = 0;

	bool found = false;
	file_documents_process(input_file, [&] (DocumentTree_t& document_tree) {
		if (++documentID == ID.second)
		{
			extractDocumentJSON(document_tree, documentJSON);
			found = true;
		}
	});

	return found;
}

void makeInvertedFile(const std::vector<std::string>& input_files, InvertedFile_t& IF)
{
	std::cout << "Parsing the input files..." << std::endl;

	FileDocumentUID_t filedocumentID;

	filedocumentID.first = 0;
	for (const std::string& path : input_files)
	{
		if (path.empty()) continue;

		std::cout << path << std::endl;
		filedocumentID.first++;

		filedocumentID.second = 0;
		try {
			file_documents_process(path, [&] (DocumentTree_t& document_tree) {
				filedocumentID.second++;

				DocumentData_t document;
				extractDocumentData(document_tree, document);

				TFID_t document_TFID = TFC_to_TFID(document.TEXT);
				invertedFileAdd(IF, DocumentUID_t(filedocumentID), document_TFID);
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
}

struct {
	bool index;
	std::vector<std::string> query;
	unsigned int documentID;
} arguments;

bool parseArgs(int argc, char * argv[])
{
	arguments.index = false;
	arguments.query.clear();
	arguments.documentID = 0;

	if (argc == 0) return false;

	if (streq(argv[0], "index"))
	{
		arguments.index = true;
		return (argc <= 1);
	}
	else if (streq(argv[0], "query"))
	{
		arguments.query.reserve(argc);
		for (int argi = 1; argi < argc; argi++)
		{
			arguments.query.push_back(argv[argi]);
		}
		return (argc > 1);
	}
	else if (streq(argv[0], "document_json"))
	{
		if (argc <= 1) return false;
		if (!tryParseInt(argv[1], arguments.documentID)) return false;
		return (argc == 2);
	}
	else
		return false;
}

int main(int argc, char * argv[])
{
	std::vector<std::string> input_files;
	if (!loadInputFiles(input_files))
	{
		std::cerr << "FATAL ERROR: Unable to load 'inputs.txt' containing the list of input files." << std::endl;
		return 1;
	}

	if (argc <= 1)
	{
		std::cout << "WARNING: Missing command line arguments." << std::endl;
		std::cout << std::endl;
		std::cout << "Available arguments are:" << '\n'
			<< "- 'index' to (re)build the InvertedFile" << '\n'
			<< "- 'query <word> <word> ...' to search for articles" << '\n'
			<< "- 'document_json <ID>' to get the full contents of an article" << std::endl;
		std::cout << std::endl;
	}
	else
	{
		if (!parseArgs(argc - 1, argv + 1))
		{
			std::cerr << "ERROR: Unable to parse command line arguments." << std::endl;
			std::cout << std::endl;
		}
	}

	try
	{
		InvertedFile_t IF;

		if (arguments.index || (arguments.query.empty() && (arguments.documentID == 0)))
		{
			if (!arguments.index)
			{
				std::cout << "Importing 'InvertedFile.bin'..." << std::endl;
				if (IFImport(IF, "InvertedFile.bin"))
				{
					std::cout << "Imported 'InvertedFile.bin' successfully." << std::endl;
				}
				else
				{
					std::cout << "Unable to import 'InvertedFile.bin'." << std::endl;
				}
				std::cout << std::endl;
			}

			if (IF.empty())
			{
				std::cout << "Generating the InvertedFile..." << std::endl;
				makeInvertedFile(input_files, IF);
				std::cout << "Generated the InvertedFile successfully." << std::endl;
				std::cout << std::endl;

				std::cout << "Exporting 'InvertedFile.bin'..." << std::endl;
				if (!IFExport(IF, "InvertedFile.bin"))
				{
					std::cout << "FATAL ERROR: Unable to export the InvertedFile in 'InvertedFile.bin'." << std::endl;
					return 1;
				}
				std::cout << "Exported 'InvertedFile.bin' successfully." << std::endl;
				std::cout << std::endl;
			}
		}

		if (!arguments.query.empty())
		{
			auto& words = arguments.query;

			for (std::string& word : arguments.query)
			{
				if (!stemming_stopword(word)) word.clear();
			}
			words.erase(std::remove_if(words.begin(), words.end(), [] (const std::string& word) { return word.empty(); }), words.end());

			if (!IFImportPart(IF, "InvertedFile.bin", arguments.query))
			{
				std::cout << "ERROR: Unable to import 'InvertedFile.bin'." << std::endl;
				std::cout << "Run this program with the argument 'index' to generate it." << std::endl;
				return 1;
			}

			auto resultsConjunction = resultsOrder(searchNaive(IF, aggregate_maps_AND_min), 10);
			auto resultsDisjunction = resultsOrder(searchNaive(IF, aggregate_maps_OR_max), 10);
			
			auto resultsShow = [&](const SearchResults_t& results, const char* name) {
				std::cout << "Top 10 (" << name << "):" << std::endl;
				int index = 0;
				for (auto& result : results)
				{
					std::cout << (++index) << ". document " << result.docID << " (frequency: " << result.frequency << ")" << std::endl;
				}
				std::cout << std::endl;
			};

			resultsShow(resultsConjunction, "naive search conjunction");
			resultsShow(resultsDisjunction, "naive search disjunction");
		}
		else if (arguments.documentID > 0)
		{
			DocumentJSON_t documentJSON;
			if (!loadDocumentJSON(input_files, FileDocumentUID_t(arguments.documentID), documentJSON))
			{
				std::cerr << "ERROR: Could not load the document with ID " << arguments.documentID << "." << std::endl;
				return 1;
			}

			std::cout << documentJSON.title << (documentJSON.date.empty() ? "" : std::string(documentJSON.date)) << std::endl;
			std::cout << std::endl;

			std::cout << stringJoin(documentJSON.contents, std::string("\n\n")) << std::endl;

			return 0;
		}
		else if (!arguments.index)
		{
			if (!IFImport(IF, "InvertedFile.bin"))
			{
				std::cout << "ERROR: Unable to import 'InvertedFile.bin'." << std::endl;
				std::cout << "Run this program with the argument 'index' to generate it." << std::endl;
				return 1;
			}

			std::cout << "InvertedFile contains " << IF.size() << " entries." << std::endl;
		}

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
