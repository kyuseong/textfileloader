#include "TextFileLoader.h"
#include "text_encoding_detect.h"

#include <windows.h>
#include <stringapiset.h>

using namespace AutoIt::Common;

bool TextFileLoader::LoadFile(const wchar_t* Filename, std::wstring& Out)
{
	std::vector<unsigned char> Doc;

	bool Ret = LoadDoc(Filename, Doc);

	Ret = Ret && ConvertTxt(Doc, Out);

	return Ret;
}

bool TextFileLoader::ConvertTxt(std::vector<unsigned char> Buff, std::wstring& Output)
{
	TextEncodingDetect detect;
	TextEncodingDetect::Encoding  Encoding = detect.DetectEncoding(&Buff[0], Buff.size());
	switch (Encoding)
	{
	case TextEncodingDetect::None:
		return false;
	case TextEncodingDetect::ANSI:
	case TextEncodingDetect::ASCII:
	{
		size_t len = MultiByteToWideChar(CP_ACP, 0, (char*)&Buff[0], Buff.size(), 0, 0);
		Output.resize(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, (char*)&Buff[0], Buff.size(), &Output[0], len);
	}
	break;
	case TextEncodingDetect::UTF8_BOM:
	case TextEncodingDetect::UTF8_NOBOM:
	{
		unsigned char* Buf = &Buff[0];
		size_t Size = Buff.size();

		if (Encoding == TextEncodingDetect::UTF8_BOM)
		{
			Buf = Buf + 3;
			Size = Size - 3;
		}

		size_t len = MultiByteToWideChar(CP_UTF8, 0, (char*)Buf, Size, 0, 0);
		Output.resize(len, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, (char*)Buf, Size, &Output[0], len);

	}
	break;
	case TextEncodingDetect::UTF16_LE_BOM:
	case TextEncodingDetect::UTF16_LE_NOBOM:
	{
		const unsigned char* Buf = &Buff[0];
		size_t Size = Buff.size();

		if (Encoding == TextEncodingDetect::UTF16_LE_BOM)
		{
			Buf = Buf + 2;
			Size = Size - 2;
		}

		wchar_t* wdoc = (wchar_t*)Buf;
		size_t length = Size / sizeof(wchar_t);
		Output.assign(wdoc, length);
	}
	break;
	case TextEncodingDetect::UTF16_BE_BOM:
	case TextEncodingDetect::UTF16_BE_NOBOM:
	{
		unsigned char* Buf = (unsigned char*)&Buff[0];
		size_t Size = Buff.size();

		if (Encoding == TextEncodingDetect::UTF16_BE_BOM)
		{
			Buf = Buf + 2;
			Size = Size - 2;
		}

		wchar_t* wdoc = (wchar_t*)Buf;
		size_t length = Size / sizeof(wchar_t);
		for (size_t i = 0; i < length; i++)
		{
			wchar_t tchr = wdoc[i];
			char *p1 = reinterpret_cast<char*>(wdoc + i);
			char *p2 = reinterpret_cast<char*>(&tchr);
			p1[0] = p2[1];
			p1[1] = p2[0];
		}
		Output.assign(wdoc, length);
	}

	break;
	default:
		return false;
		break;
	}

	return true;
}

bool TextFileLoader::LoadDoc(const wchar_t* filename, std::vector<unsigned char>& out)
{
	FILE* pf = nullptr;
	errno_t no = _wfopen_s(&pf, filename, L"rb");
	if (no != 0 || pf == nullptr)
	{
		return false;
	}

	fseek(pf, 0, SEEK_END);
	size_t size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	out.resize(size);

	// char* data = new char[size + 1];
	fread(&out[0], sizeof(unsigned char), size, pf);

	fclose(pf);

	return true;
}

