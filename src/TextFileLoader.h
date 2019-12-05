#pragma once

#include <string>
#include <vector>

class TextFileLoader
{
public:
	bool LoadFile(const wchar_t* filename, std::wstring& out);

	bool ConvertTxt(std::vector<unsigned char> Buff, std::wstring& Output);

	bool LoadDoc(const wchar_t* filename, std::vector<unsigned char>& out);


};
