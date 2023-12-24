#include "FileHandling.h"

#include <windows.h>
#include <shobjidl.h> 
#include <iostream>

std::wstring getFilePath()
{
	std::wstring path = L"";
	IFileOpenDialog* fileOpen;

	HRESULT hr = CoCreateInstance(
		CLSID_FileSaveDialog,
		NULL,
		CLSCTX_ALL,
		IID_IFileSaveDialog,
		reinterpret_cast<void**>(&fileOpen)
	);

	if (SUCCEEDED(hr))
	{
		hr = fileOpen->Show(NULL);
		if (SUCCEEDED(hr))
		{
			IShellItem* item;
			hr = fileOpen->GetResult(&item);
			if (SUCCEEDED(hr))
			{
				PWSTR filePath;
				hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
				if (SUCCEEDED(hr))
				{
					path = std::wstring(filePath);
				}
				item->Release();
			}
		}
		fileOpen->Release();

	}
	return path;
}

std::wstring getOpenFilePath()
{

	IFileOpenDialog* fileOpen;
	std::wstring path;
	
	HRESULT hr = CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_ALL,
		IID_IFileOpenDialog,
		reinterpret_cast<void**>(&fileOpen)
	);

	if (SUCCEEDED(hr))
	{
		hr = fileOpen->Show(NULL);
		if (SUCCEEDED(hr))
		{
			IShellItem* item;
			hr = fileOpen->GetResult(&item);
			if (SUCCEEDED(hr))
			{
				PWSTR filePath;
				hr = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
				if (SUCCEEDED(hr))
				{
					path = std::wstring(filePath);
				}
				item->Release();
			}
		}
		fileOpen->Release();
	}

	return path;
}

_Success_(return)
bool openBinaryFile(std::wstring path, _Out_ uint8_t** data, _Out_ DWORD* dataSize)
{
	if (data == NULL || dataSize == NULL) {
		return false;
	}
	HANDLE file = CreateFile(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (file == INVALID_HANDLE_VALUE) {
		return false;
	}

	*dataSize = GetFileSize(file, NULL);
	
	*data = (uint8_t*)calloc(*dataSize, sizeof(uint8_t));
	if (!ReadFile(file, *data, *dataSize, NULL, NULL)) {
		CloseHandle(file);
		free(*data);
		*data = NULL;
		return false;
	}
	CloseHandle(file);

	return true;
}