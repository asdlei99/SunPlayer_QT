﻿#include "SunFun.h"
using std::string;
using std::wstring;


string GetAppPathA()
{
	char szFilePath[_MAX_PATH] = {0}, szDrive[_MAX_DRIVE] = {0}, szDir[_MAX_DIR] = {0}, szFileName[_MAX_FNAME] = {0}, szExt[_MAX_EXT] = {0};
	GetModuleFileNameA(NULL, szFilePath, ARRAYSIZE(szFilePath));
	_splitpath_s(szFilePath,szDrive,szDir,szFileName,szExt);

	string str(szDrive);
	str.append(szDir);
	return str;
}

wstring GetAppPathW()
{
	wchar_t wszFilePath[_MAX_PATH] = {0}, wszDrive[_MAX_DRIVE] = {0}, wszDir[_MAX_DIR] = {0}, wszFileName[_MAX_FNAME] = {0}, wszExt[_MAX_EXT] = {0};
	GetModuleFileNameW(NULL, wszFilePath, ARRAYSIZE(wszFilePath));
	_wsplitpath_s(wszFilePath,wszDrive,wszDir,wszFileName,wszExt);

	wstring str(wszDrive);
	str.append(wszDir);
	return str;
}

wstring CHAR2wstring(LPCSTR lpcszSrc)
{
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszSrc,  - 1, NULL, 0);
	wchar_t *pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) *sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszSrc,  - 1, (LPWSTR)pUnicode, unicodeLen);
	wstring wString = (wchar_t *)pUnicode;
	delete [] pUnicode;
	return wString;
}

string WCHAR2string(LPCWSTR lpwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, lpwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) 
		return std::string("");
	char* lpszDst = new char[nLen];
	if (NULL == lpszDst) 
		return std::string("");
	WideCharToMultiByte(CP_ACP, 0, lpwszSrc, -1, lpszDst, nLen, NULL, NULL);
	lpszDst[nLen - 1] = 0;
	std::string strTemp(lpszDst);
	delete [] lpszDst;
	return strTemp;
}

wstring UTF8ToWString(const char *lpcszString)
{
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, lpcszString,  - 1, NULL, 0);
    wchar_t *pUnicode = new wchar_t[unicodeLen + 1];
	memset((void *)pUnicode, 0, (unicodeLen + 1) *sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, lpcszString,  - 1, (LPWSTR)pUnicode, unicodeLen);
	wstring wstrReturn(pUnicode);
	delete [] pUnicode;
	return wstrReturn;
}

string WStringToUTF8(const wchar_t *lpwcszWString)
{
	char *pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString,  - 1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void *)pElementText, 0, (iTextLen + 1) *sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString,  - 1, pElementText, iTextLen, NULL, NULL);
	string strReturn(pElementText);
	delete [] pElementText;
	return strReturn;
}

string GBKToUTF8(const string &strGBK)
{
    string strOutUTF8 = "";
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, NULL, 0);
    wchar_t *str1 = new wchar_t[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
    delete [] str1;
    delete [] str2;
	return strOutUTF8;
}

string UTF8ToGBK(const string &strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(),  - 1, NULL, 0);
    wchar_t *wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, (len+1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(),  - 1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	string strTemp(szGBK);
    delete [] szGBK;
    delete [] wszGBK;
	return strTemp;
}

string FormatString(const char *lpcszFormat,...)
{
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		strResult.assign(nLength,'\0');
		_vsnprintf_s(const_cast<char*>(strResult.c_str()), strResult.size(), nLength, lpcszFormat, marker);
		va_end(marker); //重置变量参数
	}
	return strResult;
}


wstring FormatWstring(const wchar_t *lpcwszFormat,...)
{
	wstring strResult;
	if (NULL != lpcwszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcwszFormat); //初始化变量参数
		size_t nLength = _vscwprintf(lpcwszFormat, marker) + 1; //获取格式化字符串长度
		strResult.assign(nLength,L'\0');
		_vsnwprintf_s(const_cast<wchar_t*>(strResult.c_str()), strResult.size(), nLength, lpcwszFormat, marker);
		va_end(marker); //重置变量参数
	}
	return strResult;
}

string &replace(string &str, const string &old_value, const string &new_value)
{
	for(string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}
