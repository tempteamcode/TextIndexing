
#include <fstream>
#include <iostream>
#include <filesystem>

#include <string>
#include <sstream>

namespace fs = std::experimental::filesystem;


class Tokenizer
{
public:
	Tokenizer();

	std::string extract_token(std::istream& is);

private:
	bool char_is_text[256] = {};
};

Tokenizer::Tokenizer()
{
	for (char c = '0'; c <= '9'; c++)
		char_is_text[c] = true;

	for (char c = 'a'; c <= 'z'; c++)
		char_is_text[c] = true;

	for (char c = 'A'; c <= 'Z'; c++)
		char_is_text[c] = true;
}

std::string Tokenizer::extract_token(std::istream& is)
{
	std::ostringstream token;

	char c = {0};
	while (is.get(c) && !char_is_text[c] && c != '<');
	if (is)
	{
		if (c == '<')
		{
			for (;;)
			{
				token << c;
				if (c == '>') break;
				if (!is.get(c)) break;
			}
		}
		else
		{
			for (;;)
			{
				token << c;
				if (!is.get(c)) break;
				if (!char_is_text[c]) break;
			}
		}
	}

	return token.str();
}

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
		print_file_tokens(p.path().string());
		break;
	}
	
	return 0;
}
