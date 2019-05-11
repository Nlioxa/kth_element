#include <algorithm>

template<class type>
void median_filter_(type* signal, int signal_size, int window_size)
{
	using namespace std;

	type* window = new type[window_size];
	type** _window = new type*[window_size];

	// construct a window for the first element of the signal 
	for( auto ( cell ) { window }, ( median_cell ) { window + window_size / 2 }, ( end_cell ) { window + window_size - 1 };
		cell <= end_cell; cell++ )
	{
		*cell = cell < median_cell ? 0 : *( signal + ( cell - median_cell ) );
	}

	// assign pointer to each cell of the window
	for( auto ( index ) { 0 }; index < window_size; index++ )
	{
		_window[index] = &window[index];
	}

	// the first and the least sort of the window
	sort(_window, _window + window_size,
		[](type* a, type* b) {return *a < *b; });

	// set a pointer to the median cell of the sorted window
	auto ( median_cell ) { _window + window_size / 2 };

	signal[0] = **median_cell;

	for( auto ( current_element ) { signal }, ( first_element ) { signal }, ( last_element ) { signal + signal_size - 1 }, ( dated_cell ) { window };
		current_element < last_element; current_element++ )
	{
		auto ( indent ) { static_cast<int>( ceil(window_size / 2.0) ) };
		auto ( next_element ) { current_element + indent };
		auto ( next_value ) { next_element <= last_element ? *next_element : 0 };
		*dated_cell = next_value;
		dated_cell = dated_cell + 1 < window + window_size ? dated_cell + 1 : window;
		sort(_window, _window + window_size,
			[](type* a, type* b) {return *a < *b; });
		*( current_element + 1 ) = window_size % 2 ? **median_cell : ( **median_cell + **( median_cell - 1 ) ) / 2;
	}
}
