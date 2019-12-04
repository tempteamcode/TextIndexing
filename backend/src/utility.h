#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>
#include <chrono>

/**
* Fonction pour chronométrer l'exécution d'une autre fonction en millisecondes.
* 
* // exemple, pour mesurer l'exécution de "r = f(true, 'A', 0);" :
* auto duration = measure_milliseconds(r, f, true, 'A', 0);
**/

template <typename Function_t, typename Result_t, typename ...Args>
std::chrono::milliseconds measure_milliseconds(Result_t& result, Function_t function, Args&& ...args)
{
	auto begin = std::chrono::high_resolution_clock::now();
	result = function(std::forward<Args>(args)...);
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	return duration;
}

/// Permet de limiter les sorties sur cout aux données JSON.
extern bool outputOnlyJSON;

/// Exceptions personnalisées générées par notre programme.
enum class custom_exception
{
	fs_access,
	parsing_error,
	no_tagordata,
	no_token,
};

extern const std::vector<std::string> names_months;
extern const std::vector<std::string> names_weekdays;

/// Structure servant à stocker une date (année, mois, jour, et éventuellement jour de la semaine).
struct Date_t
{
	enum class month_t : unsigned char {unknown = 0, january, february, march, april, may, june, july, august, september, october, november, december};
	enum class weekday_t : unsigned char {unknown = 0, monday, tuesday, wednesday, thursday, friday, saturday, sunday};

	unsigned short year = 0;
	month_t month = month_t::unknown;
	unsigned char day = 0;
	weekday_t weekday = weekday_t::unknown;

	bool empty() const
	{
		return ((year | static_cast<unsigned char>(month) | day | static_cast<unsigned char>(weekday)) == 0);
	}

	operator std::string() const;
};

/**
* La classe string_view permet de définir des sous-chaînes de chaînes existantes sans faire une copie de tous leurs caractères.
* 
* Elle repose sur le même principe (même si elle est bien plus simple) que la classe std::string_view apparue en C++17,
* que nous n'utilisons pas car (pour des raisons de compilateurs utilisés) notre programme doit être compatible avec C++14.
**/
class string_view
{
public:
	std::string::const_iterator begin;
	std::string::const_iterator end;
	
	string_view() = default;

	/// Construction d'une string_view sur une chaîne complète.
	string_view(const std::string& str)
		: begin(str.cbegin()), end(str.cend())
	{}
	
	/// Construction d'une string_view à partir d'itérateurs sur une chaîne de début et de fin.
	string_view(std::string::const_iterator begin, std::string::const_iterator end)
		: begin(begin), end(end)
	{}

	/// Retourne une copie de la chaîne en tant que std::string. 
	inline std::string str() const
	{
		return std::string(begin, end);
	}

	/** Opérateur de conversion en std::string.
	* Les conversions implicites sont désactivées pour éviter des conversions non indispensables.
	**/
	inline explicit operator std::string() const
	{
		return str();
	}
};

/// Renvoit si un caractère ASCII est une lettre.
inline bool isCharAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/// Renvoit si un caractère ASCII est un chiffre.
inline bool isCharNumeric(char c) {
	return (c >= '0' && c <= '9');
}

/** Teste si deux chaînes de caractères sont égales.
* Il est plus efficace d'utiliser cette fonction que de convertir les chaînes en std::string,
* puis utiliser l'opérateur de comparaison sur des std::string fourni par le standard.
**/
inline bool streq(const char* str1, const char* str2)
{
	for (;;)
	{
		if (*str1 != *str2) return false;
		if (*str1 == '\0') return true;
		str1++; str2++;
	}
}

/// Teste si deux chaînes de caractères sont égales (l'une étant représentée par une string_view).
bool operator ==(const string_view& str_view, const char* text);

/// Renvoit une string_view de la chaîne fournie sans les éventuels espaces et sauts de ligne en début et en fin.
string_view strstrip(const std::string& text);

/// Modifie une chaîne de caractères pour transformer les majuscules en minuscules.
inline void toLowerCase(std::string& text)
{
	for (unsigned int i = 0, len = text.length(); i < len; i++)
	{
		char c = text[i];
		if (c >= 'A' && c <= 'Z')
		{
			text[i] = c - 'A' + 'a';
		}
	}
}

/// Modifie une chaîne de caractères pour transformer les minuscules en majuscules.
inline void toUpperCase(std::string& text)
{
	for (unsigned int i = 0, len = text.length(); i < len; i++)
	{
		char c = text[i];
		if (c >= 'a' && c <= 'z')
		{
			text[i] = c - 'a' + 'A';
		}
	}
}

/// Tente de convertir un texte en entier. Renvoit true en cas de succès.
bool tryParseInt(const string_view& chars, unsigned int& value);

/// Tente de convertir un texte en entier. Renvoit true en cas de succès.
bool tryParseInt(const std::string& chars, unsigned int& value);

/// Tente de convertir un texte en date. Renvoit true en cas de succès.
bool tryParseDate(const std::vector<std::string>& words, Date_t& date);

/**
* Réunit plusieurs chaînes en une seule avec une complexité linéaire.
* La chaîne résultante est allouée une seule fois avec sa taille définitive pour éviter des réallocations lors de chaque concaténation.
**/
std::string stringJoin(const std::vector<std::string>& data, const std::string& sep);
std::string stringJoin(const std::list<std::string>& data, char sep = ' ');

/// Renvoit l'intersection de deux std::map.
template <typename Key, typename Value, typename MergedValue, class Aggregator_t>
std::map<Key, MergedValue> mapIntersection(const std::map<Key, Value>& lhs, const std::map<Key, Value>& rhs, Aggregator_t aggregator)
{
	std::map<Key, MergedValue> result;
	auto endl = lhs.cend();
	auto endr = rhs.cend();
	for (auto itl = lhs.cbegin(), itr = rhs.cbegin();;)
	{
		if (itl->first == itr->first)
		{
			result[itl->first] = aggregator(itl->second, itr->second);
			if (++itl == endl || ++itr == endr) break;
		}
		else if (itl->first < itr->first)
		{
			if (++itl == endl) break;
		}
		else
		{
			if (++itr == endr) break;
		}
	}
	return result;
}

/// Renvoit l'union de deux std::map.
template <typename Key, typename Value, typename MergedValue, class Aggregator_t, class AggregatorUnique_t>
std::map<Key, MergedValue> mapUnion(const std::map<Key, Value>& lhs, const std::map<Key, Value>& rhs, Aggregator_t aggregator, AggregatorUnique_t aggregatorUnique)
{
	std::map<Key, MergedValue> result;
	auto itl = lhs.cbegin(), endl = lhs.cend();
	auto itr = rhs.cbegin(), endr = rhs.cend();
	for (;;)
	{
		if (itl->first == itr->first)
		{
			result[itl->first] = aggregator(itl->second, itr->second);
			++itl;
			++itr;
			if (itl == endl || itr == endr) break;
		}
		else if (itl->first < itr->first)
		{
			result[itl->first] = aggregatorUnique(itl->second);
			if (++itl == endl) break;
		}
		else if (itr->first < itl->first)
		{
			result[itr->first] = aggregatorUnique(itr->second);
			if (++itr == endr) break;
		}
	}
	for (; itl != endl; ++itl) result[itl->first] = aggregatorUnique(itl->second);
	for (; itr != endr; ++itr) result[itr->first] = aggregatorUnique(itr->second);
	return result;
}
