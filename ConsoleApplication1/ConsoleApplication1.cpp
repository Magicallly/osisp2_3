// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include <conio.h>
#include <fstream>
#include <iostream>


// function name for dinamic import
#define FUNCTION_NAME "_PrintStr@4"

#define DLL_IMPORT __declspec(dllimport)


//static
//typedef void _stdcall TChangeString(const char *indata,
//	const char *replacement);

typedef void _stdcall TChangeString(const char* path_indata,
	const char* path_replacement);


//dinamic
typedef void _stdcall TPrintStr(const char* str);

typedef HMODULE(WINAPI *LPLoadLibrary)(LPCSTR);
typedef HMODULE(WINAPI *LPGetProcAddress)(HMODULE, LPCSTR);


int len(char* arr);
int setPrivilege(HANDLE hCurrentProc, LPCTSTR name, BOOL fEnable);
int getDllLocation();
TPrintStr* dynamicImport();
TChangeString* staticImport();
int integrateDLL(const char* proc_name,
	const char* DLL_location);
int exitFunc(int error);
std::vector<DWORD> GetProcList(const char* searchProc);
int chooseCommand();
int staticFunction();
int dynamicFunction();
int injectionFunction();
void prepareMemory();
int solveCommand(int command);
void GetRemoteThread(DWORD procID, 
	const char* dll_path);
int writeToFile(const char* name, char* data);


//statuc
extern "C" DLL_IMPORT void _stdcall ChangeString(const char* path_indata, 
	const char* path_replacement);

//dinamic
extern "C" DLL_IMPORT void _stdcall TPrintString();


int command;
char proc_name[500];
char pathToDLL[500];
char indata[5000];
char replacement[5000];
const char* path_indata = "D:\\indata.txt";
const char* path_replacement = "D:\\replacement.txt";
HMODULE hDll = NULL;


int main(int argc, char* argv[])
{
	prepareMemory();

	if (!setPrivilege(GetCurrentProcess(), SE_DEBUG_NAME, TRUE))
	{
		printf("ошибка установки привилегий");
		return 1;
	}

	if (!chooseCommand())
	{
		printf("неверно выбрана команда");
		return 1;
	}
	
	return exitFunc(solveCommand(command));
}

void prepareMemory()
{
	ZeroMemory(proc_name, len(proc_name)); // memset by zeros
	ZeroMemory(pathToDLL, len(pathToDLL));
}


int solveCommand(int command)
{
	switch (command)
	{
	case 0:
		return staticFunction();
	case 1:
		return dynamicFunction();
	case 2:
		return injectionFunction();
	}
	//return -1;
}

int staticFunction()
{
	TChangeString* func_target = NULL;
	func_target = staticImport();
	if (func_target != NULL)
	{
		printf("find: ");
		fflush(stdin);
		scanf("%s", indata);
		printf("replace: ");
		fflush(stdin);
		scanf("%s", replacement);

		if ((writeToFile(path_indata, indata) == -1)
			|| (writeToFile(path_replacement, replacement) == -1))
			return 1;

		func_target(path_indata, path_replacement);
		
	}
	else
	{
		printf("%s DLL not found", pathToDLL);
		return 1;
	}
	return 0;
}


int writeToFile(const char* name, char* data)
{
	std::ofstream file;
	file.open(name);
	if (!file.is_open())
		return -1;
	file.write(data, strlen(data));
	file.close();
}


int dynamicFunction()
{
	TPrintStr* func_target_print = NULL;
	func_target_print = dynamicImport();
	char name[100];
	if (func_target_print != NULL)
	{
		printf("name: ");
		fflush(stdin);
		scanf("%s", name);
		func_target_print(name);
	}
	else
	{
		printf("%s DLL not found", pathToDLL);
		return 1;
	}
	return 0;
}

int injectionFunction()
{
	printf("injected funcname: ");
	char func_name[50];
	scanf("%s", func_name);
	printf("DLL location: ");
	char dll_location[50];
	scanf("%s", dll_location);

	if (integrateDLL(func_name, dll_location))
	{
		printf("integrate problem");
		return 1;
	}
	return 0;
}

int len(char* arr) 
{
	if (arr == NULL) { return 0; }
	return sizeof(arr) / sizeof(arr[0]);
}



//If the caller has enabled the SeDebugPrivilege privilege, the requested access is granted regardless of the contents of the security descriptor.
// OpenProcess
int setPrivilege(HANDLE hCurrentProc, LPCTSTR name, BOOL fEnable)
{
	HANDLE hToken;
	// TOKEN_QUERY = Required to query an access token.
	// TOKEN_ADJUST_PRIVILEGES = Required to enable or disable the privileges in an access token.
	if (!OpenProcessToken(hCurrentProc, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		&hToken))
	{
		return 0;
	}
	int error = GetLastError();
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	// 1) If a null string is specified, the function 
	//attempts to find the privilege name on the local system
	// 2) specifies the name of the privilege 
	// DEBUG = Required to debug and adjust 
	// the memory of a process owned by another account
	int e = LookupPrivilegeValue(NULL, name, &tp.Privileges[0].Luid);
	tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
	
	
	// 1) access token that contains the privileges to be modifieed
	// 2) If it is FALSE, the function modifies privileges based 
	// on the information pointed to by the NewState parameter
	// 3) array of privileges and their attributes
	// 4) size of array
	// 5) contains the previous state of any privileges that the function modifies
	// A pointer to a variable that receives the required size, in bytes, of the buffer pointed to by the PreviousState parameter. 
	//This parameter can be NULL if PreviousState is NULL
	e = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
	if (e) { return e; }
	error = (GetLastError() == ERROR_SUCCESS);
	return(error);
}

int chooseCommand()
{
	printf("Select command (static import=0, dynamic import=1, remote thread injection=2): ");
	scanf_s("%d", &command);
	switch (command)
	{
	case 0:
	case 1:
	case 2:
		return 1;
	default:
		return 0;
	}
}

void GetRemoteThread(DWORD procID, const char* dll_path)
{

	
	HMODULE hdll = LoadLibraryA("Kernel32.dll");

	LPLoadLibrary LoadLibraryA = (LPLoadLibrary)GetProcAddress(hdll, "LoadLibraryA");
	//LPGetProcAddress GetProcAddressA = (LPGetProcAddress)GetProcAddress(hdll, "GetProcAddress");

	// 1) the access to the process object.
	// 2) If this value is TRUE, processes created by this process 
	//will inherit the handle. Otherwise, the processes do 
	//not inherit this handle.
	HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE |
		PROCESS_CREATE_THREAD | PROCESS_CREATE_PROCESS,
		FALSE, procID);

	// 2)The pointer that specifies a desired starting address
	//for the region of pages that you want to allocate. 
	//NULL = the function determines where to allocate the region.
	// 3) The size of the region of memory to allocate, in bytes.
	// 4) The type of memory allocation
	// 5) MEM_RESERVE = Reserves a range of the process's virtual address space 
	//without allocating any actual physical storage in memory
	//or in the paging file on disk.
	// MEM_COMMIT = Allocates memory charges (from the overall size of memory
	//and the paging files on disk) for the specified reserved memory pages.
	// 6) The memory protection for the region of pages to be allocated
	// PAGE_READWRITE = Enables read-only or read/write 
	// access to the committed region of pages
	LPVOID path = VirtualAllocEx(hProc, NULL, strlen(dll_path) + 1,
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);


	// last parameter = A pointer to a variable 
	// that receives the number of bytes 
	int er = WriteProcessMemory(hProc, path, dll_path,
		strlen(dll_path) + 1, NULL);

	DWORD threadID;

	// 2) A pointer to a SECURITY_ATTRIBUTES
	// NULL = the thread gets a default security descriptor 
	// 3) initial size of the stack(bytes)
	// 0 = the new thread uses the default size
	// 4) A pointer to the application-defined 
	// function of type LPTHREAD_START_ROUTINE to be executed  
	// 5) A pointer to a variable to be passed to the thread function.
	// 6) The flags that control the creation of the thread.
	// 0 = The thread runs immediately after creation.
	HANDLE HThread = CreateRemoteThread(hProc, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)path
		, NULL, &threadID);

	if (HThread == NULL)
	{
		printf("error\n");
	}
	FreeLibrary(hDll);
	CloseHandle(hProc); 

}

int integrateDLL(const char* proc_name,
	const char* DLL_location)
{
	std::vector<DWORD> targetID = GetProcList(proc_name);
	if (targetID.size() == 0)
	{
		targetID.clear();
		return -1;
	}
	for (int i = 0; i < targetID.size(); i++)
	{
		GetRemoteThread(targetID[i], DLL_location);
	}
	targetID.clear();
	return 0;
}



WCHAR* to_wchar(const char* str)
{
	int nChars = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	WCHAR* converted = new WCHAR[nChars];
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)converted, nChars);
	return converted;
}

std::vector<DWORD> GetProcList(const char* searchProc)
{
	std::vector<DWORD> outdata;
	// Takes a snapshot of the specified processes, as well as the 
	// heaps, modules, and threads used by these processes
	// 1) The portions of the system to be included in the snapshot. 
	// TH32CS_SNAPPROCESS = Includes all processes in the system in the snapshot.
    // 2) The process identifier of the process to be included in the snapshot. 
	// This parameter can be zero to indicate the current process
	HANDLE handler = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(handler, &ProcEntry);
	do
	{
		// wcscmp = cmp of WCHAR
		if (wcscmp(ProcEntry.szExeFile, to_wchar(searchProc)) == 0)
			outdata.push_back(ProcEntry.th32ProcessID);
	} while (Process32Next(handler, &ProcEntry));
	CloseHandle(handler);
	return outdata;
}

TPrintStr* dynamicImport()
{
	getDllLocation();
	hDll = LoadLibraryA(pathToDLL);
	if (hDll == NULL)
		return NULL;
	TPrintStr* func = (TPrintStr*)GetProcAddress(hDll, FUNCTION_NAME);
	int error = GetLastError();
	return func;
}
 
int getDllLocation()
{
	printf("enter DLL location: ");
	scanf("%s", pathToDLL);
	printf("\n");
	return 0;
}

TChangeString* staticImport()
{
	return (TChangeString*)ChangeString;
}

int exitFunc(int err)
{
	if (hDll != NULL)
	{
		if (!FreeLibrary(hDll))
		{
			printf("error with memory");
		}
	}
	_getch();
	return err;
}
