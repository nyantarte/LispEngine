#include"lispErrors.h"

namespace LispScriptEngine {
#ifndef USE_CLR
	lisp_ptr CLispError::selfType(new CLispType("error"));
#endif
};