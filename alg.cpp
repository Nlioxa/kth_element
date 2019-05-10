#include <algorithm>

template <class dtype, class ptype>
inline void med3_unchecked(dtype* first, dtype* mid, dtype* last, ptype& less)
{
	if( less(*mid, *first) )
	{
		std::iter_swap(mid, first);
	}
	if( less(*last, *mid) )
	{
		std::iter_swap(last, mid);
		if( less(*mid, *first) )
		{
			std::iter_swap(mid, first);
		}
	}
}

template<class dtype, class ptype>
void unchecked_median(dtype* first, dtype* mid, dtype* last, ptype& less)
{
	if( 40 < last - first )
	{
		auto step = ( last - first + 1 ) / 8;
		med3_unchecked(first, first + step, first + 2 * step, less);
		med3_unchecked(mid - step, mid, mid + step, less);
		med3_unchecked(last - 2 * step, last - step, last, less);
		med3_unchecked(first + step, mid, last - step, less);
	}
	else
	{
		med3_unchecked(first, mid, last, less);
	}

}

template<class dtype, class ptype>
std::pair<dtype*, dtype*> partition(dtype* first, dtype* last, ptype& pred)
{
	dtype* mid = first + ( last - first ) / 2;
	unchecked_median(first, mid, last - 1, pred);
	dtype* pfirst = mid;
	dtype* plast = pfirst + 1;

	while( first < pfirst && !pred(*pfirst, *( pfirst - 1 )) )
	{
		--pfirst;
	}
	while( plast < last && !pred(*pfirst, *plast) )
	{
		++plast;
	}

	dtype* gfirst = plast;
	dtype* glast = pfirst;

	while( true )
	{
		for( ; gfirst < last; ++gfirst )
		{
			if( pred(*gfirst, *pfirst) )
			{
				break;
			}
			else if( plast++ != gfirst )
			{
				std::iter_swap(plast - 1, gfirst);
			}
		}
		for( ; first < glast; --glast )
		{
			if( pred(*pfirst, *( glast - 1 )) )
			{
				break;
			}
			else if( --pfirst != glast - 1 )
			{
				std::iter_swap(pfirst, glast - 1);
			}
		}
		if( glast == first && gfirst == last )
		{
			return ( std::pair<dtype*, dtype*>(pfirst, plast) );
		}

		if( glast == first )
		{
			if( plast != gfirst )
			{
				std::iter_swap(pfirst, plast);
			}
			++plast;
			std::iter_swap(pfirst++, gfirst++);
		}
		else if( gfirst == last )
		{
			if( --glast != --pfirst )
			{
				std::iter_swap(glast, plast);
			}
			std::iter_swap(pfirst, --plast);
		}
		else
		{
			std::iter_swap(gfirst++, --glast);
		}
	}
}

template<class dtype, class ptype>
void nth_element(dtype* first, dtype* nth, dtype* last, ptype pred)
{
	const int isort_max = 32;

	if( nth == last )
		return;

	while( isort_max < last - first )
	{
		auto cut = partition(first, last, pred);

		if( cut.second <= nth )
		{
			first = cut.second;
		}
		else if( cut.first <= nth )
		{
			return;
		}
		else
		{
			last = cut.first;
		}
	}

	std::sort(first, last, pred);
}
