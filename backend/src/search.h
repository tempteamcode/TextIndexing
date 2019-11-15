#pragma once

#include <vector>
#include <string>
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


typedef std::map<DocumentUID_t, Frequency_t> result_t;


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
