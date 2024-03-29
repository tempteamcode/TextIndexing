#include "files.h"

template<>
void binWrite<unsigned int>(std::ostream& os, const unsigned int& value)
{
	unsigned char bits[4];
	for (int b = 0; b < 4; b++)
	{
		bits[b] = (value >> (8 * b)) & 0xFF;
	}
	os.write(reinterpret_cast<char*>(&bits[0]), 4);
}
template<>
void binRead<unsigned int>(std::istream& is, unsigned int& value)
{
	unsigned char bits[4];
	is.read(reinterpret_cast<char*>(&bits[0]), 4);
	value = (((bits[3]) * 256 + bits[2]) * 256 + bits[1]) * 256 + bits[0];
}
template<>
void binSkip<unsigned int>(std::istream& is)
{
	is.ignore(4);
}

template<typename T>
void binWrite( std::ostream& os, const T& value )
{
	char bits[sizeof(T)];
	const char* ptr = reinterpret_cast<const char*>(&value);
	for (int b = 0; b < sizeof(T); b++)
	{
		bits[b] = *ptr++;
	}
	os.write(&bits[0], sizeof(T));
}
template<typename T>
void binRead( std::istream& is, T& value )
{
	char bits[sizeof(T)];
	is.read(&bits[0], sizeof(T));
	char* ptr = reinterpret_cast<char*>(&value);
	for (int b = 0; b < sizeof(T); b++)
	{
		*ptr++ = bits[b];
	}
}
template<typename T>
void binSkip( std::istream& is )
{
	is.ignore(sizeof(T));
}

template<>
void binWrite<std::string>(std::ostream& os, const std::string& text)
{
	unsigned int length = text.length();
	binWrite<unsigned int>(os, length);
	if (length > 0) os.write(text.c_str(), length);
}
template<>
void binRead<std::string>(std::istream& is, std::string& text)
{
	unsigned int length; binRead(is, length);
	if (length > 0)
	{
		char* buffer = new char[length + 1];
		is.read(buffer, length);
		buffer[length] = '\0';
		text = buffer;
		delete[] buffer;
	}
	else text.clear();
}
template<>
void binSkip<std::string>(std::istream& is)
{
	unsigned int length; binRead(is, length);
	if (length > 0) is.ignore(length);
}

template
void binWrite<unsigned_ints>(std::ostream& os, const unsigned_ints& value);
template
void binRead<unsigned_ints>(std::istream& is, unsigned_ints& value);
template
void binSkip<unsigned_ints>(std::istream& is);
/*
template
void binWrite<float>(std::ostream& os, const float& value);
template
void binRead<float>(std::istream& is, float& value);
template
void binSkip<float>(std::istream& is);
*/
