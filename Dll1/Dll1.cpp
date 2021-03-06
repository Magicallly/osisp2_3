// Dll1.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

char string_1[4200] = { "0000wddddddddddddddddddddddddddddddddkekololrkekekdddddddddddkekololrkekekdddddddddddkekololrkekekddddkekololrkekekdddddddddddkekololrkekekekkekekekkekekkekekgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjv00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101101000010100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010101100000000000000000010110100001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101101000010100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010110000000000000000001011010000101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010110100001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101011000000000000000000101101000010100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001011010000101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010101100000000000000000010110100001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101101000010100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010110000000000000000001011010000101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010110100001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101011000000000000000000101101000010100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001011010000101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010110000000000000000001011010000101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010110100001010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" };
char string_2[4200] = { "0000wdddddddddddddddddddddddddkekololrkekekekkekekekkekekkekekgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjviefffffffffffffffffffffff" };
//char hello_string[50000] = { "wdddddddddddddddddddddddddkekololrgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjviefffffffffffffffffffffffffffffffffffffffffffffffffffffffffgewgsgggggggggggggggsegsgwdddddddddddddddddddddddddkekololrgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjviefffffffffffffffffffffffffffffffffffffffffffffffffffffffffgewgsgggggggggggggggsegsgwdddddddddddddddddddddddddkekololrgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjviefffffffffffffffffffffffffffffffffffffffffffffffffffffffffgewgsgggggggggggggggsegsgwdddddddddddddddddddddddddkekololrgrgrkjgkrjgkrjkjgkrjgvkrjihirbhrbjviefffffffffffffffffffffffffffffffffffffffffffffffffffffffffgewgsgggggggggggggggsegsg" };
#define DLL_EXPORT __declspec(dllexport)

std::vector<int> find_string(char* buffer, const char* indata, int sizeofbuff);
int checkEndOfPage(char* buff_1, char* buff_2, char* indata, int pageSize);
int get_indata(const char* path_indata);
int get_replacement(const char* path_replacement);

char indata[5000];
char replacement[5000];
std::vector<int> str_indexes;
int str_index_between_pages;



extern "C" void DLL_EXPORT _stdcall ChangeString(const char* path_indata, const char* path_replacement)
{
	//HANDLE handle = GetCurrentProcess();
	//HANDLE handle = OpenProcess(TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
	//	FALSE, GetCurrentProcessId());

	HANDLE handle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		FALSE, GetCurrentProcessId());
	SIZE_T nb; int error;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	//precessor level = Family, 
	//wpRocessorRevision(High) = model
	//wpRocessorRevision(Low) = stepping
	//milkyway.cs.rpi.edu/milkyway/cpu_list.php

	char** buffer = new char*[2];
	buffer[0] = new char[info.dwPageSize];
	buffer[1] = new char[info.dwPageSize];
	int i, curBuffIndex = 0;
	if (!get_indata(path_indata)) { return; }
	if (!get_replacement(path_replacement)) { return; }

	LPBYTE adr = (LPBYTE)info.lpMinimumApplicationAddress - info.dwPageSize;;
	MEMORY_BASIC_INFORMATION inf;
	inf.State = MEM_FREE;
	ZeroMemory(buffer[curBuffIndex], info.dwPageSize);
	while ((adr < (LPBYTE)info.lpMaximumApplicationAddress) && (inf.State != MEM_COMMIT))
	{
		adr += info.dwPageSize;
		VirtualQueryEx(handle, adr, &inf, sizeof(inf));
	}
	ReadProcessMemory(handle, adr, buffer[curBuffIndex], info.dwPageSize, NULL);
	curBuffIndex = ++curBuffIndex % 2;
	




	//adr = adr + info.dwPageSize;

	while (adr < (LPBYTE)info.lpMaximumApplicationAddress)
	{
		VirtualQueryEx(handle, adr, &inf, sizeof(inf));
		ZeroMemory(buffer[curBuffIndex], info.dwPageSize);
		if (inf.State == MEM_COMMIT)
		{
			ReadProcessMemory(handle, adr, buffer[curBuffIndex], info.dwPageSize, NULL);
			error = GetLastError();
			str_indexes = find_string(buffer[curBuffIndex], indata, info.dwPageSize);


			// need to write string between 2 pages
			if (!get_replacement(path_replacement)) { return; }
			if (checkEndOfPage(buffer[curBuffIndex], buffer[(curBuffIndex + 1) % 2], indata, info.dwPageSize))
			{
				int str_index_ = str_index_between_pages;
				for (i = 0; (i < strlen(replacement)+1) && (str_index_ < info.dwPageSize); i++) {
					buffer[curBuffIndex][str_index_++] = replacement[i];
				}
				for (int j = 0; i < strlen(replacement)+1; i++, j++) {
					buffer[(curBuffIndex + 1) % 2][j] = replacement[i];
				}

				VirtualQueryEx(handle, adr, &inf, sizeof(inf));
				if (PAGE_READWRITE != inf.AllocationProtect)
				{
					if (!WriteProcessMemory(handle, adr, buffer[curBuffIndex], info.dwPageSize, NULL))
						error = GetLastError();

				}

				VirtualQueryEx(handle, adr + info.dwPageSize, &inf, sizeof(inf));
				if (PAGE_READWRITE != inf.AllocationProtect)
				{
					if (!WriteProcessMemory(handle, adr + info.dwPageSize, buffer[(curBuffIndex + 1 ) % 2], info.dwPageSize, NULL))
						error = GetLastError();

				}//buffer[curBuffIndex][str_index_between_pages + i] = '\n';
			}

			VirtualQueryEx(handle, adr, &inf, sizeof(inf));
			if (str_indexes.size() == 0)
			{
				adr += info.dwPageSize;
				str_indexes.clear();
				continue;
			}
			if (!get_replacement(path_replacement)) { return; }
			for (int j = 0; j < str_indexes.size(); j++)
			{
				
				for (i = 0; i < strlen(replacement)+1; i++) {
					buffer[curBuffIndex][str_indexes[j] + i] = replacement[i];
				}
				//buffer[curBuffIndex][str_indexes[j] + i] = '\n';
			}
			if (str_indexes.size() != 0)
			{
				if (PAGE_READWRITE == inf.AllocationProtect)
					continue;
				if (!WriteProcessMemory(handle, adr, buffer[curBuffIndex], info.dwPageSize, NULL))
					error = GetLastError();
			}
			curBuffIndex = ++curBuffIndex % 2;
			str_indexes.clear();
		}
		adr += info.dwPageSize;
	}
	printf("\n");
	printf(string_1);
	printf("\n");
	printf(string_2);
	//delete[] buffer;
	return;
}



int get_indata(const char* path_indata)
{
	std::ifstream file;
	file.open(path_indata);
	if (!file.good()) { return 0; }
	file >> indata;
	file.close();
	return 1;
}

int get_replacement(const char* path_replacement)
{
	std::ifstream file;
	file.open(path_replacement);
	if (!file.good()) { return 0; }
	file >> replacement;
	file.close();
	return 1;
}


int checkEndOfPage(char* buff_1, char* buff_2, char* indata, int pageSize)
{
	int p = pageSize - strlen(indata);
	if (p < 0)
		p = 0;
	int i;
	for (; p < pageSize; p++)
	{
		for (i = 0; i < strlen(indata), i + p < pageSize; i++)
		{

			if (buff_1[i + p] != indata[i])
				break;
		}
		if (i + p == pageSize)
		{
			for (int j = 0; (i < strlen(indata)) && (buff_2[j] != indata[i++]); j++);
			
			if (i == strlen(indata))
			{
				str_index_between_pages = p;
				return 1;
			}
		}
	}
	return 0;
}



std::vector<int> find_string(char* buffer, const char* indata, int sizeofbuff)
{
	std::vector<int> indexes;
	int j;
	char temp[50];
	int key = 0;
	for (int i = 0; i < sizeofbuff; i++)
	{
		j = 0;
		while (j < strlen(indata))
		{
			if ((i + j >= sizeofbuff) || (buffer[i+j] != indata[j]))
				break;
			j++;
			if (j == strlen(indata))
			{
				indexes.push_back(i);
				i += strlen(indata) - 1;
				continue;
			}
			
		}
	}
	return indexes;
}
