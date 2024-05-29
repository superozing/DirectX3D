#pragma once

class CPathMgr
{
private:
	static wchar_t g_szContent[255];
	static wchar_t g_szLog[255];

public:
	static void init();
	static const wchar_t* GetContentPath() { return g_szContent; }
	static const wchar_t* GetLogPath() { return g_szLog; }
	static wstring GetRelativePath(const wstring& _FullPath);
};

