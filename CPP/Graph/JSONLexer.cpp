#include"JSONLexer.h"
#include"GraphError.h"
std::string CJSONLexer::parse() {

	while (m_pos < m_src.end() &&( isspace(*m_pos) || ','==*m_pos)) {

		++m_pos;
	}

	if (m_pos == m_src.end()) {
		if (m_currentToken.empty()) {
			throw CJSONUnexcpectedEOF();

		}
		m_currentToken.clear();
		return m_currentToken;

	}

	if ('{' == *m_pos || '}' == *m_pos || ':'==*m_pos || '['==*m_pos || ']'==*m_pos) {
		std::string tmp;
		tmp+=(*m_pos++);

		return tmp;
	}

	if ('\"' == *m_pos) {
		std::string::iterator it = m_pos + 1;
		while (it < m_src.end() && '\"' != (*it)) {

			++it;
		}
		std::string tmp(m_pos, it);
		m_pos = it + 1;
		return tmp;
	
	}

	if (isdigit(*m_pos)) {
		std::string::iterator it = m_pos + 1;
		while (it < m_src.end() && isdigit(*it)) {
			++it;
		}
		std::string tmp(m_pos, it);
		m_pos = it;
		return tmp;
	}

	if (isalpha(*m_pos)) {
		std::string::iterator it = m_pos + 1;
		while (it < m_src.end() && (isalnum(*it) || '_' == *it)) {
			++it;
		}
		std::string tmp(m_pos, it);
		m_pos = it;

		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		return tmp;



	}
	
	assert(false);
	return std::string();
}

bool CJSONLexer::isEnd() {

	std::string::iterator it = m_pos;
	if (it < m_src.end() && isspace(*it)) {
		do {
		} while (isspace(*(++it)));
	}
	return it == m_src.end();
}