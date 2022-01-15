
#ifndef USE_CRL
#include<iostream>
#endif

#include"lispTypes.h"
#include"vm.h"
namespace LispScriptEngine {
#ifndef USE_CLR
	lisp_ptr CLispVoid::s_selfType(new CLispType("void"));
	lisp_ptr CLispInt::s_selfType(new CLispType("int"));
	lisp_ptr CLispBool::s_selfType(new CLispType("bool"));
	lisp_ptr CLispString::s_selfType(new CLispType("string"));
	lisp_ptr CLispRange::s_selfType(new CLispType("range"));
	lisp_ptr CLispArray::s_selfType(new CLispType("array"));
	lisp_ptr CLispNativeFunc::s_selfType(new CLispType("nativeFunc"));
	lisp_ptr CLispFuncCall::s_selfType(new CLispType("funcCall"));
	lisp_ptr CLispLet::s_selfType(new CLispType("let"));
	lisp_ptr CLispFor::s_selfType(new CLispType("for"));
	lisp_ptr CLispWhile::s_selfType(new CLispType("while"));
	lisp_ptr CLispIf::s_selfType(new CLispType("if"));
	lisp_ptr CLispIf::s_elseIfType(new CLispType("elseif"));
	lisp_ptr CLispChunck::s_elseType(new CLispType("else"));
	lisp_ptr CLispDefunc::s_selfType(new CLispType("defunc"));
	lisp_ptr CLispVarAccess::s_selfType(new CLispType("var access"));
#endif
	lisp_ptr CLispObject::find(STRING name) {
#ifdef USE_CLR
		if (m_members->ContainsKey(name)) {
			return m_members[name];
		}
		return nullptr;
#else
		MemberTbl::iterator it = m_members.find(name);
		if (it != m_members.end()) {
			return it->second;
		}
		return lisp_ptr();
#endif

	}
	void CLispObject::regist(STRING name, lisp_ptr p) {
#ifdef USE_CLR
		m_members[name] = p;
#else
		m_members.insert(MemberTbl::value_type(name, p));
#endif
	}
	CLispLet::CLispLet(STRING name, lisp_ptr v,  int idx)
		:CLispObject(s_selfType),
		m_name(name),
		m_value(v),
		m_idx(idx)
	{}

#ifdef USE_CLR
	void print(int n, Object^ p) {
		CVM^ lpVM = safe_cast<CVM^>(p);
		System::Console::WriteLine(lpVM->popObj()->ToString());
	}
#else
	void print(int, void* p) {
		CVM* lpVM = (CVM*)p;
		std::cout << lpVM->popObj()->toString() << std::endl;
	}
#endif
#ifdef USE_CLR
	void createRange(int, Object^ p) {
		CVM^ lpVM = safe_cast<CVM^>(p);
		int e = lpVM->popInt();
		int b = lpVM->popInt();
		lpVM->setRCValue(b, e);
	}
#else
	void createRange(int, void* p) {
		CVM* lpVM = (CVM*)p;
		int e = lpVM->popInt();
		int b = lpVM->popInt();
		lpVM->setRCValue(b, e);
	}
#endif
};