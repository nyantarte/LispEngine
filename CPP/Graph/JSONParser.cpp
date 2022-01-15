#include"JSONParser.h"
#include"GraphError.h"
#include"NodeFactory.h"
void CJSONParser::parse(std::string src) {
	CJSONLexer lexer(src);
	m_root=parseChunck(lexer);

}
json_node_ptr CJSONParser::parseChunck(CJSONLexer& l) {
	if ("{" != l.getCurrent()) {
		throw CJSONInvalidToken();

	}
	CJSONChunck* pChunck = new CJSONChunck();
	json_node_ptr r(pChunck);
	l.getNext();
	while ("}" != l.getCurrent()) {

		parseElement(pChunck, l);

	}

	l.getNext();

	return r;
}

void CJSONParser::parseElement(CJSONChunck* p, CJSONLexer& l) {
	std::string eleName = l.getCurrent();
	if ('\"' != eleName[0]) {
		throw CJSONInvalidToken();	
	}

	if (":" != l.getNext()) {
		throw CJSONInvalidToken();
	}
	l.getNext();
	json_node_ptr value = parseValue(l);
	p->setElement(eleName.substr(1), value);
	

}

json_node_ptr CJSONParser::parseValue(CJSONLexer& l) {
	std::string v = l.getCurrent();
	if ("{" == v) {
		return parseChunck(l);
	}else if("[" == v) {
		return parseArray(l);
	}
	else if ("true" == v) {
		l.getNext();
		return json_node_ptr(new CJSONBool(true));
	}
	else if ("false" == v) {
		l.getNext();
		return json_node_ptr(new CJSONBool(false));

	}
	else if ('\"' == v[0]) {
		l.getNext();
		v = v.substr(1);

		return json_node_ptr(new CJSONValue<std::string>(v));

	
	}
		std::stringstream ss(v);
		int nValue;
		ss >> nValue;
		l.getNext();
		return json_node_ptr(new CJSONValue<int>(nValue));

}
json_node_ptr CJSONParser::parseArray(CJSONLexer& l) {
	assert("[" == l.getCurrent());
	CJSONArray* pArray = new CJSONArray();
	json_node_ptr r(pArray);
	
	l.getNext();
	while ("]" != l.getCurrent()) {
		pArray->value().push_back(parseValue(l));
		if ("," == l.getCurrent()) {
			l.getNext();
		}
	}
	l.getNext();
	return r;
}