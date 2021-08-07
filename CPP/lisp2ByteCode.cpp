#include"pch.h"
#include"lisp2ByteCode.h"
#include"lispParser.h"
void CLisp2ByteCode::parse(CLispLexer& l, CVM& vm) {
	CLispParser parser;

	parser.parse(l);

	lisp_ptr pRoot = parser.getRoot();
	assert(nullptr != pRoot);
	CLispChunck* pChunck = dynamic_cast<CLispChunck*>(pRoot.get());
	assert(nullptr != pChunck);
	for (lisp_ptr pExp : pChunck->getImplements()) {
		if (pExp->getType() == CLispLet::s_selfType) {
			parseLet(pExp,vm);
		}
		else if (pExp->getType() == CLispFuncCall::s_selfType) {
			parseCall(pExp,vm);
		}
	}

}
void CLisp2ByteCode::parseCall(lisp_ptr p, CVM& vm) {
	assert(CLispFuncCall::s_selfType == p->getType());
	CLispFuncCall* pFuncCall=reinterpret_cast<CLispFuncCall*>(p.get());
	CLispArray* pArgs = reinterpret_cast<CLispArray*>(pFuncCall->getArgs().get());
	for (lisp_ptr arg :pArgs->value() ) {
		parseValue(arg,vm);
	}

	lisp_ptr pTargetFunc=pFuncCall->getFunc();
	assert(CLispNativeFunc::s_selfType==pTargetFunc->getType());
	CLispNativeFunc* pNativeFunc=reinterpret_cast<CLispNativeFunc*>(pTargetFunc.get());
	vm.addNativeCallCmd(pNativeFunc->getFunc());

	
}

void CLisp2ByteCode::parseLet(lisp_ptr p, CVM& vm) {

	assert(p->getType() == CLispLet::s_selfType);

	CLispChunck* pOwner = reinterpret_cast<CLispChunck*>(m_parser.getTop().get());
	CLispLet* pLet = reinterpret_cast<CLispLet*>(p.get());
	if (-1==pLet->index()) {
		pLet->index()=pOwner->varNum()++;
		parseValue(pLet->getValue(), vm);

	}
	else {
		vm.addPushVarCmd(pLet->index());
	}
}
void  CLisp2ByteCode::parseValue(lisp_ptr p, CVM& vm) {
	CLispLet* pLet = reinterpret_cast<CLispLet*>(p.get());
	if (p->getType() == CLispLet::s_selfType) {
		parseLet(p, vm);
	}
	else if (p->getType() == CLispInt::s_selfType) {
		CLispInt* pInt = reinterpret_cast<CLispInt*>(p.get());
		vm.addPushIntCmd(pInt->value());
	}
	else if (p->getType() == CLispString::s_selfType) {
		CLispString* pStr = reinterpret_cast<CLispString*>(p.get());
		vm.addPushStrCmd(pStr->value());
	}
	else if (p->getType() == CLispFuncCall::s_selfType) {
		parseCall(p, vm);
		vm.addPushRCCmd();
	}
}