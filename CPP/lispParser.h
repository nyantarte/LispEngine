#pragma once
#include"export.h"
#include"lispTypes.h"
#include"lispLexer.h"
class LISP_SCRIPT_API CLispParser {
	lisp_ptr m_root;
	typedef std::vector<lisp_ptr> NameStack;
	NameStack m_nameStack;

	static lisp_ptr s_rootType;
public:
	CLispParser();

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
	lisp_ptr parseFor(CLispLexer& l);
	lisp_ptr parseWhile(CLispLexer& l);
	lisp_ptr parseIf(CLispLexer& l);
	lisp_ptr parseElseIf(CLispLexer& l);
	lisp_ptr parseElse(CLispLexer& l);
	lisp_ptr parseDefunc(CLispLexer& l);
	lisp_ptr parseInclude(CLispLexer& l);

	lisp_ptr find(std::string name,int &popNum);
	void regist(std::string name, lisp_ptr p);

	void push(lisp_ptr p) {
		m_nameStack.push_back(p);
	}
	void pop() {
		m_nameStack.pop_back();
	}
	void genLexerFromFile(CLispLexer& l,std::string fname);

};