#include <fstream>
#include <iostream>

#include "src/files.h"
#include "src/Tokenizer.h"
#include "src/DocumentExtractor.h"
#include "src/DocumentParser.h"
#include "src/InvertedFile.h"
// #include "src/search.h"

#include "includes/json.hpp"

using json = nlohmann::json;


#define RETURN_ON_FAILURE( ret ) { if( ret ) return ret; }

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
			default:break;
			}
		}
	}

	std::cout << std::endl;

	return true;
}

struct {
	bool index;
	std::vector<std::string> query;
	unsigned int documentID;
} arguments;

bool streq(const char* str1, const char* str2)
{
	for (;;)
	{
		if (*str1 != *str2) return false;
		if (*str1 == '\0') return true;
		str1++; str2++;
	}
}

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




// ======== Modes

/**
* Generate the inverted file if needed.
* @return 0 if it worked, 1 if an error occured
*/
int invertedFileGeneration( void ) {

	InvertedFile_t IF;

	std::cout << "Generating the InvertedFile..." << std::endl;
	if (!makeInvertedFile(IF)) return 1;

	std::cout << "Generated the InvertedFile successfully." << std::endl;
	std::cout << std::endl;

	std::cout << "Exporting 'InvertedFile.bin'..." << std::endl;
	if (!IFExport(IF, "InvertedFile.bin"))
	{
		std::cerr << "FATAL ERROR: Unable to export the InvertedFile in 'InvertedFile.bin'." << std::endl;
		return 1;
	}
	std::cout << "Exported 'InvertedFile.bin' successfully." << std::endl;
	std::cout << std::endl;
	return 0;
}

/**
* Return the result of a query
* @param words list of the words in the query
* @return 0 if it worked, 1 if an error occured
*/
int searchQueryMode( std::vector< std::string > & words ) {

	// InvertedFile_t IF;
	//
	// if (!IFImportPart(IF, "InvertedFile.bin", arguments.query))
	// {
	// 	std::cerr << "ERROR: Unable to import 'InvertedFile.bin'." << std::endl;
	// 	std::cerr << "Run this program with the argument 'index' to generate it." << std::endl;
	// 	return 1;
	// }
	//
	// auto resultsConjunction = resultsOrder(searchNaive(IF, aggregate_maps_AND_min), 10);
	// auto resultsDisjunction = resultsOrder(searchNaive(IF, aggregate_maps_OR_max), 10);
	//
	// auto resultsShow = [&](const SearchResults_t& results, const char* name) {
	// 	std::cout << "Top 10 (" << name << "):" << std::endl;
	// 	int index = 0;
	// 	for (auto& result : results)
	// 	{
	// 		std::cout << (++index) << ". document " << result.docID << " (frequency: " << result.frequency << ")" << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// };
	//
	// resultsShow(resultsConjunction, "naive search conjunction");
	// resultsShow(resultsDisjunction, "naive search disjunction");

	return 0;
}


/**
* Return a specific document
*/
int documentQueryMode( unsigned int documentID ) {

	//TODO document query
	std::cout
		<< "{ code: 503, message: \"Not supported yet\", document: null }"
		<< std::endl;

	return 0;

}

int importInvertedFile( void ) {

	InvertedFile_t IF;

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

	std::cout << "InvertedFile contains " << IF.size() << " entries." << std::endl;

	return 0;
}

// ======== Where the magic happens!



int main(int argc, char * argv[])
{
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

		int ret;

		if (arguments.index || (arguments.query.empty() && (arguments.documentID == 0)))
		{
			RETURN_ON_FAILURE( invertedFileGeneration( ) );
		}

		if (!arguments.query.empty() )
		{
			RETURN_ON_FAILURE( searchQueryMode( arguments.query ) );
		}
		else if (arguments.documentID > 0)
		{
			RETURN_ON_FAILURE( documentQueryMode( arguments.documentID ) );
		}
		else if (!arguments.index)
		{
			RETURN_ON_FAILURE( importInvertedFile( ) );
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
