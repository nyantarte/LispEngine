#include"pch.h"
#include"lispParser.h"
#include"lispErrors.h"

void CLispParser::parse(CLispLexer& l) {
	do {
		parseCall(l);
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

	throw CLispInvalidToken();
}

lisp_ptr CLispParser::parseCall(CLispLexer& l){
	assert("(" == l.getCurrent());
	std::string fName = l.getNext();
	if ("let" == fName) {
		return parseLet(l);
	}
	else {
		lisp_ptr f = find(fName);

		if (nullptr == f)
			throw CLispUndefinedIdent();

		l.getNext();
		lisp_ptr args = parseArgs(l);
		CLispFuncCall* pFC = new CLispFuncCall(f, args);
		assert(")" == l.getCurrent());
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
	if (nullptr != find(vname)) {
		throw CLispAlreadyDefined();
	}

	
	lisp_ptr pLet = lisp_ptr(new CLispLet(vname, parseValue(l)));
	
	regist(vname, pLet);
	return pLet;
}
lisp_ptr CLispParser::find(std::string name) {
	if (0 < m_nameStack.size()) {
		for (NameStack::iterator it = m_nameStack.end() - 1; it >= m_nameStack.begin(); ++it) {
			lisp_ptr o = it->get()->find(name);
			if (nullptr != o)
				return o;
		}
	}
	return nullptr;
}
void CLispParser::regist(std::string name, lisp_ptr p) {
	assert(0 < m_nameStack.size());
	(m_nameStack.end() - 1)->get()->regist(name, p);

}
