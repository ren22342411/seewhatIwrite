#include "stdafx.h"
#include "unsigned_char_char.h"

void unsigned_char_char_main( void )
{
	unsigned char uch = 'A';
	char ch = 'A';

	cout << "uch = " << uch << endl;

	cout << "ch = " << ch << endl;

	printf("%c %c %d %d %u %u\n", uch, ch, uch, ch, uch, ch);

}
