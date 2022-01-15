// strconv.cpp : スタティック ライブラリ用の関数を定義します。
//

#include"strconv.h"


#if __cplusplus >= 201103L && !defined(STRCONV_CPP98)
std::wstring cp_to_wide(const std::string& s, UINT codepage)
{
    int in_length = (int)s.length();
    int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
    std::wstring result(out_length, L'\0');
    if (out_length)
        MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0], out_length);
    return result;
}
std::string wide_to_cp(const std::wstring& s, UINT codepage)
{
    int in_length = (int)s.length();
    int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
    std::string result(out_length, '\0');
    if (out_length)
        WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0], out_length, 0, 0);
    return result;
}
#else /* __cplusplus < 201103L */
std::wstring cp_to_wide(const std::string& s, UINT codepage)
{
    int in_length = (int)s.length();
    int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
    std::vector<wchar_t> buffer(out_length);
    if (out_length)
        MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &buffer[0], out_length);
    std::wstring result(buffer.begin(), buffer.end());
    return result;
}
std::string wide_to_cp(const std::wstring& s, UINT codepage)
{
    int in_length = (int)s.length();
    int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
    std::vector<char> buffer(out_length);
    if (out_length)
        WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &buffer[0], out_length, 0, 0);
    std::string result(buffer.begin(), buffer.end());
    return result;
}
#endif

std::string cp_to_utf8(const std::string& s, UINT codepage)
{
    if (codepage == CP_UTF8)
        return s;
    std::wstring wide = cp_to_wide(s, codepage);
    return wide_to_cp(wide, CP_UTF8);
}
std::string utf8_to_cp(const std::string& s, UINT codepage)
{
    if (codepage == CP_UTF8)
        return s;
    std::wstring wide = cp_to_wide(s, CP_UTF8);
    return wide_to_cp(wide, codepage);
}

std::wstring ansi_to_wide(const std::string& s)
{
    return cp_to_wide(s, CP_ACP);
}
std::string wide_to_ansi(const std::wstring& s)
{
    return wide_to_cp(s, CP_ACP);
}

std::wstring sjis_to_wide(const std::string& s)
{
    return cp_to_wide(s, 932);
}
std::string wide_to_sjis(const std::wstring& s)
{
    return wide_to_cp(s, 932);
}

std::wstring utf8_to_wide(const std::string& s)
{
    return cp_to_wide(s, CP_UTF8);
}
std::string wide_to_utf8(const std::wstring& s)
{
    return wide_to_cp(s, CP_UTF8);
}

std::string ansi_to_utf8(const std::string& s)
{
    return cp_to_utf8(s, CP_ACP);
}
std::string utf8_to_ansi(const std::string& s)
{
    return utf8_to_cp(s, CP_ACP);
}

std::string sjis_to_utf8(const std::string& s)
{
    return cp_to_utf8(s, 932);
}
std::string utf8_to_sjis(const std::string& s)
{
    return utf8_to_cp(s, 932);
}

#ifdef __cpp_char8_t
std::u8string utf8_to_char8(const std::string& s)
{
    return std::u8string(s.begin(), s.end());
}
std::string char8_to_utf8(const std::u8string& s)
{
    return std::string(s.begin(), s.end());
}

std::wstring char8_to_wide(const std::u8string& s)
{
    return cp_to_wide(char8_to_utf8(s), CP_UTF8);
}
std::u8string wide_to_char8(const std::wstring& s)
{
    return utf8_to_char8(wide_to_cp(s, CP_UTF8));
}

std::u8string cp_to_char8(const std::string& s, UINT codepage)
{
    return utf8_to_char8(cp_to_utf8(s, codepage));
}
std::string char8_to_cp(const std::u8string& s, UINT codepage)
{
    return utf8_to_cp(char8_to_utf8(s), codepage);
}

std::u8string ansi_to_char8(const std::string& s)
{
    return cp_to_char8(s, CP_ACP);
}
std::string char8_to_ansi(const std::u8string& s)
{
    return char8_to_cp(s, CP_ACP);
}

std::u8string sjis_to_char8(const std::string& s)
{
    return cp_to_char8(s, 932);
}
std::string char8_to_sjis(const std::u8string& s)
{
    return char8_to_cp(s, 932);
}
#endif
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
std::wstring vformat(const wchar_t* format, va_list args)
{
    int len = _vsnwprintf(0, 0, format, args);
    if (len < 0)
        return L"";
    std::vector<wchar_t> buffer(len + 1);
    len = _vsnwprintf(&buffer[0], len, format, args);
    if (len < 0)
        return L"";
    buffer[len] = L'\0';
    return &buffer[0];
}
std::string vformat(const char* format, va_list args)
{
    int len = _vsnprintf(0, 0, format, args);
    if (len < 0)
        return "";
    std::vector<char> buffer(len + 1);
    len = _vsnprintf(&buffer[0], len, format, args);
    if (len < 0)
        return "";
    buffer[len] = '\0';
    return &buffer[0];
}
#ifdef __cpp_char8_t
std::u8string vformat(const char8_t* format, va_list args)
{
    int len = _vsnprintf(0, 0, (const char*)format, args);
    if (len < 0)
        return u8"";
    std::vector<char> buffer(len + 1);
    len = _vsnprintf(&buffer[0], len, (const char*)format, args);
    if (len < 0)
        return u8"";
    buffer[len] = '\0';
    return (char8_t*)&buffer[0];
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
std::wstring format(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring s = vformat(format, args);
    va_end(args);
    return s;
}
std::string format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    return s;
}
#ifdef __cpp_char8_t
std::u8string format(const char8_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::u8string s = vformat(format, args);
    va_end(args);
    return s;
}
#endif

void format(std::ostream& ostrm, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring s = vformat(format, args);
    va_end(args);
    ostrm << wide_to_utf8(s) << std::flush;
}
void format(std::ostream& ostrm, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    ostrm << s << std::flush;
}
#ifdef __cpp_char8_t
void format(std::ostream& ostrm, const char8_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::u8string s = vformat(format, args);
    va_end(args);
    ostrm << char8_to_utf8(s) << std::flush;
}
#endif

std::string formatA(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring s = vformat(format, args);
    va_end(args);
    return wide_to_ansi(s);
}
std::string formatA(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    return utf8_to_ansi(s);
}
#ifdef __cpp_char8_t
std::string formatA(const char8_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::u8string s = vformat(format, args);
    va_end(args);
    return char8_to_ansi(s);
}
#endif

void formatA(std::ostream& ostrm, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring s = vformat(format, args);
    va_end(args);
    ostrm << wide_to_ansi(s) << std::flush;
}
void formatA(std::ostream& ostrm, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    ostrm << utf8_to_ansi(s) << std::flush;
}
#ifdef __cpp_char8_t
void formatA(std::ostream& ostrm, const char8_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::u8string s = vformat(format, args);
    va_end(args);
    ostrm << char8_to_ansi(s) << std::flush;
}
#endif

void dbgmsg(const wchar_t* title, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::wstring s = vformat(format, args);
    va_end(args);
    MessageBoxW(0, s.c_str(), title, MB_OK);
}
void dbgmsg(const char* title, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    MessageBoxW(0, utf8_to_wide(s).c_str(), utf8_to_wide(title).c_str(), MB_OK);
}
#ifdef __cpp_char8_t
void dbgmsg(const char8_t* title, const char8_t* format, ...)
{
    va_list args;
    va_start(args, format);
    std::u8string s = vformat(format, args);
    va_end(args);
    MessageBoxW(0, char8_to_wide(s).c_str(), char8_to_wide(title).c_str(), MB_OK);
}
#endif

