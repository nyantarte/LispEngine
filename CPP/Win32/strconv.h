#pragma once
#include"Win32Framework.h"
std::wstring cp_to_wide(const std::string& s, UINT codepage);
std::string wide_to_cp(const std::wstring& s, UINT codepage);

std::string cp_to_utf8(const std::string& s, UINT codepage);
std::string utf8_to_cp(const std::string& s, UINT codepage);
std::wstring ansi_to_wide(const std::string& s);
std::string wide_to_ansi(const std::wstring& s);
std::wstring sjis_to_wide(const std::string& s);
std::string wide_to_sjis(const std::wstring& s);

std::wstring utf8_to_wide(const std::string& s);
std::string wide_to_utf8(const std::wstring& s);

std::string ansi_to_utf8(const std::string& s);
std::string utf8_to_ansi(const std::string& s);

std::string sjis_to_utf8(const std::string& s);
std::string utf8_to_sjis(const std::string& s);

#ifdef __cpp_char8_t
std::u8string utf8_to_char8(const std::string& s);
std::string char8_to_utf8(const std::u8string& s);

std::wstring char8_to_wide(const std::u8string& s);
std::u8string wide_to_char8(const std::wstring& s);

std::u8string cp_to_char8(const std::string& s, UINT codepage);
std::string char8_to_cp(const std::u8string& s, UINT codepage);

std::u8string ansi_to_char8(const std::string& s);
std::string char8_to_ansi(const std::u8string& s);

std::u8string sjis_to_char8(const std::string& s);
std::string char8_to_sjis(const std::u8string& s);
#endif



std::wstring vformat(const wchar_t* format, va_list args);
std::string vformat(const char* format, va_list args);
#ifdef __cpp_char8_t
std::u8string vformat(const char8_t* format, va_list args);
#endif



std::wstring format(const wchar_t* format, ...);
std::string format(const char* format, ...);
#ifdef __cpp_char8_t
std::u8string format(const char8_t* format, ...);
#endif

void format(std::ostream& ostrm, const wchar_t* format, ...);
void format(std::ostream& ostrm, const char* format, ...);
#ifdef __cpp_char8_t
void format(std::ostream& ostrm, const char8_t* format, ...);
#endif

std::string formatA(const wchar_t* format, ...);
std::string formatA(const char* format, ...);
#ifdef __cpp_char8_t
std::string formatA(const char8_t* format, ...);
#endif

void formatA(std::ostream& ostrm, const wchar_t* format, ...);
void formatA(std::ostream& ostrm, const char* format, ...);
#ifdef __cpp_char8_t
void formatA(std::ostream& ostrm, const char8_t* format, ...);
#endif

void dbgmsg(const wchar_t* title, const wchar_t* format, ...);
void dbgmsg(const char* title, const char* format, ...);
#ifdef __cpp_char8_t
void dbgmsg(const char8_t* title, const char8_t* format, ...);
#endif

class unicode_ostream
{
private:
    std::ostream* m_ostrm;
    UINT m_target_cp;
    bool is_ascii(const std::string& s)
    {
        for (std::size_t i = 0; i < s.size(); i++)
        {
            unsigned char c = (unsigned char)s[i];
            if (c > 0x7f)
                return false;
        }
        return true;
    }

public:
    unicode_ostream(std::ostream& ostrm, UINT target_cp = CP_ACP) : m_ostrm(&ostrm), m_target_cp(target_cp) {}
    std::ostream& stream() { return *m_ostrm; }
    void stream(std::ostream& ostrm) { m_ostrm = &ostrm; }
    UINT target_cp() { return m_target_cp; }
    void target_cp(UINT cp) { m_target_cp = cp; }
    template <typename T>
    unicode_ostream& operator<<(const T& x)
    {
        std::ostringstream oss;
        oss << x;
        std::string output = oss.str();
        if (is_ascii(output))
        {
            (*m_ostrm) << x;
        }
        else
        {
            (*m_ostrm) << utf8_to_cp(output, m_target_cp);
        }
        return *this;
    }
    unicode_ostream& operator<<(const std::wstring& x)
    {
        (*m_ostrm) << wide_to_cp(x, m_target_cp);
        return *this;
    }
    unicode_ostream& operator<<(const wchar_t* x)
    {
        (*m_ostrm) << wide_to_cp(x, m_target_cp);
        return *this;
    }
    unicode_ostream& operator<<(const std::string& x)
    {
        (*m_ostrm) << utf8_to_cp(x, m_target_cp);
        return *this;
    }
    unicode_ostream& operator<<(const char* x)
    {
        (*m_ostrm) << utf8_to_cp(x, m_target_cp);
        return *this;
    }
#ifdef __cpp_char8_t
    unicode_ostream& operator<<(const std::u8string& x)
    {
        (*m_ostrm) << char8_to_cp(x, m_target_cp);
        return *this;
    }
    unicode_ostream& operator<<(const char8_t* x)
    {
        (*m_ostrm) << char8_to_cp(x, m_target_cp);
        return *this;
    }
#endif
    unicode_ostream& operator<<(std::ostream& (*pf)(std::ostream&)) // For manipulators...
    {
        (*m_ostrm) << pf;
        return *this;
    }
    unicode_ostream& operator<<(std::basic_ios<char>& (*pf)(std::basic_ios<char>&)) // For manipulators...
    {
        (*m_ostrm) << pf;
        return *this;
    }
};

#define U8(X) ((const char *)u8##X)
#define WIDE(X) (L##X)
