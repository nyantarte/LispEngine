#pragma once
#include"lispTypes.h"
#include"lispLexer.h"
class CLispParser {
	lisp_ptr m_root;
	typedef std::vector<lisp_ptr> NameStack;
	NameStack m_nameStack;
public:


	void parse(CLispLexer& l);

	lisp_ptr getRoot() {
		return m_root;
	}

	lisp_ptr getTop() {
		return *(m_nameStack.end() - 1);
	}

private:
	lisp_ptr parseValue(CLispLexer& l);
	lisp_ptr parseCall(CLispLexer& l);
	lisp_ptr parseArgs(CLispLexer& l);
	lisp_ptr parseLet(CLispLexer& l);


	lisp_ptr find(std::string name);
	void regist(std::string name, lisp_ptr p);

};