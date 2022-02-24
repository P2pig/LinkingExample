#include <stdio.h>

extern "C" char const* __cdecl greeting();

int main()
{
	puts(greeting());
}