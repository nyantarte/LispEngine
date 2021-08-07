#pragma once
class CLispLexer {
	std::string m_src;
	std::string::iterator m_pos;
	std::string m_currentToken;
public:
	CLispLexer(std::string src)
		:m_src(src),
		m_pos(src.begin())
	{
		m_currentToken = getNext();
	}


	std::string getCurrent() {
		return m_currentToken;
	}

	std::string getNext();

	bool isEnd() {
		return m_pos == m_src.end();

	}
private:
	std::string parse();
};
