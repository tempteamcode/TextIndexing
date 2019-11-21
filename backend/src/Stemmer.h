#pragma once

#include <vector>
#include <string>

void stem(char* word);

inline void stem(std::string& word)
{
	std::vector<char> buffer(word.begin(), word.end());
	buffer.push_back('\0');
	stem(buffer.data());
	word = buffer.data();
}
