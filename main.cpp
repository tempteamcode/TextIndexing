#include <fstream>
#include <iostream>

#if __cplusplus >= 201703L
// built with C++17, C++20 support
#include <filesystem>
namespace fs = std::filesystem;
#else
// built with C++11, C++14 support
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

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

int main(int argc, char * argv[])
{
	InvertedFile_t IF;

	if (fs::exists("InvertedFile.bin"))
	{
		IFImport(IF, "InvertedFile.bin");
		std::cout << "InvertedFile.bin" << std::endl;
		std::cout << IF.size() << " entries" << std::endl;
		return 0;
	}

	if (!fs::exists("latimes/"))
	{
		// fs::create_directories("latimes/");
		std::cerr << "directory 'latimes' not found" << std::endl;
		return 1;
	}

	for (auto& p : fs::directory_iterator("latimes/"))
	{
		const std::string path = p.path().string();

		std::cout << path << std::endl;
		// print_file_tokens(path);
		// print_file_documents(path);
		try {
			process_file(path, [&] (DocumentData_t& document) {
				invertedFileAdd(IF, document.DOCID, document.TEXT);
				// std::cout << TFID.size() << std::endl;
			});
			// break;
		}
		catch (custom_exception& e)
		{
			switch (e)
			{
			case custom_exception::fs_access:
				std::cerr << "ERROR: denied access to '" << path << "'" << std::endl; break;
			case custom_exception::parsing_error:
				std::cerr << "ERROR: incorrect tag hierarchy in '" << path << "'" << std::endl; break;
			}
		}
	}

	IFExport(IF, "InvertedFile.bin");
	
	return 0;
}
