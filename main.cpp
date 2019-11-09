//#define USE_EXPERIMENTAL_FILESYSTEM
#define USE_FILESYTEM
//#define USE_CUSTOM_FILESYSTEM



#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "Tokenizer.h"
#include "DocExtractor.h"
#include "DocProcessor.h"
#include "InvertedFileWriter.h"

#if defined( USE_EXPERIMENTAL_FILESYSTEM )
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#elif defined( USE_FILESYTEM )
	#include <filesystem>
	namespace fs = std::filesystem;
#else
	namespace fs = std;
#endif


void print_file_tokens(const std::string& path)
{
	Tokenizer tokenizer;

	std::ifstream is(path);
	if (!is) throw "error reading file at" + path;

	std::string token;
	do
	{
		token = tokenizer.extract(is);

		std::cout << token << std::endl;

	} while (token.size() != 0);
}

void print_file_documents(const std::string& path)
{
	Tokenizer tokenizer;
	DocExtractor docextractor([] (DocumentData& doc) {
		std::cout << doc.DOCNO[0] << std::endl;
	});

	std::ifstream is(path);
	if (!is) throw "error reading file at" + path;

	do
	{
		docextractor.parseToken(tokenizer.extract(is));
	} while (is);
}

typedef void (*TFID_callback) (int docID, TFID_t TFID);
int docid;

TFID_callback callbackforTFID;

void process_file(const std::string& path, TFID_callback callback)
{
	callbackforTFID = callback;
	docid = 0;

	Tokenizer tokenizer;
	DocExtractor docextractor([] (DocumentData& doc) {
		TFID_t TFID = produceTFID(doc);
		callbackforTFID(++docid, TFID);
	});

	std::ifstream is(path);
	if (!is) throw "error reading file at" + path;

	for(;;)
	{
		std::string token = tokenizer.extract(is);
		if (token.empty()) break;
		docextractor.parseToken(token);
	}
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