#pragma once
#include <string>
#include <atlstr.h>
#include <sstream>

class MyString
{
public:
	MyString(void);
	~MyString(void);

	static CString TCHAR2CString(TCHAR *tchar) {
		return (LPCTSTR)tchar;
	}

	static TCHAR* CString2TCHAR(CString cstr) {
		return (TCHAR*)(LPCTSTR)cstr;
	}
	
	static std::string CString2string(CString cstr) {
		CT2CA pszConvertedAnsiString(cstr);
		std::string s(pszConvertedAnsiString);
		return s;
	}

	static std::wstring CString2wstring(CString cstr) {
		return LPCTSTR(cstr);
	}

	static LPWSTR CString2LPWSTR(CString cstr) {
		return (LPWSTR)(LPCWSTR)cstr;
	}

	static int CString2int(CString cstr) {
		return _ttoi(cstr);
	}

	static CString string2CString(std::string str) {
		CString cstr(str.c_str());
		return cstr;
	}

	static CString wstring2CString(std::wstring wstr) {
		CString cstr(wstr.c_str());
		return cstr;
	}

	static std::string wstring2string(std::wstring wstr) {
		CString cstr = wstr.c_str();
		return CString2string(cstr);
	}	

	static std::wstring string2wstring(std::string str) {
		std::wstring wstr(str.length(),L' ');
		copy(str.begin(),str.end(),wstr.begin());
		return wstr;
	}

	static CString long2CString(long num) {
		if (num > 0) {
			CString cstr;
			cstr.Format(_T("%ld"), num);
			return cstr;
		}
		else if (num == 0) {
			CString cstr = _T("0");
			return cstr;
		}
		else {
			num = num * (-1);
			CString cstr, cstr2;
			cstr.Format(_T("%ld"), num);
			cstr2 = _T("-") + cstr;
			return cstr2;
		}
	}

	static CString double2CString(double num) {
		if (num > 0) {
			CString cstr;
			cstr.Format(_T("%.2lf"), num);
			return cstr;
		}
		else if (num == 0) {
			CString cstr = _T("0");
			return cstr;
		}
		else {
			num = num * (-1);
			CString cstr, cstr2;
			cstr.Format(_T("%.2lf"), num);
			cstr2 = _T("-") + cstr;
			return cstr2;
		}
	}

	static CString DWORD2CString(DWORD d) {
		if (d == NULL) {
			return _T("");
		} else {
			CString cstr;
			cstr.Format(_T("%lu"), (ULONG)d);
			return cstr;
		}
	}

	static std::wstring int2wstring(int num) {
		CString cstr;
		cstr.Format(_T("%d"), num);
		return CString2wstring(cstr);
	}

	static int wstring2int(wstring wstr) {
		std::wistringstream wstrm(wstr);
		int num;
		wstrm >> num;
		cout << num << endl;

		return num;
	}

	static string int2string(int num) {
		stringstream sstr;
		sstr << num ;
		return sstr.str();
	}

	static int string2int(string str) {
		return atoi(str.c_str());
	}

	static string replaceAll(const string &str, const string &pattern, const string &replace)
	{
		string result = str;
		string::size_type pos = 0;
		string::size_type offset = 0;

		while((pos = result.find(pattern, offset)) != string::npos)
		{
			result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
			offset = pos + replace.size();
		}

		return result;
	}
};