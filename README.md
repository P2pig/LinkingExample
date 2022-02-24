------------------------------------------------------

A:\>type Hello.cpp
extern "C" char const* __cdecl GetGreeting()
{
	return "Hello, C++ Programmers!";
}

DLL code with "A calling convension __cdecl"
you can pick any calling convension just as long 
As you use the same one on both sides.
------------------------------------------------------
A:\>cl /c Hello.cpp

cl compile the Hello.cpp and it does not link it.
it will produce Hello.obj
------------------------------------------------------
A:\> link Hello.obj /DLL /NOENTRY /EXPORT:GetGreeting

Produce Hello.lib and Hello.dll:
takes Hello.obj and pass DLL option to the linker
dont build an exe
noentry to reduce entry stuff
use /export to tell "GetGreeting" is a "public interface of this DLL"

=========================================================
# Explicit Linking

A:\type PrintGreeting.cpp
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
		GetProcAddress(HelloDll, "GetGreeting"));

	puts(GetGreeting());
	
	FreeLibrary(HelloDll);
}
==========================================================
--------------------------------------------------------
A:\>cl PrintGreeting.cpp
A:\>PrintGreeting.exe
Hello, C++ Programmers!
>dumpbin /dependents printimplicit.exe
KERNEL32.dll

--------------------------------------------------------
# implicit linking
A:\type printImplicit.cpp
#include <stdio.h>
extern "C" char const* __cdecl GetGreeting();
int main()
{	
	puts(GetGreeting());
}
---------------------------------------------------------
>cl /c printImplicit.cpp
>link printImplicit.obj hello.lib
>printImplicit.exe
Hello, C++ Programmers!

>dumpbin /dependents printimplicit.exe
hello.dll
KERNEL32.dll

--------------------------------------------------------
# section #1 .text
> dumpbin /rawdata section:.text hello.dll		// see hex of the data insiade each section
> dumpbin /disasm /section:.text hello.dll 		// show disambles
# section #2 .rdata
> dumpbin /rawdata /section:.rdata hello.dll	// show rawbytes
# Export Directory
> dumpbin /exports hello.dll					// show function/addrOf RVA, what function has exported, then call getprocaddr to 
# shows dependency
> dumpbin /dependents printhello.exe
# shows imports
> dumpin /imports printhello.exe
---------------------------------------------------------


New example:
#numbers.cpp
extern "C" int getOne() { return 1; }
extern "C" int getTwo() { return 2; }
extern "C" int getThree() { return 3; }


> link number.cpp
	/dll
	/noentry
	/export:getOne
	/export:getTwo
	/export:getThree
	
>dumpbin /exports numbers.dll
ordinal RVA			name			// RVA: Relative Virtual Address
	1	00001000	getOne
	2	00001020	getThree
	3	00001010	getTwo

>dumpbin /exports numbers.lib
ordinal				name
					getOne
					getThree
					getTwo


# rename is availiable
> link number.cpp
	/dll
	/noentry
	/export:getOne
	/export:getTwo
	/export:getOnePlusTwo=getThree

>dumpbin /exports numbers.dll
ordinal RVA			name			// RVA: Relative Virtual Address
	1	00001000	getOne
	2	00001020	getOnePlusTwo
	3	00001010	getTwo

>dumpbin /exports numbers.lib
ordinal				name
					getOne
					getOnePlusTwo
					getTwo


# .def file for many managing large exporting name
# numbers.def
LIBRARY numbers
exports
	getOne
	getTwo
	getOnePlusTwo=getThree
	
>link numbers.obj
	/dll
	/noentry
	/def:number.def


# specify export in the code with _declspec(dllexport) instead of __cdecl
extern "C" __declspec(dllexport) int getOne()  { return 1; }
extern "C" __declspec(dllexport) int getTwo()  { return 2; }
extern "C" __declspec(dllexport) int getThree() { return 3; }
	
> link numbers.obj				// tell linker nothing about what to export
	/dll
	/noentry
	
>dumpbin /exports numbers.dll
ordinal RVA			name
	1	00001000	getOne
	2	00001020	getOnePlusTwo
	3	00001010	getTwo

>dumpbin /directives numbers.obj			// compiler inject them into obj file
Linker Directives
-----------------
/EXPORT:getOne
/EXPORT:getTwo
/EXPORT getTHREE


# same thing but in C++ specific with '#pragma comment'
# numbers.cpp

extern "C" int getOne()   { return 1; }
extern "C" int getTwo()   { return 2; }
extern "C" int getThree() { return 3; }

#pragma comment(linker, "/export:getOne")
#pragma comment(linker, "/export:getTwo")
#pragma comment(linker, "/export:getThree")




















========================================================================

C:\Program Files\Microsoft Visual Studio\2022\Community>dumpbin /headers C:\cshis.dll
Microsoft (R) COFF/PE Dumper Version 14.30.30709.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\cshis.dll

PE signature found

File Type: DLL

FILE HEADER VALUES
             14C machine (x86)
               5 number of sections
        61BB3825 time date stamp Thu Dec 16 20:59:17 2021
               0 file pointer to symbol table
               0 number of symbols
              E0 size of optional header
            2102 characteristics
                   Executable
                   32 bit word machine
                   DLL

OPTIONAL HEADER VALUES
             10B magic # (PE32)
           10.00 linker version
          29DC00 size of code
           EB200 size of initialized data
               0 size of uninitialized data
          1EAE07 entry point (101EAE07)
            1000 base of code
          29F000 base of data
        10000000 image base (10000000 to 1057AFFF)
            1000 section alignment
             200 file alignment
            5.01 operating system version
            0.00 image version
            5.01 subsystem version
               0 Win32 version
          57B000 size of image
             400 size of headers
          38FD93 checksum
               3 subsystem (Windows CUI)
             140 DLL characteristics
                   Dynamic base
                   NX compatible
          100000 size of stack reserve
            1000 size of stack commit
          100000 size of heap reserve
            1000 size of heap commit
               0 loader flags
              10 number of directories
          329F90 [     AAF] RVA [size] of Export Directory
          32691C [     190] RVA [size] of Import Directory
          524000 [    3460] RVA [size] of Resource Directory
               0 [       0] RVA [size] of Exception Directory
               0 [       0] RVA [size] of Certificates Directory
          528000 [   40F24] RVA [size] of Base Relocation Directory
               0 [       0] RVA [size] of Debug Directory
               0 [       0] RVA [size] of Architecture Directory
               0 [       0] RVA [size] of Global Pointer Directory
               0 [       0] RVA [size] of Thread Storage Directory
               0 [       0] RVA [size] of Load Configuration Directory
               0 [       0] RVA [size] of Bound Import Directory
          29F000 [     A10] RVA [size] of Import Address Table Directory
               0 [       0] RVA [size] of Delay Import Directory
               0 [       0] RVA [size] of COM Descriptor Directory
               0 [       0] RVA [size] of Reserved Directory


SECTION HEADER #1
   .text name
  29DA7F virtual size
    1000 virtual address (10001000 to 1029EA7E)
  29DC00 size of raw data
     400 file pointer to raw data (00000400 to 0029DFFF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
60000020 flags
         Code
         Execute Read

SECTION HEADER #2
  .rdata name
   8BA3F virtual size
  29F000 virtual address (1029F000 to 1032AA3E)
   8BC00 size of raw data
  29E000 file pointer to raw data (0029E000 to 00329BFF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
40000040 flags
         Initialized Data
         Read Only

SECTION HEADER #3
   .data name
  1F88A1 virtual size
  32B000 virtual address (1032B000 to 105238A0)
    9400 size of raw data
  329C00 file pointer to raw data (00329C00 to 00332FFF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
C0000040 flags
         Initialized Data
         Read Write

SECTION HEADER #4
   .rsrc name
    3460 virtual size
  524000 virtual address (10524000 to 1052745F)
    3600 size of raw data
  333000 file pointer to raw data (00333000 to 003365FF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
40000040 flags
         Initialized Data
         Read Only

SECTION HEADER #5
  .reloc name
   52BF2 virtual size
  528000 virtual address (10528000 to 1057ABF1)
   52C00 size of raw data
  336600 file pointer to raw data (00336600 to 003891FF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
42000040 flags
         Initialized Data
         Discardable
         Read Only

  Summary

      1F9000 .data
       8C000 .rdata
       53000 .reloc
        4000 .rsrc
      29E000 .text

========================================================================














