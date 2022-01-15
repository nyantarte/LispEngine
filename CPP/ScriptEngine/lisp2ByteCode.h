#pragma once
#include"ScriptEngineConfig.h"
#ifdef USE_CLR
using namespace System;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;
#else
#include<list>
#endif
#include"lispLexer.h"
#include"vm.h"
#include"lispParser.h"

namespace LispScriptEngine {
	DEF_CLASS(CDefuncCallInfo) {
public:

	STRING funcName;
	_SIZE_T callPos;
public:
	CDefuncCallInfo(STRING fn, _SIZE_T cpos)
		:funcName(fn),
		callPos(cpos)
	{}
	};
	DEF_CLASS(CLisp2ByteCode) {

		CLASS(CLispParser,m_parser);


		typedef MAP<STRING, _SIZE_T> DefuncPosTbl;

		CLASS(DefuncPosTbl,m_defuncTbl);


#ifdef USE_CLR
		typedef LIST<CDefuncCallInfo^> DefuncCallInfoTbl;
#else
		typedef LIST<CDefuncCallInfo> DefuncCallInfoTbl;
#endif

		CLASS(DefuncCallInfoTbl,m_callInfoTbl);
	public:
		void parse(STRING s, REF(CVM) vm);
		void parse(REF(CLispParser) p, REF(CVM) vm);
	private:
		void parse(REF(CLispLexer) l, REF(CVM) vm);
		void parseChunck(lisp_ptr p, REF(CVM) vm);
		void parseStatement(lisp_ptr p, REF(CVM) vm);
		void parseCall(lisp_ptr p, REF(CVM) vm);
		void parseLet(lisp_ptr p, REF(CVM) vm);
		void parseValue(lisp_ptr p, REF(CVM) vm);
		void parseWhile(lisp_ptr p, REF(CVM) vm);
		void parseIf(lisp_ptr p, REF(CVM) vm);
		void parseDefunc(lisp_ptr p, REF(CVM) vm);

		_SIZE_T parseElseIf(lisp_ptr p, REF(CVM) vm);
	};
};