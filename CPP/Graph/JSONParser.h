#pragma once
#include"node_types.h"
#include"JSONLexer.h"
#include"GraphError.h"
class CJSONNode {
public:
	virtual ~CJSONNode() {}

	template<class T>
	T* castTo() {
		T* p=dynamic_cast<T*>(this);
		if (nullptr == p) {
			throw CJSONInvalidType();
		}
		return p;

	}
};
typedef std::shared_ptr<CJSONNode> json_node_ptr;



class CJSONChunck :public CJSONNode{
public:
	typedef std::map<std::string, json_node_ptr> ElementTbl;
private:
	ElementTbl m_eleTbl;
public:
	void setElement(std::string name, json_node_ptr v) {
		m_eleTbl.insert(ElementTbl::value_type(name, v));
	}
	json_node_ptr getElement(std::string name) {
		ElementTbl::iterator it = m_eleTbl.find(name);
		if (it == m_eleTbl.end()) {
			return nullptr;
		}
		return it->second;
	}
	
	ElementTbl& getElementTbl() {
		return m_eleTbl;
	}

};
template<class T>
class CJSONValue :public CJSONNode {
	T m_value;
public:
	CJSONValue(T v)
		:m_value(v)
	{}
	CJSONValue() {}
	T& value() {
		return m_value;
	}
};
typedef CJSONValue<int> CJSONInt;
typedef CJSONValue<std::string> CJSONString;
typedef CJSONValue < std::vector<json_node_ptr>> CJSONArray;
typedef CJSONValue<bool> CJSONBool;
template<class T>
inline T castTo(json_node_ptr target, T def) {
	if (nullptr != target) {
		CJSONValue<T>* lpValue = target->castTo<CJSONValue<T>>();
		return lpValue->value();
	}
	return def;
}

class CJSONParser {

	json_node_ptr m_root;

public:
	void parse(std::string src);
	json_node_ptr getRoot() {
		return m_root;
	}
private:
	json_node_ptr parseChunck(CJSONLexer& l);
	void parseElement(CJSONChunck* p, CJSONLexer& l);
	json_node_ptr parseValue(CJSONLexer& l);
	json_node_ptr parseArray(CJSONLexer& l);

};