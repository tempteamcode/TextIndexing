#include <fstream>
#include <iostream>

#include "src/files.h"
#include "src/Tokenizer.h"
#include "src/DocumentExtractor.h"
#include "src/DocumentParser.h"
#include "src/InvertedFile.h"
#include "src/search.h"
#include "src/utility.h"
#include "src/TA.h"
#include "src/FA.h"

typedef unsigned_ints FileDocumentUID_t;


std::string ressourcesRootFolder 	= "./ressources/";
std::string inputsFilePath				= ressourcesRootFolder + "inputs.txt";



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
		std::cerr << tagordata << std::endl;
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
		std::cerr << document.DOCNO << std::endl;
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
	std::cerr << "Parsing the input files..." << std::endl;

	FileDocumentUID_t filedocumentID;

	filedocumentID.first = 0;
	for (const std::string& path : input_files)
	{
		if (path.empty()) continue;

		std::cerr << path << std::endl;
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

	std::cerr << std::endl;
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
	unsigned int documentID = 0;

	arguments_t()
	: index(false), query(), documentID(0) {}

	void clear() {
		index = false;
		query.clear();
		documentID = 0;
	}
};

/// Extrait et parse les arguments passés en ligne de commande.
bool parseArgs(arguments_t& arguments, int argc, char * argv[])
{
	arguments.clear();

	if (argc <= 0) return false;

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


// ======== Modes d'utilisation

/**
* Génère l'InvertedFile (et affiche la progression).
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeBuildInvertedFile(const std::vector<std::string>& input_files)
{
	InvertedFile_t IF;

	std::cerr << "Generating the InvertedFile..." << std::endl;
	makeInvertedFile(input_files, IF);
	std::cerr << "Generated the InvertedFile successfully." << std::endl;
	std::cerr << std::endl;

	std::cerr << "Exporting 'InvertedFile.bin'..." << std::endl;
	if (!IFExport(IF, "InvertedFile.bin"))
	{
		std::cerr << "FATAL ERROR: Unable to export the InvertedFile in 'InvertedFile.bin'." << std::endl;
		return false;
	}
	std::cerr << "Exported 'InvertedFile.bin' successfully." << std::endl;
	std::cerr << std::endl;

	return true;
}

/**
* Affiche les résultats d'une recherche.
* @return true en cas de succès, false si une erreur s'est produite
*/
bool modeQuerySearch(std::vector<std::string>& words)
{
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
		std::cout << "null" << std::endl;
		return false;
	}


	std::vector<std::vector<TA::TF>> tab;
	for (auto& it: IF)
	{
		auto& vec = it.second;
		std::vector<TA::TF> row; row.reserve(vec.size());
		for (auto& elem : vec)
		{
			TA::TF item;
			item.d = elem.first;
			item.frequency = freqTF(elem.second);
			row.push_back(item);
		}
		std::sort(row.begin(), row.end(), [] (TA::TF& f1, TA::TF& f2) {
			return (f1.frequency > f2.frequency) || (f1.frequency == f2.frequency && f1.d < f2.d);
		});
		tab.push_back(row);
	}

	TA ta;
	std::vector<TA::TS> result = ta.TAlgo(10, tab);

	SearchResults_t resultsTA;
	for (auto& res : result)
	{
		DocFreq_t freq;
		freq.docID = res.d;
		freq.frequency.first = res.score * 50000;
		freq.frequency.second = 50000;
		resultsTA.push_back(freq);
	}

	FA fa;
	fa.FAlgo(10, tab);

	SearchResults_t resultsFA;
	for (auto& res : fa.result)
	{
		DocFreq_t freq;
		freq.docID = res.d;
		freq.frequency.first = res.score * 50000;
		freq.frequency.second = 50000;
		resultsFA.push_back(freq);
	}

	// auto aggregate_maps_AND_min = [&](auto& lhs, auto& rhs) {
	// 	return mapIntersection<DocumentUID_t, Frequency_t, Frequency_t, decltype(val_min<Frequency_t>)>(lhs, rhs, val_min<Frequency_t>);
	// };
	// auto aggregate_maps_OR_max = [&](auto& lhs, auto& rhs) {
	// 	return mapUnion<DocumentUID_t, Frequency_t, Frequency_t, decltype(val_max<Frequency_t>), decltype(val_self<Frequency_t>)>(lhs, rhs, val_max<Frequency_t>, val_self<Frequency_t>);
	// };
	//
	// auto resultsConjunction = resultsOrder(searchNaive(IF, aggregate_maps_AND_min), 10);
	// auto resultsDisjunction = resultsOrder(searchNaive(IF, aggregate_maps_OR_max), 10);

	auto resultsShow = [&](const SearchResults_t& results, const char* name) {
		std::cerr << "Top 10 (" << name << "):" << std::endl;
		int index = 0;
		for (auto& result : results)
		{
			std::cerr
				<< (++index) << ". document " << result.docID
				<< " (frequency: " << freqTF(result.frequency) << ")"
				<< std::endl;
		}
		std::cerr << std::endl;
	};
	//
	// resultsShow(resultsConjunction, "naive search conjunction");
	// resultsShow(resultsDisjunction, "naive search disjunction");
	resultsShow(resultsTA, "TA");
	resultsShow(resultsFA, "FA");

	json j;
	j["TA"] = toJson( resultsTA );
	j["FA"] = toJson( resultsFA );

	std::cout << j.dump(4) << std::endl;
	
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
		std::cout
			<< "{\"error\":\"Could not load the document with ID "
			<< documentID
			<< "\"}" << "\n" << std::flush;

		std::cerr
			<< "ERROR: Could not load the document with ID "
			<< documentID << "." << std::endl;

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
	InvertedFile_t IF;

	std::cerr << "Importing 'InvertedFile.bin'..." << std::endl;
	if (!IFImport(IF, "InvertedFile.bin"))
	{
		std::cerr << "Unable to import 'InvertedFile.bin'." << std::endl;
		std::cerr << "Run this program with the argument 'index' to generate it." << std::endl;
		return false;
	}
	std::cerr << "Imported 'InvertedFile.bin' successfully." << std::endl;

	std::cerr << "InvertedFile contains " << IF.size() << " entries." << std::endl;
	std::cerr << std::endl;

	return true;
}

// =================================
// ======== Where the magic happens!
// =================================

// ======== Point d'entrée

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
				<< "- 'document_json <ID>' to get the full contents of an article" << std::endl;
			std::cout << std::endl;
		}
		else
		{
			std::cerr << "ERROR: Unable to parse command line arguments." << std::endl;
			std::cout << std::endl;
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
			success = modeQuerySearch(arguments.query);
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
		std::cout << "{\"error\":\"Unknown error (bad try catch)\"}" << std::endl;
		return 1;
	}
}
