#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <string>

#include "src/utility.h"
#include "src/files.h"
#include "src/Tokenizer.h"
#include "src/DocumentExtractor.h"
#include "src/DocumentParser.h"
#include "src/InvertedFile.h"
#include "src/search.h"
#include "src/FA.h"
#include "src/TA.h"

typedef unsigned_ints FileDocumentUID_t;

bool outputOnlyJSON = false;

std::string ressourcesRootFolder = "./ressources/";
std::string inputsFilePath = ressourcesRootFolder + "inputs.txt";


// ======== Fonctions annexes

/// Sépare les balises d'un fichier XML de leur contenu, et appelle une fonction callback sur chaque morceau (dans le bon ordre).
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

/// Affiche le contenu d'un fichier XML, en sautant des lignes avant et après chaque balise. (à n'utiliser qu'à des fins de débuggage uniquement)
void file_tagsdata_print(const std::string& path)
{
	file_tagsdata_process(path, [] (std::string& tagordata) {
		if (!outputOnlyJSON)
		std::cout << tagordata << std::endl;
	});
}

/// Extrait les documents (articles de journal) contenus dans un fichier XML, et appelle une fonction callback sur chacun.
template<class callback_t>
void file_documents_process(const std::string& path, callback_t callback)
{
	DocumentExtractor docextractor;

	file_tagsdata_process(path, [&] (std::string& tagordata) {
		docextractor.parseTagOrData(tagordata);
	});

	consumeDocuments(docextractor.getDocument(), callback);
}

/// Affiche les numéros des documents (articles de journal) contenus dans un fichier XML. (à n'utiliser qu'à des fins de débuggage uniquement)
void file_documents_print(const std::string& path)
{
	file_documents_process(path, [] (DocumentTree_t& document_tree) {
		DocumentData_t document;
		extractDocumentData(document_tree, document);
		if (!outputOnlyJSON)
		std::cout << document.DOCNO << std::endl;
	});
}

/// Charge la liste des fichiers d'entrée (contenue dans "inputs.txt"). Renvoit true en cas de succès.
bool loadInputFiles(std::vector<std::string>& input_files)
{
	std::ifstream inputs( inputsFilePath );
	if (!inputs) return false;

	input_files.clear();

	std::string path;
	for (;std::getline(inputs, path);)
	{
		if (path.empty()) continue;

		input_files.push_back( ressourcesRootFolder + path );
	}

	return true;
}

/// Récupère un document à partir de son UID, et le renvoit dans un format destiné à être converti en JSON.
bool loadDocumentJSON(const std::vector<std::string>& input_files, FileDocumentUID_t ID, DocumentJSON_t& documentJSON)
{
	const std::string& input_file = input_files[ID.first - 1];

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

/// Génère l'InvertedFile.
void makeInvertedFile(const std::vector<std::string>& input_files, InvertedFile_t& IF)
{
	if (!outputOnlyJSON)
	std::cout << "Parsing the input files..." << std::endl;

	FileDocumentUID_t filedocumentID;

	filedocumentID.first = 0;
	for (const std::string& path : input_files)
	{
		if (path.empty()) continue;

		if (!outputOnlyJSON)
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
			default: break;
			}
		}
	}

	if (!outputOnlyJSON)
	std::cout << std::endl;
}

/**
* structure pour stocker les arguments en ligne de commande
*
* Il n'est pas nécessaire de mettre (un constructeur avec) des valeurs par défaut (traduisant l'absence d'arguments)
* car c'est lors du parsing des arguments (et non lors de la construction de la structure) que les attributs sont initialisés.
**/
struct arguments_t {
	bool index = false;
	std::vector<std::string> query;
	unsigned int documentID;
};

/// Extrait et parse les arguments passés en ligne de commande.
bool parseArgs(arguments_t& arguments, int argc, char ** argv)
{
	arguments.index = false;
	arguments.query.clear();
	arguments.documentID = 0;

	if (argc <= 0) return false;

	if (streq(argv[0], "json"))
	{
		outputOnlyJSON = true;
		if (argv++, --argc <= 0) return false;
	}

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
	else if (streq(argv[0], "document"))
	{
		if (argc <= 1) return false;
		if (!tryParseInt(argv[1], arguments.documentID)) return false;
		return (argc == 2);
	}
	else
		return false;
}


// ======== Modes d'utilisation

/**
* Génère l'InvertedFile (et affiche la progression).
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeBuildInvertedFile(const std::vector<std::string>& input_files)
{
	std::ostringstream os;

	InvertedFile_t IF;

	os << "Generating the InvertedFile..." << std::endl;
	makeInvertedFile(input_files, IF);
	os << "Generated the InvertedFile successfully." << std::endl;
	os << std::endl;

	os << "Exporting 'InvertedFile.bin'..." << std::endl;
	bool success = IFExport(IF, "InvertedFile.bin");
	if (!success)
	{
		std::cerr << "FATAL ERROR: Unable to export the InvertedFile in 'InvertedFile.bin'." << std::endl;
	}
	else 
	{
		os << "Exported 'InvertedFile.bin' successfully." << std::endl;
		os << std::endl;
	}

	if (!outputOnlyJSON)
	std::cout << os.str();

	return success;
}

/**
* Affiche les résultats d'une recherche.
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeQuerySearch(const std::vector<std::string>& input_files, std::vector<std::string>& words)
{
	// LOAD THE ENTRIES OF THE INVERTED FILE CORRESPONDING TO THE QUERY WORDS

	InvertedFile_t IF;

	for (std::string& word : words)
	{
		if (!stemming_stopword(word)) word.clear();
	}
	words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word) { return word.empty(); }), words.end());

	if (!IFImportPart(IF, "InvertedFile.bin", words))
	{
		std::cerr << "ERROR: Unable to import 'InvertedFile.bin'." << std::endl;
		std::cerr << "Run this program with the argument 'index' to generate it." << std::endl;
		std::cerr << std::endl;
		return false;
	}

	if (IF.size() == 0)
	{
		std::cerr << "ERROR: None of the words of the query are in the InvertedFile!" << std::endl;
		std::cerr << "The search for documents matching query terms can't be done." << std::endl;
		std::cerr << std::endl;
		return false;
	}

	// for debug purposes only
	/*
	std::cout << "Words:" << std::endl;
	for (auto& it : IF)
	{
		auto& str = it.first;
		std::cout << "- " << str << " (" << it.second.size() << ")\n";
	}
	std::cout << std::endl;
	*/


	// SEARCH AND MEASURE SEARCH TIME

	auto IF_sorted = IF_sort(IF);

	auto to_ms = [] (auto duration) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	};
	auto timer_begin = [] () {return std::chrono::high_resolution_clock::now(); };
	auto timer_end = [&] (auto begin) {return to_ms(timer_begin() - begin); };

	auto timer = timer_begin();
	SearchResults_t resultsConjunction = resultsOrder(searchNaive(IF, aggregate_maps_AND_min), 10);
	auto time_naiveconjunction = timer_end(timer);

	timer = timer_begin();
	SearchResults_t resultsDisjunction = resultsOrder(searchNaive(IF, aggregate_maps_OR_max), 10);
	auto time_naivedisjunction = timer_end(timer);

	SearchResults_t resultsTA;
	std::chrono::milliseconds time_TA;
	{
		std::vector<TS> resultsTA_not_as_SearchResults_t;
		TA ta;
		timer = timer_begin();
		try {
			resultsTA_not_as_SearchResults_t = ta.TAlgo(10, IF_sorted);
		} catch (...) {
			// Sometimes unexpected exceptions happen...
		}
		time_TA = timer_end(timer);
		resultsTA = resultsTA_to_SearchResults_t(resultsTA_not_as_SearchResults_t);
	}

	SearchResults_t resultsFA;
	std::chrono::milliseconds time_FA;
	{
		std::vector<TS> resultsFA_not_as_SearchResults_t;
		FA fa;
		timer = timer_begin();
		try {
			fa.FAlgo(10, IF_sorted);
		} catch (...) {
			// Sometimes unexpected exceptions happen...
		}
		time_FA = timer_end(timer);
		resultsFA = resultsFA_to_SearchResults_t(fa.result);
	}


	// FIND TITLES OF DOCUMENTS IN THE SEARCH RESULTS

	std::map<DocumentUID_t, DocumentJSON_t> resultsJSON;
	auto loadDocument = [&] (DocumentUID_t ID)
	{
		if (resultsJSON.find(ID) != resultsJSON.end()) return; // document already loaded

		auto& documentJSON = resultsJSON[ID];
		try {
			loadDocumentJSON(input_files, ID, documentJSON);
		} catch (...) {
			// Not being able to load a document here is not so dramatic.
		}

		for (char& c : documentJSON.title) if (c == '\n') c = ' ';
	};

	std::vector<DocumentUID_t> resultsUIDs;
	for (auto& result : resultsConjunction) loadDocument(result.docID);
	for (auto& result : resultsDisjunction) loadDocument(result.docID);
	for (auto& result : resultsFA) loadDocument(result.docID);
	for (auto& result : resultsTA) loadDocument(result.docID);


	// SHOW THE RESULTS

	auto resultsShow = [&] (const SearchResults_t& results, const char* name, std::chrono::milliseconds duration_ms) {
		if (outputOnlyJSON) return;
		std::cout << "Top 10 (" << name << ", took " << duration_ms.count() << "ms):" << std::endl;
		int index = 0;
		for (auto& result : results)
		{
			std::cout
				<< (++index) << ". document " << result.docID
				<< " (score: " << freqTF(result.frequency) << ")";

			if (resultsJSON.find(result.docID) != resultsJSON.end())
			std::cout << ": \"" << strstrip(resultsJSON[result.docID].title).str() << "\"";

			std::cout
				<< std::endl;
		}
		std::cout << std::endl;
	};
	
	if (!outputOnlyJSON)
	{
		resultsShow(resultsConjunction, "naive search conjunction", time_naiveconjunction);
		resultsShow(resultsDisjunction, "naive search disjunction", time_naivedisjunction);

		resultsShow(resultsTA, "TA", time_TA);
		resultsShow(resultsFA, "FA", time_FA);
	}
	else
	{
		json j;
		j["TA"] = toJson(resultsTA);
		j["FA"] = toJson(resultsFA);
		std::cout << j.dump(4) << std::endl;
	}
	
	return true;
}

/**
* Affiche un document en JSON à partir de son UID.
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeQueryDocument(const std::vector<std::string>& input_files, FileDocumentUID_t documentID)
{
	DocumentJSON_t documentJSON;
	if (!loadDocumentJSON(input_files, documentID, documentJSON))
	{
		if (!outputOnlyJSON)
		{
			std::cerr
				<< "ERROR: Could not load the document with ID "
				<< documentID << "." << std::endl;
		}
		else
		{
			std::cout
				<< "{\"error\":\"Could not load the document with ID "
				<< documentID
				<< "\"}" << "\n" << std::flush;
		}

		return false;
	}

	std::cout << documentJSON << std::endl;

	return true;
}

/**
* Action par défaut (en l'absence d'arguments en ligne de commande).
* Pour l'instant, l'action par défaut consiste à vérifier qu'un InvertedFile a bien été généré.
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeDefault()
{
	std::ostringstream os;

	InvertedFile_t IF;

	os << "Importing 'InvertedFile.bin'..." << std::endl;
	bool success = IFImport(IF, "InvertedFile.bin");
	if (!success)
	{
		std::cerr << "Unable to import 'InvertedFile.bin'." << std::endl;
		std::cerr << "Run this program with the argument 'index' to generate it." << std::endl;
	}
	else
	{
		os << "Imported 'InvertedFile.bin' successfully." << std::endl;

		os << "InvertedFile contains " << IF.size() << " entries." << std::endl;
	}
	os << std::endl;

	if (!outputOnlyJSON)
	std::cout << os.str();

	return true;
}


// ==================================
// ========= Point d'entrée =========
// ==================================

int main(int argc, char * argv[])
{
	std::vector<std::string> input_files;
	if (!loadInputFiles(input_files))
	{
		std::cerr << "FATAL ERROR: Unable to load '" << inputsFilePath << "' containing the list of input files." << std::endl;
		return 1;
	}

	arguments_t arguments;
	if (!parseArgs(arguments, argc - 1, argv + 1))
	{
		if (argc <= 1)
		{
			std::cout << "WARNING: Missing command line arguments." << std::endl;
			std::cout << std::endl;
			std::cout << "Available arguments are:" << '\n'
				<< "- 'index' to (re)build the InvertedFile" << '\n'
				<< "- 'query <word> <word> ...' to search for articles" << '\n'
				<< "- 'document <ID>' to get the full contents of an article" << std::endl;
			std::cout << std::endl;
		}
		else
		{
			std::cerr << "ERROR: Unable to parse command line arguments." << std::endl;
			std::cerr << std::endl;
		}
	}

	try
	{
		bool success = false;

		if (arguments.index)
		{
			success = modeBuildInvertedFile(input_files);
		}
		else if (!arguments.query.empty())
		{
			success = modeQuerySearch(input_files, arguments.query);
		}
		else if (arguments.documentID > 0)
		{
			success = modeQueryDocument(input_files, FileDocumentUID_t(arguments.documentID));
		}
		else if (!arguments.index)
		{
			success = modeDefault();
		}

		return success ? 0 : 1;
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

		std::cerr << "Aborting." << std::endl;

		if (outputOnlyJSON)
		std::cout << "{\"error\":\"Unknown error (bad try catch)\"}" << std::endl;

		return 1;
	}
}
