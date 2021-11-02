#include"pch.h"
#include"lispTypes.h"
#include"vm.h"
LISP_SCRIPT_API lisp_ptr CLispVoid::s_selfType(new CLispType("void"));
LISP_SCRIPT_API lisp_ptr CLispInt::s_selfType(new CLispType("int"));
LISP_SCRIPT_API lisp_ptr CLispBool::s_selfType(new CLispType("bool"));
LISP_SCRIPT_API lisp_ptr CLispString::s_selfType(new CLispType("string"));
LISP_SCRIPT_API lisp_ptr CLispRange::s_selfType(new CLispType("range"));
LISP_SCRIPT_API lisp_ptr CLispArray::s_selfType(new CLispType("array"));
LISP_SCRIPT_API lisp_ptr CLispNativeFunc::s_selfType(new CLispType("nativeFunc"));
LISP_SCRIPT_API lisp_ptr CLispFuncCall::s_selfType(new CLispType("funcCall"));
LISP_SCRIPT_API lisp_ptr CLispLet::s_selfType(new CLispType("let"));
LISP_SCRIPT_API lisp_ptr CLispFor::s_selfType(new CLispType("for"));
LISP_SCRIPT_API lisp_ptr CLispWhile::s_selfType(new CLispType("while"));
LISP_SCRIPT_API lisp_ptr CLispIf::s_selfType(new CLispType("if"));
LISP_SCRIPT_API lisp_ptr CLispIf::s_elseIfType(new CLispType("elseif"));
LISP_SCRIPT_API lisp_ptr CLispChunck::s_elseType(new CLispType("else"));
LISP_SCRIPT_API lisp_ptr CLispDefunc::s_selfType(new CLispType("defunc"));
LISP_SCRIPT_API lisp_ptr CLispVarAccess::s_selfType(new CLispType("var access"));

lisp_ptr CLispObject::find(std::string name) {

	MemberTbl::iterator it = m_members.find(name);
	if (it != m_members.end()) {
		return it->second;
	}
	return lisp_ptr();
}
void CLispObject::regist(std::string name,lisp_ptr p) {

	m_members.insert(MemberTbl::value_type(name, p));

}


void print(int,void* p) {
	CVM* lpVM = (CVM*)p;
	std::cout << lpVM->popObj()->toString() << std::endl;
}
void createRange(int,void* p) {
	CVM* lpVM = (CVM*)p;
	int e = lpVM->popInt();
	int b = lpVM->popInt();
	lpVM->setRCValue(b, e);
}