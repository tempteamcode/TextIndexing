
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Tokenizer.h"
#include "DocExtractor.h"

namespace fs = std::experimental::filesystem;

void print_file_tokens(const std::string& path)
{
	Tokenizer tokenizer;

	std::ifstream is(path);
	if (!is) throw std::exception("");

	std::string token;
	do
	{
		token = tokenizer.extract_token(is);

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
	if (!is) throw std::exception("");

	do
	{
		docextractor.parseToken(tokenizer.extract_token(is));
	} while (is);
}


int main(int argc, char * argv[])
{
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
		print_file_documents(p.path().string());
		break;
	}
	
	return 0;
}
