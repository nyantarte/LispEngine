#ifndef USE_CRL
#include<cassert>
#endif



#include"lispLexer.h"
#include"lispErrors.h"


namespace LispScriptEngine {
	STRING CLispLexer::getNext() {
		m_currentToken = parse();
		return m_currentToken;
	}
	STRING CLispLexer::parse() {

		while (
#ifdef USE_CLR
			m_pos < m_src->Length && Char::IsWhiteSpace(m_src[m_pos])
#else
			m_pos < m_src.end() && isspace(*m_pos)
#endif
			
			) {

			++m_pos;
		}

		if (
#ifdef USE_CLR
			m_pos==m_src->Length
#else
			m_pos == m_src.end()
#endif
			
			) {

			if (
#ifdef USE_CLR
				0==m_currentToken->Length
#else
				m_currentToken.empty()
#endif
				) {
#ifdef USE_CLR
				throw gcnew CLispUnexcpectedEOF();
#else
				throw CLispUnexcpectedEOF();
#endif
			}
#ifdef USE_CLR
			m_currentToken = String::Empty;
#else
			m_currentToken.clear();
#endif
			return m_currentToken;

		}



		switch (
#ifdef USE_CLR
			m_src[m_pos]
#else
			*m_pos
#endif
			) {
		case '(':
		case ')':
		case ':': {
#ifdef USE_CLR
			String^ tmp = String::Empty;
#else
			std::string tmp;
#endif
			tmp += (
#ifdef USE_CLR
				m_src[m_pos++]
#else
				*m_pos++
#endif
				);
			return tmp;
		}
		case '\"': {
#ifdef USE_CLR
			int it = m_pos + 1;
			while(it < m_src->Length && '\"'!=m_src[it]){
#else
			std::string::iterator it = m_pos + 1;
			while (it < m_src.end() && '\"'!=(*it)) {
#endif

				++it;
			}
#ifdef USE_CLR
			String^ tmp = m_src->Substring(m_pos, it-m_pos);
#else
			std::string tmp(m_pos, it);
#endif
			m_pos = it + 1;
			return tmp;

		}

		default:
			if (
#ifdef USE_CLR
				Char::IsNumber(m_src[m_pos])
#else
				isdigit(*m_pos)
#endif
				) {
#ifdef USE_CLR
				int it = m_pos + 1;
				while (it < m_src->Length && Char::IsDigit(m_src[it])){
#else
				std::string::iterator it = m_pos + 1;
				while (it < m_src.end() && isdigit(*it)) {
#endif
					++it;
				}
#ifdef USE_CLR
				String^ tmp = m_src->Substring(m_pos, it - m_pos);
#else
				std::string tmp(m_pos, it);
#endif
				m_pos = it;
				return tmp;
			}
			else if (
#ifdef USE_CLR
				Char::IsLetter(m_src[m_pos])
#else
				isalpha(*m_pos)
#endif
				) {
#ifdef USE_CLR
				int it = m_pos + 1;
				while(it < m_src->Length && Char::IsLetter(m_src[it])){
#else
				std::string::iterator it = m_pos + 1;
				while (it < m_src.end() && (isalpha(*it) || '_'==*it)) {
#endif
					++it;
				}
#ifdef USE_CLR
				String^ tmp = m_src->Substring(m_pos, it - m_pos);
#else
				std::string tmp(m_pos, it);
#endif
				m_pos = it;
				return tmp;

			}


		}
		assert(false);
#ifdef USE_CLR
		return String::Empty;
#else
		return std::string();
#endif
	}
	bool CLispLexer::isEnd() {

#ifdef USE_CLR
		int it = m_pos;
		if(it < m_src->Length && Char::IsWhiteSpace(m_src[it])){
#else
		std::string::iterator it = m_pos;
		if (it < m_src.end() && isspace(*it)) {
#endif
			do {
			} while (
#ifdef USE_CLR
				Char::IsWhiteSpace(m_src[++it])
#else
				isspace(*(++it))
#endif
				);
		}
#ifdef USE_CLR
		return it == m_src->Length;
#else
		return it == m_src.end();
#endif
	}
};