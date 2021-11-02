#pragma once
#include"export.h"

class LISP_SCRIPT_API CLispLexer {
	std::string m_src;
	std::string::iterator m_pos;
	std::string m_currentToken;
public:
	CLispLexer() 
		:m_pos(m_src.end())
	{}
	CLispLexer(std::string src) {
		reset(src);
	}
	void reset(std::string src) {
		m_src = src;
		m_pos = m_src.begin();
		getNext();
	
	}

	std::string getCurrent() {
		return m_currentToken;
	}
	std::string::iterator getPos() {
		return m_pos;
	}
	void setPos(std::string::iterator pos) {
		m_pos = pos;
		getNext();
	}

	std::string getNext();

	bool isEnd();
private:
	std::string parse();
};
