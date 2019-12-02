/**
 * @file Stemmer.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief 
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
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
