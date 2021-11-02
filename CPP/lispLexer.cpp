#include"pch.h"
#include"lispLexer.h"
#include"lispErrors.h"
std::string CLispLexer::getNext() {
	m_currentToken = parse();
	return m_currentToken;
}
std::string CLispLexer::parse() {

	while (m_pos < m_src.end() && isspace(*m_pos)) {
	
		++m_pos;
	}

	if (m_pos == m_src.end()) {
		
		if (m_currentToken.empty()) {
			throw CLispUnexcpectedEOF();
		}
		m_currentToken.clear();
		return m_currentToken;
		
	}



	switch (*m_pos) {
	case '(':
	case ')':
	case ':': {
		std::string tmp;
		tmp+=(*m_pos++);
		return tmp;
	}
	case '\"': {
		std::string::iterator it = m_pos + 1;
		while (m_pos < m_src.end() && (*it) != '\"') {
		
			++it;
		}
		std::string tmp(m_pos, it);
		m_pos = it + 1;
		return tmp;
	
	}
	
	default:
		if (isdigit(*m_pos)) {
			std::string::iterator it = m_pos + 1;
			while (it < m_src.end() && isdigit(*it)) {
				++it;
			}
			std::string tmp(m_pos, it);
			m_pos = it;
			return tmp;
		}
		else if (isalpha(*m_pos)) {
			std::string::iterator it = m_pos + 1;
			while (it < m_src.end() && isalpha(*it)) {
				++it;
			}
			std::string tmp(m_pos, it );
			m_pos = it ;
			return tmp;

		}
	
	
	}
	assert(false);
	return std::string();
}
bool CLispLexer::isEnd() {

	std::string::iterator it = m_pos;
	if (it < m_src.end() && isspace(*it)) {
		do {
		} while (isspace(*(++it)));
	}
	return it == m_src.end();

}
