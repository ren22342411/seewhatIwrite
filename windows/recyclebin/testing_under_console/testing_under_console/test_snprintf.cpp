#include "stdafx.h"
#include "test_snprintf.h"

int test_snprintf_main()
{
	char type_char = 'd';
	char format_string[8] = {0};
	_snprintf (format_string,  8, "%%%c", type_char);
	std::cout << format_string << endl;

	char out_string[7] = {0};
	long i = 345678;
	int result = _snprintf (out_string,  32, format_string, i);
	std::cout << out_string << '\t' << result << endl;

	return 0;
}