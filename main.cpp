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

#include "Tokenizer.h"
#include "DocumentExtractor.h"
#include "DocumentParser.h"
#include "InvertedFileWriter.h"


void print_file_chunks(const std::string& path)
{
	PreprocessorTagsData preproc;

	std::ifstream is(path);
	if (!is) throw "error reading file '" + path + "'";

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
	if (!is) throw "error reading file '" + path + "'";

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
	if (!is) throw "error reading file '" + path + "'";

	while (is) {
		std::string tagordata = preproc.separateTagsData(is);
		docextractor.parseTagOrData(tagordata);
	}

	extractDocuments(docextractor.getDocument(), callback);
}

int main(int argc, char * argv[])
{
	InvertedFile_t IF;

	if (!fs::exists("latimes/"))
	{
		// fs::create_directories("latimes/");
		std::cerr << "directory 'latimes' not found" << std::endl;
		return 1;
	}

	for (auto& p : fs::directory_iterator("latimes/"))
	{
		std::cout << p.path() << '\n';
		// print_file_tokens(p.path().string());
		// print_file_documents(p.path().string());
		process_file(p.path().string(), [&] (DocumentData_t& document) {
			invertedFileAdd(IF, document.DOCID, document.TEXT);
			// std::cout << TFID.size() << std::endl;
		});
		// break;
	}
	
	return 0;
}
