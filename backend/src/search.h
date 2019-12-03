#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

#include "InvertedFile.h"

#include "../includes/json.hpp"
// for convenience
using json = nlohmann::json;




template<typename val_t>
val_t val_min(val_t val1, val_t val2)
{
	return val1 < val2 ? val1 : val2;
}
template<typename val_t>
val_t val_max(val_t val1, val_t val2)
{
	return val1 > val2 ? val1 : val2;
}

template<typename val_t>
val_t val_self(val_t val)
{
	return val;
}
template<typename val_t>
val_t val_zero(val_t)
{
	return 0;
}



typedef std::vector<DocFreq_t> SearchResults_t;
typedef std::map<DocumentUID_t, Frequency_t> result_t;

inline json toJson( const SearchResults_t & sr ) {

	json j;
	j["results"] = json::array();

	for( auto & docFreq : sr ) {
		auto jDF = json::object();
		jDF["documentID"] = docFreq.docID;
		jDF["count"] = docFreq.frequency.first;
		jDF["global_word_count"] = docFreq.frequency.second;
		j["results"].push_back( jDF );
	}

	return j;

}


/*
inline SearchResults_t resultsOrder(result_t& results, unsigned int maxcount = 0)
{
	SearchResults_t searchresults(results.begin(), results.end());
	sort(searchresults.begin(), searchresults.end(), [] (DocFreq_t& lhs, DocFreq_t& rhs) { return lhs.frequency >= rhs.frequency; });
	if (maxcount != 0 && searchresults.size() > maxcount) searchresults.resize(maxcount);
	return searchresults;
}
*/



/// Ordonne les résultats d'une recherche et ne conserve que les "maxcount" premiers.
inline SearchResults_t resultsOrder( const result_t& results, unsigned int maxcount = 0)
{
	SearchResults_t searchresults;
	searchresults.reserve(results.size());
	for (auto& result : results)
	{
		searchresults.push_back({ result.first, result.second });
	}
	sort(searchresults.begin(), searchresults.end(), [](DocFreq_t& lhs, DocFreq_t& rhs) { return freqTF(lhs.frequency) >= freqTF(rhs.frequency); });
	if (maxcount != 0 && searchresults.size() > maxcount) searchresults.resize(maxcount);
	return searchresults;
}


/**
* Effectue une recherche naïve sur un InvertedFile selon donné un agrégateur.
* Ainsi, searchNaive<aggregate_maps_OR_max> aggrège les résultats de façon disjonctive en conservant la fréquence maximale des termes dans les documents.
* searchNaive<aggregate_maps_AND_min> aggrège les résultats de façon conjonctive en conservant la fréquence minimale des termes dans les documents.
**/
template <class Aggregator_t>
inline result_t searchNaive(const InvertedFile_t& IF, Aggregator_t aggregator)
{
	if (IF.empty()) return result_t();

	auto it = IF.begin();
	const auto& first = (*it).second;

	result_t results(first.begin(), first.end());

	for (++it; it != IF.end(); ++it)
	{
		const auto& other = (*it).second;
		result_t results_other(other.begin(), other.end());

		results = aggregator(results, results_other);
	}

	return results;
}
