#include"ScriptEngineConfig.h"
#ifndef USE_CLR
#include<cassert>
#endif
#include"lisp2ByteCode.h"
#include"lispParser.h"
#include"lispErrors.h"
namespace LispScriptEngine {
	void CLisp2ByteCode::parse(STRING s, REF(CVM) vm) {
#ifdef USE_CLR
		CLispLexer^ l=gcnew CLispLexer(s);
#else
		CLispLexer l(s);
#endif
		parse(l, vm);
	}
	void CLisp2ByteCode::parse(REF(CLispLexer) l, REF(CVM) vm) {
		CLASS(CLispParser,parser);

		REF_ACCESS(parser,parse)(l);
		parse(parser, vm);

	}
	void CLisp2ByteCode::parse(REF(CLispParser) parser, REF(CVM) vm) {
		lisp_ptr pRoot = REF_ACCESS(parser,getRoot)();

		ASSERT(nullptr != pRoot);
		parseChunck(pRoot, vm);
#ifdef USE_CLR
		for each(CDefuncCallInfo^ ci in m_callInfoTbl) {
#else
		for (CDefuncCallInfo& ci : m_callInfoTbl) {
#endif

#ifdef USE_CLR
			if (!m_defuncTbl->ContainsKey(ci->funcName)) {

				throw gcnew  CLispUndefinedIdent();
			}
			vm->setJmpPos(ci->callPos, m_defuncTbl[ci->funcName]);
#else
			DefuncPosTbl::iterator it = m_defuncTbl.find(ci.funcName);
			if (it == m_defuncTbl.end()) {
				throw CLispUndefinedIdent();
			}

			vm.setJmpPos(ci.callPos, it->second);
#endif
		}
#ifdef USE_CLR
		m_callInfoTbl->Clear();
#else
		m_callInfoTbl.clear();
#endif
	}
	void CLisp2ByteCode::parseChunck(lisp_ptr p, REF(CVM) vm) {
#ifdef USE_CLR
		CLispChunck^ pChunck = safe_cast<CLispChunck^>(p);;

#else
		CLispChunck* pChunck = dynamic_cast<CLispChunck*>(p.get());
		
#endif
		ASSERT(nullptr != pChunck);
		REF_ACCESS(vm,addThisCmd)(p);
#ifdef USE_CLR
		for each(lisp_ptr pExp in pChunck->getImplements()) {

#else
		for (lisp_ptr pExp : pChunck->getImplements()) {
#endif
			parseStatement(pExp, vm);
		}
		REF_ACCESS(vm,addPopThisCmd)();

	}

	void CLisp2ByteCode::parseStatement(lisp_ptr pExp, REF(CVM) vm) {
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
			ASSERT(false);
		}
	}

	void CLisp2ByteCode::parseCall(lisp_ptr p, REF(CVM) vm) {
		ASSERT(CLispFuncCall::s_selfType == p->getType());
#ifdef USE_CLR
		CLispFuncCall^ pFuncCall = safe_cast<CLispFuncCall^>(p);
		CLispArray^ pArgs = safe_cast<CLispArray^>(pFuncCall->getArgs());
#else
		CLispFuncCall* pFuncCall = reinterpret_cast<CLispFuncCall*>(p.get());
		CLispArray* pArgs = reinterpret_cast<CLispArray*>(pFuncCall->getArgs().get());
#endif

		if (CLispDefunc::s_selfType == pFuncCall->getFunc()->getType()) {
			REF_ACCESS(vm,addPushRetPosCmd)();
			REF_ACCESS(vm,addPushThisCmd)();

		}
#ifdef USE_CLR
		for each(lisp_ptr arg in pArgs->value) {
#else
		for (lisp_ptr arg : pArgs->value()) {
#endif
			parseValue(arg, vm);
		}

		lisp_ptr pTargetFunc = pFuncCall->getFunc();
		if (CLispNativeFunc::s_selfType == pTargetFunc->getType()) {
#ifdef USE_CLR
			CLispNativeFunc^ pNativeFunc = safe_cast<CLispNativeFunc^>(pTargetFunc);
#else
			CLispNativeFunc* pNativeFunc = reinterpret_cast<CLispNativeFunc*>(pTargetFunc.get());
#endif
			REF_ACCESS(vm,addNativeCallCmd)(
#ifdef USE_CLR
				pArgs->value->Count
#else
				pArgs->value().size()
#endif				
				, pNativeFunc->getFunc());
		}
		else if (CLispDefunc::s_selfType == pTargetFunc->getType()) {

#ifdef USE_CLR
			CLispDefunc^ pDefunc = safe_cast<CLispDefunc^>(pTargetFunc);
			m_callInfoTbl->AddLast(gcnew CDefuncCallInfo(pDefunc->getName(), vm->getCmdBufLen()));
#else
			CLispDefunc* pDefunc = reinterpret_cast<CLispDefunc*>(pTargetFunc.get());
			m_callInfoTbl.push_back(DefuncCallInfoTbl::value_type(pDefunc->getName(), vm.getCmdBufLen()));
#endif


		}
		else {
			ASSERT(false);
		}



	}

	void CLisp2ByteCode::parseLet(lisp_ptr p, REF(CVM) vm) {

		ASSERT(p->getType() == CLispLet::s_selfType);
#ifdef USE_CLR
	///	CLispChunck^ pOwner = safe_cast<CLispChunck^>(m_parser->getTop());
		CLispLet^ pLet = safe_cast<CLispLet^>(p);
#else
//		CLispChunck* pOwner = reinterpret_cast<CLispChunck*>(m_parser.getTop().get());
		CLispLet* pLet = reinterpret_cast<CLispLet*>(p.get());
#endif




		parseValue(pLet->getValue(), vm);
		REF_ACCESS(vm, addSetCmd)(
#if USE_CLR
			pLet->index
#else
			pLet->index()
#endif

			);
	}
	void  CLisp2ByteCode::parseValue(lisp_ptr p, REF(CVM) vm) {
#ifdef USE_CLR
		CLispLet^ pLet = safe_cast<CLispLet^>(p);
#else
		CLispLet* pLet = reinterpret_cast<CLispLet*>(p.get());
#endif
		if (p->getType() == CLispLet::s_selfType) {
			parseLet(p, vm);
		}
		else if (p->getType() == CLispInt::s_selfType) {
#ifdef USE_CLR
			CLispInt^ pInt = safe_cast<CLispInt^>(p);
#else
			CLispInt* pInt = reinterpret_cast<CLispInt*>(p.get());
#endif
			REF_ACCESS(vm, addPushIntCmd)(
#ifdef USE_CLR
				pInt->value
#else
				pInt->value()
#endif

				);
		}
		else if (p->getType() == CLispString::s_selfType) {
#ifdef USE_CLR
			CLispString^ pStr = safe_cast<CLispString^>(p);
#else
			CLispString* pStr = reinterpret_cast<CLispString*>(p.get());
#endif
			REF_ACCESS(vm,addPushStrCmd)(
#ifdef USE_CLR
				pStr->value
#else
				pStr->value()
#endif

			);
		}
		else if (p->getType() == CLispFuncCall::s_selfType) {
			parseCall(p, vm);
			REF_ACCESS(vm,addPushRCCmd)();
		}
		else if (p->getType() == CLispVarAccess::s_selfType) {
#ifdef USE_CLR
			CLispVarAccess^ pAccess = safe_cast<CLispVarAccess^>(p);
#else
			CLispVarAccess* pAccess = reinterpret_cast<CLispVarAccess*>(p.get());
#endif
			if (pAccess->getValue()->getType() == CLispInt::s_selfType) {
#ifdef USE_CLR
				CLispInt^ pInt = safe_cast<CLispInt^>(pAccess->getValue());
#else
				CLispInt* pInt = reinterpret_cast<CLispInt*>(pAccess->getValue().get());
#endif
				REF_ACCESS(vm, addPushIntCmd)(
#ifdef USE_CLR
					pInt->value
#else
					pInt->value()
#endif

					);
			}
			else if (pAccess->getValue()->getType() == CLispString::s_selfType) {
#ifdef USE_CLR
				CLispString^ pStr = safe_cast<CLispString^>(pAccess->getValue());
#else
				CLispString* pStr = reinterpret_cast<CLispString*>(pAccess->getValue().get());
#endif
				REF_ACCESS(vm, addPushStrCmd)(
#ifdef USE_CLR
					pStr->value
#else
					pStr->value()
#endif

					);


			}
			else if (pAccess->getValue()->getType() == CLispLet::s_selfType) {
#ifdef USE_CLR
				CLispLet^ pLet = safe_cast<CLispLet^>(pAccess->getValue());
#else
				CLispLet* pLet = reinterpret_cast<CLispLet*>(pAccess->getValue().get());
#endif
				REF_ACCESS(vm, addPushVarCmd)(
#ifdef USE_CLR
					pLet->index,
#else
					pLet->index(),
#endif

					pAccess->getPopNum());
			}
		}



	}
	void CLisp2ByteCode::parseWhile(lisp_ptr p, REF(CVM) vm) {
#ifdef USE_CLR
		CLispWhile^ pWhile = safe_cast<CLispWhile^>(p);
#else
		CLispWhile* pWhile = reinterpret_cast<CLispWhile*>(p.get());
#endif
		_SIZE_T jmpContCheckPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,addPushIntCmd)(1);
		parseValue(pWhile->getContCondExp(), vm);
		REF_ACCESS(vm,addCmpCmd)();
		_SIZE_T jmpExitCmdPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,addJmpNCmd)(-1);
		parseChunck(p, vm);
		REF_ACCESS(vm,addJmpCmd)(jmpContCheckPos);
		_SIZE_T exitPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,setJmpPos)(jmpExitCmdPos, exitPos);

	}

	void CLisp2ByteCode::parseIf(lisp_ptr p, REF(CVM) vm) {
#ifdef USE_CLR
		CLispIf^ pIf = safe_cast<CLispIf^>(p);
#else
		CLispIf* pIf = reinterpret_cast<CLispIf*>(p.get());
#endif
		parseValue(pIf->getTestCondExp(), vm);
		REF_ACCESS(vm, addPushIntCmd)(1);
		REF_ACCESS(vm,addCmpCmd)();
		_SIZE_T jmpCmdPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,addJmpNCmd)(-1);
		parseChunck(p, vm);
#ifdef USE_CLR
		for each(lisp_ptr pExp in pIf->getElseIfTbl()) {
#else
		for (lisp_ptr pExp : pIf->getElseIfTbl()) {
#endif
			_SIZE_T curCmdLen = REF_ACCESS(vm,getCmdBufLen)();
			REF_ACCESS(vm,setJmpPos)(jmpCmdPos, curCmdLen);
			jmpCmdPos = parseElseIf(pExp, vm);
		}
		_SIZE_T curCmdLen = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,setJmpPos)(jmpCmdPos, curCmdLen);
#ifdef USE_CLR
		for each(lisp_ptr pExp in pIf->getElseTbl()) {
#else
		for (lisp_ptr pExp : pIf->getElseTbl()) {
#endif


			parseChunck(pExp, vm);
		}

	}
	_SIZE_T CLisp2ByteCode::parseElseIf(lisp_ptr p, REF(CVM) vm) {
		ASSERT(p->getType() == CLispIf::s_elseIfType);
#ifdef USE_CLR
		CLispElseIf^ pEIf = safe_cast<CLispElseIf^>(p);
#else
		CLispElseIf* pEIf = reinterpret_cast<CLispElseIf*>(p.get());
#endif
		parseValue(pEIf->getTestCondExp(), vm);
		REF_ACCESS(vm,addCmpCmd)();
		_SIZE_T jmpCmdPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,addJmpNCmd)(-1);
		parseChunck(p, vm);
		return jmpCmdPos;
	}
	void CLisp2ByteCode::parseDefunc(lisp_ptr p, REF(CVM) vm) {
		ASSERT(p->getType() == CLispDefunc::s_selfType);
		_SIZE_T fEndJmpPos = REF_ACCESS(vm,getCmdBufLen)();
		REF_ACCESS(vm,addJmpCmd)(0);
#ifdef USE_CLR
		CLispDefunc^ pDefunc = safe_cast<CLispDefunc^>(p);
		m_defuncTbl[pDefunc->getName()]=vm->getCmdBufLen();
#else
		CLispDefunc* pDefunc = reinterpret_cast<CLispDefunc*>(p.get());
		m_defuncTbl.insert(DefuncPosTbl::value_type(pDefunc->getName(), vm.getCmdBufLen()));
#endif
		//vm.addThisCmd(p);

		parseChunck(p, vm);

		REF_ACCESS(vm,addRetCmd)(
#ifdef USE_CLR
			(safe_cast<CLispArray^>(pDefunc->getArgs()))->value->Count
#else
			(reinterpret_cast<CLispArray*>(pDefunc->getArgs().get()))->value().size()
			
#endif	
		);

		REF_ACCESS(vm,setJmpPos)(fEndJmpPos, REF_ACCESS(vm,getCmdBufLen)());
	}
};