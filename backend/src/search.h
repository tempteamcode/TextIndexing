/**
 * @file search.h
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
#include <iterator>
#include <algorithm>

#include "InvertedFile.h"

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


auto aggregate_maps_AND_min = [&](auto& lhs, auto& rhs) { return mapIntersection<DocumentUID_t, Frequency_t, Frequency_t, decltype(val_min<Frequency_t>)>(lhs, rhs, val_min<Frequency_t>); };
auto aggregate_maps_OR_max = [&](auto& lhs, auto& rhs) { return mapUnion<DocumentUID_t, Frequency_t, Frequency_t, decltype(val_max<Frequency_t>), decltype(val_self<Frequency_t>)>(lhs, rhs, val_max<Frequency_t>, val_self<Frequency_t>); };


typedef std::vector<DocFreq_t> SearchResults_t;
typedef std::map<DocumentUID_t, Frequency_t> result_t;


/*
inline SearchResults_t resultsOrder(result_t& results, unsigned int maxcount = 0)
{
	SearchResults_t searchresults(results.begin(), results.end());
	sort(searchresults.begin(), searchresults.end(), [] (DocFreq_t& lhs, DocFreq_t& rhs) { return lhs.frequency >= rhs.frequency; });
	if (maxcount != 0 && searchresults.size() > maxcount) searchresults.resize(maxcount);
	return searchresults;
}
*/

inline SearchResults_t resultsOrder(result_t& results, unsigned int maxcount = 0)
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
 * @brief Aggregates all inverted file entries according to given aggregator policy
 * @return result_t Returns aggregated Document ID to Frequency map
 * @todo improve this comment
 */
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
