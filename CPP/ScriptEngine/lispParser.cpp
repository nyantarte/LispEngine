#ifndef USE_CRL
#include<cassert>
#include<fstream>
#endif

#include"lispParser.h"
#include"lispErrors.h"
namespace LispScriptEngine {
#ifndef USE_CLR
	lisp_ptr CLispParser::s_rootType(new CLispType("root"));
#endif
	CLispParser::CLispParser()

	{
		PTR(CLispChunck) pChunck = NEW CLispChunck(s_rootType);
#ifdef USE_CLR
		m_root = pChunck;
#else
		m_root.reset(pChunck);
#endif
		pChunck->regist("print", lisp_ptr(NEW CLispNativeFunc(
#ifdef USE_CLR
			gcnew NativeFunc(print)
#else
			&print
#endif
		)));
		pChunck->regist("range", lisp_ptr(NEW CLispNativeFunc(
#ifdef USE_CLR
			gcnew NativeFunc(createRange)
#else
			&createRange
#endif		
		
		)));
		push(m_root);

	}
	void CLispParser::parse(REF(CLispLexer) l) {
		PTR(CLispChunck) pChunck =
#ifdef USE_CLR
			safe_cast<CLispChunck^>(m_root);
#else
			reinterpret_cast<CLispChunck*>(m_root.get());
#endif
		do {
			lisp_ptr p = parseCall(l);
#ifdef USE_CLR
			pChunck->getImplements()->Add(p);
#else
			pChunck->getImplements().push_back(p);
#endif

		} while (!REF_ACCESS(l,isEnd)());
	}

	lisp_ptr CLispParser::parseValue(REF(CLispLexer) l) {
		STRING tok = REF_ACCESS(l,getCurrent)();
		if ("(" == tok) {
			return parseCall(l);

		}
		else if (isdigit(tok[0])) {
			REF_ACCESS(l,getNext)();
			return lisp_ptr(NEW CLispInt(tok));
		}
		else if ('\"' == tok[0]) {
			REF_ACCESS(l,getNext)();
			return lisp_ptr(NEW CLispString(
#ifdef USE_CLR
				gcnew String(tok[1],tok[tok->Length-2])
#else
				std::string(tok.begin() + 1, tok.end() )
#endif
			));
		}
		else if (
#ifdef USE_CLR
			Char::IsLetter(tok[0])
#else
			isalpha(tok[0])
#endif
			) {
			int popNum;
			lisp_ptr var = find(tok, popNum);
			REF_ACCESS(l,getNext)();
			return lisp_ptr(NEW CLispVarAccess(var, popNum));
		}

#ifdef USE_CLR
		throw gcnew CLispInvalidToken();
#else
		throw CLispInvalidToken();
#endif
	}

	lisp_ptr CLispParser::parseCall(REF(CLispLexer) l) {
		ASSERT("(" == REF_ACCESS(l,getCurrent)());
		STRING fName = REF_ACCESS(l,getNext)();
		if ("let" == fName) {
			return parseLet(l);
		}
		else if ("for" == fName) {
			return parseFor(l);
		}
		else if ("while" == fName) {
			return parseWhile(l);
		}
		else if ("if" == fName) {
			return parseIf(l);
		}
		else if ("elseif" == fName) {
			return parseElseIf(l);
		}
		else if ("else" == fName) {
			return parseElse(l);
		}
		else if ("defn" == fName) {
			return parseDefunc(l);
		}
		else if ("include" == fName) {
			return parseInclude(l);
		}
		else {
			int popNum = 0;
			lisp_ptr f = find(fName, popNum);

			if (nullptr == f)
#ifdef USE_CLR
				throw gcnew CLispUndefinedIdent();
#else
				throw CLispUndefinedIdent();
#endif
			REF_ACCESS(l,getNext)();
			lisp_ptr args = parseArgs(l);
			PTR(CLispFuncCall) pFC = NEW CLispFuncCall(f, args);
			ASSERT(")" == REF_ACCESS(l,getCurrent)());
			REF_ACCESS(l,getNext)();
			return lisp_ptr(pFC);
		}

	}
	lisp_ptr CLispParser::parseArgs(REF(CLispLexer) l) {
		PTR(CLispArray) pA = NEW CLispArray();

		while (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			pA->value->Add(parseValue(l));
#else
			pA->value().push_back(parseValue(l));
#endif
		}
		return lisp_ptr(pA);
	}
	lisp_ptr CLispParser::parseLet(REF(CLispLexer) l) {
		ASSERT("let" == REF_ACCESS(l,getCurrent)());
		STRING vname = REF_ACCESS(l,getNext)();
		int popNum;
		if (nullptr != find(vname, popNum)) {
#ifdef USE_CLR
			throw gcnew CLispAlreadyDefined();
#else
			throw CLispAlreadyDefined();
#endif
		}
		REF_ACCESS(l,getNext)();

		lisp_ptr pOwner = getTop();


#ifdef USE_CLR
		PTR(CLispChunck) pThis = safe_cast<CLispChunck^>(getTop());
#else
		PTR(CLispChunck) pThis = reinterpret_cast<CLispChunck*>(getTop().get());
#endif
		lisp_ptr pLet = lisp_ptr(NEW CLispLet(vname, parseValue(l),
			
#ifdef USE_CLR
			pThis->varNum
#else
			pThis->varNum()
#endif
		));

#ifdef USE_CLR
		++pThis->varNum;
#else
		++pThis->varNum();
#endif
		regist(vname, pLet);
		REF_ACCESS(l,getNext)();
		




		return pLet;
	}
	lisp_ptr CLispParser::parseFor(REF(CLispLexer) l) {
		ASSERT("for" == REF_ACCESS(l,getCurrent)());
		REF_ACCESS(l,getNext)();
		lisp_ptr range = parseCall(l);
		PTR(CLispFor) pF = NEW CLispFor(range);
		lisp_ptr rc(pF);
		push(rc);
		if (CLispRange::s_selfType != range->getType()) {
#ifdef USE_CLR
			throw gcnew CLispNotRange();
#else
			throw CLispNotRange();
#endif
		}
		REF_ACCESS(l,getNext)();
		while (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			pF->getImplements()->Add(parseCall(l));
#else
			pF->getImplements().push_back(parseCall(l));
#endif
		}
		REF_ACCESS(l,getNext)();
		pop();

		return rc;
	}
	lisp_ptr CLispParser::parseWhile(REF(CLispLexer) l) {
		ASSERT("while" == REF_ACCESS(l,getCurrent)());
		REF_ACCESS(l,getNext)();
		PTR(CLispWhile) pW = NEW CLispWhile(parseCall(l));
		lisp_ptr rc(pW);
		push(rc);
		if ("(" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			throw gcnew CLispNeedsToken("(");
#else
			throw CLispNeedsToken("(");
#endif
		}
		REF_ACCESS(l,getNext)();
		while (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			pW->getImplements()->Add(parseCall(l));
#else
			pW->getImplements().push_back(parseCall(l));
#endif
		}
		REF_ACCESS(l,getNext)();
		REF_ACCESS(l,getNext)();
		pop();

		return rc;
	}
	lisp_ptr CLispParser::parseIf(REF(CLispLexer) l) {
		ASSERT("if" == REF_ACCESS(l,getCurrent)());
		REF_ACCESS(l,getNext)();
		PTR(CLispIf) pW = NEW CLispIf(parseCall(l));
		lisp_ptr rc(pW);
		push(rc);
		if (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			_SIZE_T pos = l->getPos();
#else
			std::string::iterator pos = l.getPos();
#endif
			STRING tok = REF_ACCESS(l,getNext)();
			if ("elseif" != tok && "else" != tok) {

				while (")" != REF_ACCESS(l,getCurrent)()) {
					lisp_ptr p = parseCall(l);
#ifdef USE_CLR
					pW->getImplements()->Add(p);
#else
					pW->getImplements().push_back(p);
#endif
				}
				if ("(" == REF_ACCESS(l,getNext)()) {
					pos = REF_ACCESS(l,getPos)();
					REF_ACCESS(l,getNext)();
				}
			}

			if ("elseif" != tok) {
				REF_ACCESS(l,setPos)(pos);
				while (")" != REF_ACCESS(l,getCurrent)()) {
					lisp_ptr p = parseCall(l);
#ifdef USE_CLR
					pW->getElseIfTbl()->Add(p);
#else
					pW->getElseIfTbl().push_back(p);
#endif
				}
				if ("(" == REF_ACCESS(l,getNext)()) {
					pos = REF_ACCESS(l,getPos)();
					REF_ACCESS(l,getNext)();
				}

			}

			if ("else" != tok) {
				REF_ACCESS(l,setPos)(pos);
				while (")" != REF_ACCESS(l,getCurrent)()) {
					lisp_ptr p = parseCall(l);
#ifdef USE_CLR
					pW->getElseIfTbl()->Add(p);
#else
					pW->getElseIfTbl().push_back(p);
#endif
				}
				REF_ACCESS(l,getNext)();

			}

		}
		if (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			throw gcnew CLispNeedsToken(")");
#else
			throw CLispNeedsToken(")");
#endif
		}
		pop();
		REF_ACCESS(l,getNext)();
		return rc;
	}
	lisp_ptr  CLispParser::parseElseIf(REF(CLispLexer) l) {
		ASSERT("elseif" == REF_ACCESS(l,getCurrent)());
		if (CLispIf::s_selfType != getTop()->getType()) {
#ifdef USE_CLR
			throw gcnew CLispElseNotInIf();
#else
			throw CLispElseNotInIf();
#endif
		}
		REF_ACCESS(l,getNext)();
		lisp_ptr pCondExp = parseCall(l);
		PTR(CLispElseIf) pElseIf = NEW CLispElseIf(pCondExp, CLispIf::s_elseIfType);

		lisp_ptr rc(pElseIf);
		push(rc);
		while (")" != REF_ACCESS(l,getCurrent())) {
#ifdef USE_CLR
			pElseIf->getImplements()->Add(parseCall(l));
#else
			pElseIf->getImplements().push_back(parseCall(l));
#endif
		}
		REF_ACCESS(l,getNext)();
		pop();
		return rc;
	}
	lisp_ptr  CLispParser::parseElse(REF(CLispLexer) l) {
		ASSERT("elseif" == REF_ACCESS(l,getCurrent)());
		if (CLispIf::s_selfType != getTop()->getType()) {
#ifdef USE_CLR
			throw gcnew CLispElseNotInIf();
#else
			throw CLispElseNotInIf();
#endif
		}
		REF_ACCESS(l,getNext)();
		PTR(CLispElse) pElse = NEW CLispElse(CLispChunck::s_elseType);

		lisp_ptr rc(pElse);
		push(rc);
		while (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			pElse->getImplements()->Add(parseCall(l));
#else
			pElse->getImplements().push_back(parseCall(l));
#endif
		}
		REF_ACCESS(l,getNext)();
		pop();
		return rc;

	}
	lisp_ptr CLispParser::parseDefunc(REF(CLispLexer) l) {
		ASSERT("defn" == REF_ACCESS(l,getCurrent)());

		STRING fname = REF_ACCESS(l,getNext)();
		int popNum = 0;
		if (nullptr != find(fname, popNum)) {
#ifdef USE_CLR
			throw gcnew CLispAlreadyDefined();
#else
			throw CLispAlreadyDefined();
#endif
		}
		REF_ACCESS(l,getNext)();
		REF_ACCESS(l,getNext)();
		lisp_ptr args = parseArgs(l);
		PTR(CLispDefunc) pDefunc = NEW CLispDefunc(fname, args);
		lisp_ptr ret(pDefunc);
		push(ret);
		REF_ACCESS(l,getNext)();
		REF_ACCESS(l,getNext)();

		while (")" != REF_ACCESS(l,getCurrent)()) {
#ifdef USE_CLR
			pDefunc->getImplements()->Add(parseCall(l));
#else
			pDefunc->getImplements().push_back(parseCall(l));
#endif
		}
		REF_ACCESS(l,getNext)();
		REF_ACCESS(l,getNext)();
		pop();
		regist(fname, ret);
		return ret;
	}
	lisp_ptr CLispParser::parseInclude(REF(CLispLexer) l) {
		ASSERT("include" == REF_ACCESS(l,getCurrent)());
		REF_ACCESS(l,getNext)();
		lisp_ptr liname = parseValue(l);
		if (CLispString::s_selfType != liname) {
#ifdef USE_CLR
			throw gcnew CLispIncludeParameterMustBeString();
#else
			throw CLispIncludeParameterMustBeString();
#endif
		}
		STRING iname =
#ifdef USE_CLR
		(safe_cast<CLispString^>(liname))->value;
#else
			(reinterpret_cast<CLispString*>(liname.get()))->value();
#endif
		CLASS(CLispLexer,il);
		genLexerFromFile(il, iname);
		parse(l);

		STRING tok = REF_ACCESS(l,getNext)();
		if (")" != tok) {
#ifdef USE_CLR
			throw  gcnew CLispNeedsToken(")");
#else
			throw  CLispNeedsToken(")");
#endif
		}
		REF_ACCESS(l,getNext)();
		return lisp_ptr(NEW CLispVoid());
	}
	lisp_ptr CLispParser::find(STRING name,
#ifdef USE_CLR
		int% popNum
#else
		int& popNum
#endif
		) {
		popNum = 0;
		if (0 < 
#ifdef USE_CLR
			m_nameStack->Count
#else
			m_nameStack.size()
#endif			
			) {
			for (_SIZE_T i = 
#ifdef USE_CLR
				m_nameStack->Count;
#else
				m_nameStack.size();
#endif				
				i > 0; --i) {

				lisp_ptr o = m_nameStack[i - 1]->find(name);
				if (nullptr != o)
					return o;

				++popNum;
			}
		}
		popNum = -1;
		return nullptr;
	}
	void CLispParser::regist(STRING name, lisp_ptr p) {
		
#ifdef USE_CLR
		Debug::Assert(0 < m_nameStack->Count);
#else
		assert(0 < m_nameStack.size());
#endif
#ifdef USE_CLR
		(m_nameStack[m_nameStack->Count - 1])->regist(name, p);
#else
		(m_nameStack.end() - 1)->get()->regist(name, p);
#endif
	}

	void CLispParser::genLexerFromFile(REF(CLispLexer) l, STRING fname) {
#ifdef USE_CLR
		System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(fname);
		String^ buf = sr->ReadToEnd();
		sr->Close();
#else
		std::ifstream in(fname.c_str());
		std::string buf;
		std::string line;
		while (std::getline(in, line)) {
			buf += line;
			buf += "\n";
		}
#endif
		REF_ACCESS(l,reset)(buf);

	}
};