// myAttack.cpp

#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
//#include "user32.h"
#define EXPORTING_DLL
// #include "myAttack.h"

#define LOG_FILE L"C:\\Users\\IEUser\\SRML\\myAttack\\test\\Logs\\"

using namespace std;
int globval = 5;

int test_ptr = 0x004010B9;
int weight_Ptr = 0x0012FE80;

int globcount = 0;

// citation www.cplusplus.com
std::vector<const void*> scan_memory( void* address_low, std::size_t nbytes,
                                      const std::vector<BYTE>& bytes_to_find )
{
    std::vector<const void*> addresses_found ;

    // all readable pages: adjust this as required
    const DWORD pmask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE |
        PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY ;

    MEMORY_BASIC_INFORMATION mbi;

    BYTE* address = static_cast<BYTE*>( address_low ) ;
    BYTE* address_high = address + nbytes ;

    while( address < address_high && ::VirtualQuery( address, &mbi, sizeof(mbi) ) )
    {
        // committed memory, readable, wont raise exception guard page
        // if( (mbi.State==MEM_COMMIT) && (mbi.Protect|pmask) && !(mbi.Protect&PAGE_GUARD) )
        if( (mbi.State==MEM_COMMIT) && (mbi.Protect&pmask) && !(mbi.Protect&PAGE_GUARD) )
        {
            const BYTE* begin = static_cast<const BYTE*>(mbi.BaseAddress) ;
            const BYTE* end =  begin + mbi.RegionSize ;

            const BYTE* found = std::search( begin, end, bytes_to_find.begin(), bytes_to_find.end() ) ;
            while( found != end )
            {
                addresses_found.push_back( found ) ;
                found = std::search( found+1, end, bytes_to_find.begin(), bytes_to_find.end() ) ;
            }
        }

        address += mbi.RegionSize ;
        mbi = MEMORY_BASIC_INFORMATION();
    }

    return addresses_found ;
}

/*std::vector<const void*> scan_memory( std::string module_name, const std::vector<BYTE>& bytes_to_find )
{
    auto base = GetModuleHandleA( module_name.c_str() ) ;
    if( base == nullptr ) return {} ;

    MODULEINFO minfo {} ;
    ::GetModuleInformation( GetCurrentProcess(), base, std::addressof( minfo ), sizeof( minfo ) ) ;
    return scan_memory( base, minfo.SizeOfImage, bytes_to_find ) ;
}*/


union pointerChar {
	int ptr;
	unsigned char c[4];
};


void WriteLog(char *text) {
	/*char snum[50];
	itoa(globcount, snum, 10);
	wstring mynum((const char *)snum);*/
	
	stringstream shootMe;
	shootMe << globcount;
	string mynum = shootMe.str();// ToString( (unsigned long)globcount);
	wstring logNum(mynum.length(), L' ');
	copy(mynum.begin(), mynum.end(), logNum.begin());
	wstring filename = L"Log";
	filename += logNum;
	filename += L".txt";
	globcount++;
	
	wstring path = LOG_FILE;
	path += filename;
	
	HANDLE hfile = CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD written;
	WriteFile(hfile, text, strlen(text), &written, NULL);
	WriteFile(hfile, "\r\n", 2, &written, NULL);
	CloseHandle(hfile);
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	// std::cout < "at least it loaded\n";
	// char b[2], c[2]; //= *test_ptr;
	// b[1] = '\0';
	// c[1] = '\0';
	// b[0] = *((char *)test_ptr);
	// *((char *) test_ptr) = 'b'; This will crash the NN
	// string myStr = b;
	// WriteLog(b);
	// c[0] = ''
	// globval = 7;
	// *((char *) weight_Ptr) = 'a';
	
	// pointerChar myPointerChar;
	// char myCStr[10];
	char shortChr[8];
	int length = 50;
	
	shortChr[7] = '\0';
	// shortChr[1] = '\n';
	
	// weightPtrs[4] = '\0';
	
	/*myPointerChar.c[0] = *((char *) weight_Ptr);
	myPointerChar.c[1] = *(((char *) weight_Ptr) + 1);
	myPointerChar.c[2] = *(((char *) weight_Ptr) + 2);
	myPointerChar.c[3] = *(((char *) weight_Ptr) + 3);
	myPointerChar.c[4] = '\0';*/
	
	/*for (int i = 0; i < length; i ++) {
		char val = *(((char *) weight_Ptr) + i);
		itoa((unsigned int) val, shortChr, 16);
		
		WriteLog(shortChr);
	}*/ 
	
	// int newPtr = myPointerChar.ptr;
	
	// sprintf(myCStr, "%d", newPtr);
	
	
	
	/*if (*((char *) newPtr) == '\0' )
		WriteLog("Good Sign!!");
	else
		WriteLog("Not So Good");*/
	
	
	
	
	
   return TRUE;
}

extern "C" __declspec(dllexport) void HelloWorld()
{
   /*MessageBox( NULL, TEXT("Hello World"), 
   TEXT("In a DLL"), MB_OK);*/
   std::cout << "ran motherfucker!!\n";
};

extern "C" __declspec(dllexport) int Test()
{
   /*MessageBox( NULL, TEXT("Hello World"), 
   TEXT("In a DLL"), MB_OK);*/
   // std::cout << "ran motherfucker!!\n";
   return globval;
};