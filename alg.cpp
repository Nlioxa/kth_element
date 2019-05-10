#include <iostream>

using namespace std;

template<class type, class ptype> type* partition(type* first, type* last, ptype pred)
{
	const auto( pivot ) { first + rand() % ( distance(first, last) ) };
	swap(*pivot, *last);
	auto ( delimiter ) { first };
	for( auto( iter ) { first }; iter <= last - 1; iter++ )
	{
		if( pred(*iter, *last) )
		{
			swap(*delimiter, *iter);
			delimiter++;
		}
	}
	swap(*( delimiter ), *last);
	return delimiter;
}


template<class type, class ptype> void quick_sort(type* first, type *last, ptype pred)
{
	if( first < last )
	{
		auto delimiter = partition(first, last, pred);
		quick_sort(first, delimiter - 1, pred);
		quick_sort(delimiter + 1, last, pred);
	}
}

template<class type, class ptype> type kth_element(type* first, type* kth, type* last, ptype pred)
{
	while( distance(first, last) > 3 && !( *first == *kth || *last == *kth ) )
	{
		auto delimiter = partition(first, last, pred);
		if( delimiter < kth )
		{
			if (*delimiter == *kth)
			{
				return *delimiter;
			}
			first = delimiter;
		}
		else if( delimiter > kth )
		{
			last = delimiter;
		}
	}
	quick_sort(first, last, pred);
	return *kth;
}
