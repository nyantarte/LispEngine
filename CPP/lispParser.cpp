#include"pch.h"
#include"lispParser.h"
#include"lispErrors.h"
lisp_ptr CLispParser::s_rootType(new CLispType("root"));

CLispParser::CLispParser() 

{
	CLispChunck* pChunck = new CLispChunck(s_rootType);
	m_root.reset(pChunck);
	pChunck->regist("print", lisp_ptr(new CLispNativeFunc(&print)));
	pChunck->regist("range", lisp_ptr(new CLispNativeFunc(&createRange)));
	push(m_root);

}
void CLispParser::parse(CLispLexer& l) {
	CLispChunck* pChunck = reinterpret_cast<CLispChunck*>(m_root.get());
	do {
		lisp_ptr p=parseCall(l);
		pChunck->getImplements().push_back(p);
	} while (!l.isEnd());
}

lisp_ptr CLispParser::parseValue(CLispLexer& l) {
	std::string tok = l.getCurrent();
	if ("(" == tok) {
		return parseCall(l);

	}
	else if (isdigit(tok[0])) {
		l.getNext();
		return lisp_ptr(new CLispInt(tok));
	}
	else if ('\"' == tok[0]) {
		l.getNext();
		return lisp_ptr(new CLispString(std::string(tok.begin() + 1, tok.end() - 2)));
	}
	else if (isalpha(tok[0])) {
		int popNum;
		lisp_ptr var = find(tok,popNum);
		l.getNext();
		return lisp_ptr(new CLispVarAccess(var,popNum));
	}

	throw CLispInvalidToken();
}

lisp_ptr CLispParser::parseCall(CLispLexer& l){
	assert("(" == l.getCurrent());
	std::string fName = l.getNext();
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
	}else {
		int popNum = 0;
		lisp_ptr f = find(fName,popNum);

		if (nullptr == f)
			throw CLispUndefinedIdent();

		l.getNext();
		lisp_ptr args = parseArgs(l);
		CLispFuncCall* pFC = new CLispFuncCall(f, args);
		assert(")" == l.getCurrent());
		l.getNext();
		return lisp_ptr(pFC);
	}

}
lisp_ptr CLispParser::parseArgs(CLispLexer& l) {
	CLispArray* pA = new CLispArray();

	while (")" != l.getCurrent()) {
		
		pA->value().push_back(parseValue(l));

	}
	return lisp_ptr(pA);
}
lisp_ptr CLispParser::parseLet(CLispLexer& l) {
	assert("let" == l.getCurrent());
	std:: string vname = l.getNext();
	int popNum;
	if (nullptr != find(vname,popNum)) {
		throw CLispAlreadyDefined();
	}
	l.getNext();
	
	lisp_ptr pLet = lisp_ptr(new CLispLet(vname, parseValue(l)));
	
	regist(vname, pLet);
	l.getNext();
	CLispChunck* pThis=reinterpret_cast<CLispChunck*>(getTop().get());
	++pThis->varNum();


	return pLet;
}
lisp_ptr CLispParser::parseFor(CLispLexer& l) {
	assert("for" == l.getCurrent());
	l.getNext();
	lisp_ptr range = parseCall(l);
	CLispFor* pF = new CLispFor(range);
	lisp_ptr rc(pF);
	push(rc);
	if (CLispRange::s_selfType != range->getType()) {
		throw CLispNotRange();
	}
	l.getNext();
	while (")" != l.getCurrent()) {
		pF->getImplements().push_back(parseCall(l));
	}
	l.getNext();
	pop();

	return rc;
}
lisp_ptr CLispParser::parseWhile(CLispLexer& l) {
	assert("while" == l.getCurrent());
	l.getNext();
	CLispWhile* pW = new CLispWhile(parseCall(l));
	lisp_ptr rc(pW);
	push(rc);
	if ("(" != l.getCurrent()) {
		throw CLispNeedsToken("(");
	}
	l.getNext();
	while (")" != l.getCurrent()) {
		pW->getImplements().push_back(parseCall(l));
	}
	l.getNext();
	l.getNext();
	pop();

	return rc;
}
lisp_ptr CLispParser::parseIf(CLispLexer& l) {
	assert("if" == l.getCurrent());
	l.getNext();
	CLispIf* pW = new CLispIf(parseCall(l));
	lisp_ptr rc(pW);
	push(rc);
	if (")" != l.getCurrent()) {
		std::string::iterator pos = l.getPos();
		std::string tok = l.getNext();
		if ("elseif" != tok && "else" != tok) {

			while (")" != l.getCurrent()) {
				lisp_ptr p = parseCall(l);

				pW->getImplements().push_back(p);
			}
			if ("(" == l.getNext()) {
				pos = l.getPos();
				l.getNext();
			}
		}

		if ("elseif" != tok) {
			l.setPos(pos);
			while (")" != l.getCurrent()) {
				lisp_ptr p = parseCall(l);

				pW->getElseIfTbl().push_back(p);
			}
			if ("(" == l.getNext()) {
				pos = l.getPos();
				l.getNext();
			}

		}

		if ("else" != tok) {
			l.setPos(pos);
			while (")" != l.getCurrent()) {
				lisp_ptr p = parseCall(l);

				pW->getElseIfTbl().push_back(p);
			}
			l.getNext();

		}

	}
	if (")" != l.getCurrent()) {
		throw CLispNeedsToken(")");
	}
	pop();
	l.getNext();
	return rc;
}
lisp_ptr  CLispParser::parseElseIf(CLispLexer& l) {
	assert("elseif" == l.getCurrent());
	if (CLispIf::s_selfType != getTop()->getType()) {
		throw CLispElseNotInIf();
	}
	l.getNext();
	lisp_ptr pCondExp = parseCall(l);
	CLispElseIf* pElseIf = new CLispElseIf(pCondExp,CLispIf::s_elseIfType);
	
	lisp_ptr rc(pElseIf);
	push(rc);
	while (")" != l.getCurrent()) {
		pElseIf->getImplements().push_back(parseCall(l));
	}
	l.getNext();
	pop();
	return rc;
}
lisp_ptr  CLispParser::parseElse(CLispLexer& l) {
	assert("elseif" == l.getCurrent());
	if (CLispIf::s_selfType != getTop()->getType()) {
		throw CLispElseNotInIf();
	}
	l.getNext();
	CLispElse* pElse = new CLispElse(CLispChunck::s_elseType);

	lisp_ptr rc(pElse);
	push(rc);
	while (")" != l.getCurrent()) {
		pElse->getImplements().push_back(parseCall(l));
	}
	l.getNext();
	pop();
	return rc;
	
}
lisp_ptr CLispParser::parseDefunc(CLispLexer& l) {
	assert("defn" == l.getCurrent());

	std::string fname = l.getNext();
	int popNum=0;
	if (nullptr != find(fname,popNum)) {
		throw CLispAlreadyDefined();
	}
	l.getNext();
	l.getNext();
	lisp_ptr args = parseArgs(l);
	CLispDefunc* pDefunc = new CLispDefunc(fname,args);
	lisp_ptr ret(pDefunc);
	push(ret);
	l.getNext();
	l.getNext();

	while (")" != l.getCurrent()) {
		pDefunc->getImplements().push_back(parseCall(l));
	}
	l.getNext();
	l.getNext();
	pop();
	regist(fname, ret);
	return ret;
}
lisp_ptr CLispParser::parseInclude(CLispLexer& l) {
	assert("include" == l.getCurrent());
	l.getNext();
	lisp_ptr liname = parseValue(l);
	if (CLispString::s_selfType != liname) {
		throw CLispIncludeParameterMustBeString();
	}
	std::string iname = (reinterpret_cast<CLispString*>(liname.get()))->value();
	CLispLexer il;
	genLexerFromFile(il, iname);
	parse(l);
	
	std::string tok=l.getNext();
	if (")" != tok) {
		throw  CLispNeedsToken(")");
	}
	l.getNext();
	return lisp_ptr(new CLispVoid());
}
lisp_ptr CLispParser::find(std::string name, int& popNum) {
	popNum = 0;
	if (0 < m_nameStack.size()) {
		for (NameStack::size_type i = m_nameStack.size() ; i > 0; --i) {
			
			lisp_ptr o = m_nameStack[i-1]->find(name);
			if (nullptr != o)
				return o;

			++popNum;
		}
	}
	popNum = -1;
	return nullptr;
}
void CLispParser::regist(std::string name, lisp_ptr p) {
	assert(0 < m_nameStack.size());
	(m_nameStack.end() - 1)->get()->regist(name, p);

}

void CLispParser::genLexerFromFile(CLispLexer& l, std::string fname) {

	std::ifstream in(fname.c_str());
	std::string buf;
	std::string line;
	while (std::getline(in, line)) {
		buf += line;
		buf += "\n";
	}

	l.reset(buf);
}
