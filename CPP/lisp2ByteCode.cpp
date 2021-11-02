#include"pch.h"
#include"lisp2ByteCode.h"
#include"lispParser.h"
#include"lispErrors.h"
void CLisp2ByteCode::parse(std::string s, CVM& vm) {
	CLispLexer l(s);
	parse(l, vm);
}
void CLisp2ByteCode::parse(CLispLexer& l, CVM& vm) {
	CLispParser parser;

	parser.parse(l);
	parse(parser, vm);

}
void CLisp2ByteCode::parse( CLispParser& parser, CVM& vm) {
	lisp_ptr pRoot = parser.getRoot();
	assert(nullptr != pRoot);
	parseChunck(pRoot, vm);

	for (CDefuncCallInfo& ci : m_callInfoTbl) {
	
		DefuncPosTbl::iterator it = m_defuncTbl.find(ci.funcName);
		if (it == m_defuncTbl.end()) {
			throw CLispUndefinedIdent();
		}
		vm.setJmpPos(ci.callPos, it->second);
	}

	m_callInfoTbl.clear();
}
void CLisp2ByteCode::parseChunck(lisp_ptr p, CVM& vm) {
	CLispChunck* pChunck = dynamic_cast<CLispChunck*>(p.get());
	assert(nullptr != pChunck);
	vm.addThisCmd(p);
	for (lisp_ptr pExp : pChunck->getImplements()) {
		parseStatement(pExp, vm);
	}
	vm.addPopThisCmd();

}

void CLisp2ByteCode::parseStatement(lisp_ptr pExp, CVM& vm) {
	if (pExp->getType() == CLispLet::s_selfType) {
		parseLet(pExp, vm);
	}
	else if (pExp->getType() == CLispFuncCall::s_selfType) {
		parseCall(pExp, vm);
	}
	else if (pExp->getType() == CLispIf::s_selfType) {
		parseIf(pExp, vm);
	}
	else if (pExp->getType() == CLispWhile::s_selfType) {
		parseWhile(pExp, vm);
	}
	else if (pExp->getType() == CLispDefunc::s_selfType) {
		parseDefunc(pExp, vm);
	}
	else if (pExp->getType() == CLispVoid::s_selfType) {
	}
	else {
		assert(false);
	}
}

void CLisp2ByteCode::parseCall(lisp_ptr p, CVM& vm) {
	assert(CLispFuncCall::s_selfType == p->getType());
	CLispFuncCall* pFuncCall=reinterpret_cast<CLispFuncCall*>(p.get());
	CLispArray* pArgs = reinterpret_cast<CLispArray*>(pFuncCall->getArgs().get());
	if (CLispDefunc::s_selfType == pFuncCall->getFunc()->getType()) {
		vm.addPushRetPosCmd();
		vm.addPushThisCmd();

	}
	for (lisp_ptr arg :pArgs->value() ) {
		parseValue(arg,vm);
	}

	lisp_ptr pTargetFunc=pFuncCall->getFunc();
	if (CLispNativeFunc::s_selfType == pTargetFunc->getType()) {
		CLispNativeFunc* pNativeFunc = reinterpret_cast<CLispNativeFunc*>(pTargetFunc.get());
		vm.addNativeCallCmd(pArgs->value().size(), pNativeFunc->getFunc());
	}
	else if (CLispDefunc::s_selfType == pTargetFunc->getType()) {

		CLispDefunc* pDefunc = reinterpret_cast<CLispDefunc*>(pTargetFunc.get());
		m_callInfoTbl.push_back(DefuncCallInfoTbl::value_type(pDefunc->getName(), vm.getCmdBufLen()));

	}
	else {
		assert(false);
	}


	
}

void CLisp2ByteCode::parseLet(lisp_ptr p, CVM& vm) {

	assert(p->getType() == CLispLet::s_selfType);

	CLispChunck* pOwner = reinterpret_cast<CLispChunck*>(m_parser.getTop().get());
	CLispLet* pLet = reinterpret_cast<CLispLet*>(p.get());
	if (-1==pLet->index()) {
		pLet->index()=pOwner->varNum()++;
		parseValue(pLet->getValue(), vm);
		vm.addSetCmd(pLet->index());

	}
	else {
		assert(false);
		//vm.addPushVarCmd(pLet->index());
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
	else if (p->getType() == CLispVarAccess::s_selfType) {
		CLispVarAccess* pAccess = reinterpret_cast<CLispVarAccess*>(p.get());
		CLispLet* pLet = reinterpret_cast<CLispLet*>(pAccess->getValue().get());
		vm.addPushVarCmd(pLet->index(), pAccess->getPopNum());
	}
	

}
void CLisp2ByteCode::parseWhile(lisp_ptr p, CVM& vm) {
	CLispWhile* pWhile = reinterpret_cast<CLispWhile*>(p.get());
	std::size_t jmpContCheckPos = vm.getCmdBufLen();
	vm.addPushIntCmd(1);
	parseValue(pWhile->getContCondExp(), vm);
	vm.addCmpCmd();
	std::size_t jmpExitCmdPos = vm.getCmdBufLen();
	vm.addJmpNCmd(-1);
	parseChunck(p, vm);
	vm.addJmpCmd(jmpContCheckPos);
	std::size_t exitPos = vm.getCmdBufLen();
	vm.setJmpPos(jmpExitCmdPos,exitPos);

}

void CLisp2ByteCode::parseIf(lisp_ptr p, CVM& vm) {
	CLispIf* pIf = reinterpret_cast<CLispIf*>(p.get());
	parseValue(pIf->getTestCondExp(), vm);
	vm.addCmpCmd();
	std::size_t jmpCmdPos=vm.getCmdBufLen();
	vm.addJmpNCmd(-1);
	parseChunck(p,vm);
	for (lisp_ptr pExp : pIf->getElseIfTbl()) {
		std::size_t curCmdLen = vm.getCmdBufLen();
		vm.setJmpPos(jmpCmdPos, curCmdLen);
		jmpCmdPos=parseElseIf(pExp, vm);
	}
	std::size_t curCmdLen = vm.getCmdBufLen();
	vm.setJmpPos(jmpCmdPos, curCmdLen);
	for (lisp_ptr pExp : pIf->getElseTbl()) {
		
		
		parseChunck(pExp, vm);
	}

}
std::size_t CLisp2ByteCode::parseElseIf(lisp_ptr p, CVM& vm) {
	assert(p->getType() == CLispIf::s_elseIfType);
	CLispElseIf* pEIf = reinterpret_cast<CLispElseIf*>(p.get());
	parseValue(pEIf->getTestCondExp(), vm);
	vm.addCmpCmd();
	std::size_t jmpCmdPos = vm.getCmdBufLen();
	vm.addJmpNCmd(-1);
	parseChunck(p, vm);
	return jmpCmdPos;
}
void CLisp2ByteCode::parseDefunc(lisp_ptr p, CVM& vm) {
	assert(p->getType() == CLispDefunc::s_selfType);
	std::size_t fEndJmpPos = vm.getCmdBufLen();
	vm.addJmpCmd(0);
	CLispDefunc* pDefunc = reinterpret_cast<CLispDefunc*>(p.get());
	m_defuncTbl.insert(DefuncPosTbl::value_type(pDefunc->getName(), vm.getCmdBufLen()));
	//vm.addThisCmd(p);

	parseChunck(p,vm);

	vm.addRetCmd((reinterpret_cast<CLispArray*>(pDefunc->getArgs().get()))->value().size());

	vm.setJmpPos(fEndJmpPos, vm.getCmdBufLen());
}