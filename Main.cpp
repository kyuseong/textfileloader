// textfileloader.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include "TextFileLoader.h"

int main()
{
	setlocale(LC_ALL, "korean");

	TextFileLoader loader;

	{
		std::wstring str;
		loader.LoadFile(L"sample/ansi.txt", str);
		std::wcout << L"[ANSI]\n";
		std::wcout << str;
	}

	{
		std::wstring str;
		loader.LoadFile(L"sample/utf8.txt", str);
		std::wcout << L"\n[UTF8]\n";
		std::wcout << str;
	}
	{
		std::wstring str;
		loader.LoadFile(L"sample/utf8_bom.txt", str);
		std::wcout << L"\n[UTF8_BOM]\n";
		std::wcout << str;
	}
	{
		std::wstring str;
		loader.LoadFile(L"sample/utf16be.txt", str);
		std::wcout << L"\n[UTF16 Big]\n";
		std::wcout << str;
	}
	{
		std::wstring str;
		loader.LoadFile(L"sample/utf16le.txt", str);
		std::wcout << L"[UTF16 Little]\n";
		std::wcout << str;
	}
}
