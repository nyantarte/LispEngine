#include"pch.h"
#include"lispTypes.h"
lisp_ptr CLispInt::s_selfType(new CLispType("int"));
lisp_ptr CLispString::s_selfType(new CLispType("string"));
lisp_ptr CLispArray::s_selfType(new CLispType("array"));
lisp_ptr CLispNativeFunc::s_selfType(new CLispType("nativeFunc"));
lisp_ptr CLispFuncCall::s_selfType(new CLispType("funcCall"));
lisp_ptr CLispLet::s_selfType(new CLispType("let"));

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
