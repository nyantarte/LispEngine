#pragma once
#include"GraphFramework.h"


class CJSONLexer {
	std::string m_sourceFile;
	int m_lineNo;
	std::string m_src;
	std::string::iterator m_pos;
	std::string m_currentToken;

public:
	CJSONLexer(std::string src) 
		:m_src(src),
		 m_pos(m_src.begin())
	{
		m_currentToken=getNext();
	}
	std::string getNext() {
		m_currentToken = parse();
		return m_currentToken;
	}

	std::string getCurrent() {
		return m_currentToken;
	}
	bool isEnd();
private:
	std::string parse();
};