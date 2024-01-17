#pragma once

#define WIN_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>


class MemMan
{
private:
	DWORD processId = NULL;
	HANDLE processHandle = nullptr;

public:
	bool FindGame(const wchar_t* game);
	DWORD GetProcessId();
	HANDLE GetProcessHandle();
	bool CloseProcessHandle();
	
	/* Typical wrapper function you will see
	*  This will allow different types to be 
	*  Read/Write by calling it like so ->
	*  "int foo = Read<int>(memory_address);"
	*/
	template<typename T>
	T Read(DWORD address)
	{
		T buffer;

		bool rpm = ReadProcessMemory(this->processHandle, (LPCVOID)address, (LPVOID)&buffer, sizeof(T), NULL);

		return (!rpm) ? NULL : buffer;
	}

	template<typename T>
	bool Write(DWORD address, T value)
	{
		return WriteProcessMemory(this->processHandle, (LPVOID)address, (LPCVOID)&value, sizeof(T), NULL);
	}

	// Specialization for reading null-terminated strings
	template<>
	const char* Read<const char*>(DWORD address)
	{
		const size_t bufferSize = 256; // Adjust this size based on the expected maximum length of the string
		char buffer[bufferSize] = { 0 }; // Initialize buffer with zeros
		SIZE_T bytesRead;

		ReadProcessMemory(this->processHandle, (LPCVOID)address, buffer, bufferSize - 1, &bytesRead);

		// Null-terminate the string at the correct position
		buffer[bytesRead] = '\0';

		// Return a copy of the string
		return _strdup(buffer);
	
	}

	template<typename T>
	bool ReadArray(DWORD address, T* buffer, size_t count)
	{
		return ReadProcessMemory(this->processHandle, (LPCVOID)address, buffer, sizeof(T) * count, NULL);
	}
};

