#pragma once
#include"ScriptEngineConfig.h"
#include"lispTypes.h"
namespace LispScriptEngine {
	DERIVE_CLASS(CLispError,CLispObject) {
	public:
#ifdef USE_CLR
		static lisp_ptr selfType=gcnew CLispType("error");
#else
		static lisp_ptr selfType;
#endif
	public:
		CLispError()
			:CLispObject(selfType)
		{}
	};

	DERIVE_CLASS(CLispUnexcpectedEOF,CLispError) {

	public:


	};
	DERIVE_CLASS(CLispInvalidToken, CLispError) {};
	DERIVE_CLASS(CLispUndefinedIdent,CLispError) {};
	DERIVE_CLASS(CLispAlreadyDefined,CLispError) {};
	DERIVE_CLASS(CLispNotRange, CLispError) {};
	DERIVE_CLASS(CLispElseNotInIf,CLispError) {};
	DERIVE_CLASS(CLispNeedsToken,CLispError) {
		STRING m_tokNeeds;
	public:
		CLispNeedsToken(STRING t)
			:CLispError(),
			m_tokNeeds(t)
		{}
	};
	DERIVE_CLASS(CLispIncludeParameterMustBeString,CLispError) {};
	DERIVE_CLASS(CLispRuntimeError, CLispError) {};

};