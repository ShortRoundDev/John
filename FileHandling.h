#pragma once

#include <string>
#include <Windows.h>

std::wstring getFilePath();
std::wstring getOpenFilePath();

_Success_(return)
bool openBinaryFile(std::wstring path, _Out_ uint8_t** data, _Out_ DWORD* dataSize);