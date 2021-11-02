#pragma once
#include"export.h"
#include"lispLexer.h"
#include"vm.h"
#include"lispParser.h"
class LISP_SCRIPT_API CLisp2ByteCode {

	CLispParser m_parser;

	
	typedef std::map<std::string, std::size_t> DefuncPosTbl;
	DefuncPosTbl m_defuncTbl;
	struct CDefuncCallInfo {
		std::string funcName;
		std::size_t callPos;

	public:
		CDefuncCallInfo(std::string fn, std::size_t cpos)
			:funcName(fn),
			callPos(cpos)
		{}
	};
	typedef std::list<CDefuncCallInfo> DefuncCallInfoTbl;
	DefuncCallInfoTbl m_callInfoTbl;

public:
	void parse(std::string s, CVM& vm);
	void parse(CLispParser& p, CVM& vm);
private:
	void parse(CLispLexer& l, CVM& vm);
	void parseChunck(lisp_ptr p, CVM& vm);
	void parseStatement(lisp_ptr p, CVM& vm);
	void parseCall(lisp_ptr p, CVM& vm);
	void parseLet(lisp_ptr p, CVM& vm);
	void parseValue(lisp_ptr p, CVM& vm);
	void parseWhile(lisp_ptr p, CVM& vm);
	void parseIf(lisp_ptr p, CVM& vm);
	void parseDefunc(lisp_ptr p, CVM& vm);

	std::size_t parseElseIf(lisp_ptr p, CVM& vm);
};