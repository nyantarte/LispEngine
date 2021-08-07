#pragma once
#include"lispLexer.h"
#include"vm.h"
#include"lispParser.h"
class CLisp2ByteCode {

	CLispParser m_parser;
private:
	void parse(CLispLexer& l, CVM& vm);
	void parseCall(lisp_ptr p, CVM& vm);
	void parseLet(lisp_ptr p, CVM& vm);
	void parseValue(lisp_ptr p, CVM& vm);
};