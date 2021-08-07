#pragma once
#include"lispTypes.h"
class CLispError :public CLispObject{
public:
	static lisp_ptr selfType;

public:
	CLispError()
		:CLispObject(selfType)
	{}
};

class CLispUnexcpectedEOF :public CLispError {

public:
	

};
class CLispInvalidToken :public CLispError {};
class CLispUndefinedIdent :public CLispError {};
class CLispAlreadyDefined :public CLispError {};
