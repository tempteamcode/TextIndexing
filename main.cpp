#include <fstream>
#include <iostream>

#if __cplusplus > 201703L
// built with C++20 support
	#include <filesystem>
	namespace fs = std::filesystem;
#else
// built with C++11, C++14, C++17 support
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#endif

#include "Tokenizer.h"
#include "DocumentExtractor.h"
#include "DocumentParser.h"
#include "InvertedFileWriter.h"


void print_file_tokens(const std::string& path)
{
	Tokenizer tokenizer;

	std::ifstream is(path);
	if (!is) throw "error reading file '" + path + "'";

	for (;;) {
		std::string token = tokenizer.extract(is);
		if (token.empty()) break;
		std::cout << token << std::endl;
	}
}

void print_file_documents(const std::string& path)
{
	Tokenizer tokenizer;
	DocumentExtractor docextractor;

	std::ifstream is(path);
	if (!is) throw "error reading file '" + path + "'";

	for (;;) {
		std::string token = tokenizer.extract(is);
		if (token.empty()) break;
		docextractor.parseToken(token);
	}

	extractDocuments(docextractor.getDocument(), [] (DocumentData_t& document) {
		std::cout << document.DOCNO << std::endl;
	});
}

typedef void (*TFID_callback) (int docID, TFID_t TFID);
int docid;

TFID_callback callbackforTFID;

void process_file(const std::string& path, TFID_callback callback)
{
	callbackforTFID = callback;
	docid = 0;

	Tokenizer tokenizer;
	DocumentExtractor docextractor;

	std::ifstream is(path);
	if (!is) throw "error reading file '" + path + "'";

	for(;;) {
		std::string token = tokenizer.extract(is);
		if (token.empty()) break;
		docextractor.parseToken(token);
	}

	extractDocuments(docextractor.getDocument(), [] (DocumentData_t& document) {
		TFID_t TFID = produceTFID(document);
		callbackforTFID(++docid, TFID);
	});
}

InvertedFile_t IF;

int main(int argc, char * argv[])
{
	IF.clear();

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
		process_file(p.path().string(), [] (int docID, TFID_t TFID) {
			invertedFileAdd(IF, docID, TFID);
			// std::cout << TFID.size() << std::endl;
		});
		// break;
	}
	
	return 0;
}
