#include <stdio.h>
#include <Windows.h>

int main()
{
	HMODULE const HelloDll = LoadLibraryExW(L"Hello.dll", 	nullptr, 0);
	
	// char const* __cdecl GetGreeting();
	using GetGreetingType = char const* (__cdecl*)();
	
	// ask loader to give the address of this function
	GetGreetingType const GetGreeting =
		reinterpret_cast<GetGreetingType>(
		GetProcAddress(HelloDll, "greeting"));

	puts(GetGreeting());
	
	FreeLibrary(HelloDll);
}