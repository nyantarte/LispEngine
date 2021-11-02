#pragma once
#include"lispTypes.h"
class LISP_SCRIPT_API CLispError :public CLispObject{
public:
	static lisp_ptr selfType;

public:
	CLispError()
		:CLispObject(selfType)
	{}
};

class LISP_SCRIPT_API CLispUnexcpectedEOF :public CLispError {

public:
	

};
class LISP_SCRIPT_API CLispInvalidToken :public CLispError {};
class LISP_SCRIPT_API CLispUndefinedIdent :public CLispError {};
class LISP_SCRIPT_API CLispAlreadyDefined :public CLispError {};
class LISP_SCRIPT_API CLispNotRange :public CLispError {};
class LISP_SCRIPT_API CLispElseNotInIf :public CLispError {};
class LISP_SCRIPT_API CLispNeedsToken :public CLispError {
	std::string m_tokNeeds;
public:
	CLispNeedsToken(std::string t)
		:CLispError(),
		 m_tokNeeds(t)
	{}
};
class LISP_SCRIPT_API CLispIncludeParameterMustBeString :public CLispError {};