#include <algorithm>

template<class type>
type* construct_window(type* signal, const int window_size)
{
	const auto ( median_indent ) { window_size / 2 };

	auto window = new type[window_size];

	for( auto ( element ) { signal - median_indent }, ( cell ) { window }; element <= signal + median_indent; element++ )
	{
		*( cell++ ) = element >= signal ? *element : 0;
	}

	return window;
}

template<class type>
type** get_pointers(type* window, int window_size)
{
	auto ptr_window = new type*[window_size];

	for( auto ( index ) { 0 }; index < window_size; index++ )
	{
		ptr_window[index] = &window[index];
	}

	return ptr_window;
}

template<class type>
void median_filter_(type* signal, int signal_size, int window_size)
{
	using namespace std;

	auto ( window ) { construct_window(signal, window_size) };
	auto ( window_ptrs ) { get_pointers(window, window_size) };

	// the first and the least sort of the window
	sort(window_ptrs, window_ptrs + window_size,
		[](type* a, type* b) {return *a < *b; });

	const auto ( median_indent ) { window_size / 2 };
	const auto ( median_cell_ptr ) { window_ptrs + median_indent };

	signal[0] = **median_cell_ptr;

	// the main algorithm
	const auto ( next_element_indent ) { median_indent + window_size % 2 };
	for( auto ( current_element ) { signal }, ( first_element ) { signal }, ( last_element ) { signal + signal_size - 1 }, ( dated_cell ) { window };
		current_element < last_element; current_element++, dated_cell++ )
	{
		dated_cell = dated_cell < window + window_size ? dated_cell : window;

		const auto ( next_element ) { current_element + next_element_indent };
		const auto ( next_value ) { next_element <= last_element ? *next_element : 0 };

		*dated_cell = next_value;

		auto ( target_cell ) { find(window_ptrs, window_ptrs + window_size, dated_cell) };
		while( **target_cell > **( target_cell + 1 < window_ptrs + window_size ? target_cell + 1 : target_cell ) )
		{
			iter_swap(target_cell, target_cell + 1);
			target_cell++;
		}
		while( **target_cell < **( target_cell - 1 >= window_ptrs ? target_cell - 1 : window_ptrs ) )
		{
			iter_swap(target_cell, target_cell - 1);
			target_cell--;
		}
		*( current_element + 1 ) = window_size % 2 ? **median_cell_ptr : ( **median_cell_ptr + **( median_cell_ptr - 1 ) ) / 2;
	}
}
