#pragma once
#include"framework.h"
#include"export.h"

class CLispObject;
typedef std::shared_ptr<CLispObject> lisp_ptr;
typedef std::map < std::string, lisp_ptr> MemberTbl;
class LISP_SCRIPT_API CLispObject {
	lisp_ptr m_type;
	MemberTbl m_members;
public:
	
	CLispObject(lisp_ptr type)
		:m_type(type)
	{}

	virtual ~CLispObject() {}
	virtual lisp_ptr getType() {
		return m_type;
	}
	lisp_ptr find(std::string name);
	void regist(std::string name,lisp_ptr p);
	virtual std::string toString() { return ""; }
protected:
	void setType(lisp_ptr t) {
		m_type = t;
	}
};
class LISP_SCRIPT_API CLispType :public CLispObject {
	std::string m_typeName;
public:
	CLispType(std::string name)
		:CLispObject(nullptr),
		m_typeName(name)
	{}
	std::string getTypeName() {
		return m_typeName;
	}

};
class LISP_SCRIPT_API CLispVoid :public CLispObject {
public:
	static lisp_ptr s_selfType;
public:
	CLispVoid()
		:CLispObject(s_selfType)
	{}
};
template<class T>
class /*LISP_SCRIPT_API*/ CLispPrimitive :public CLispObject {
	T m_value;
public:
	typedef T value_type;
public:
	CLispPrimitive(T v, lisp_ptr type)
		:CLispObject(type),
		 m_value(v)
	{}
	CLispPrimitive(lisp_ptr type)
		:CLispObject(type)
	{}
	T& value() {
		return m_value;
	}

};

class LISP_SCRIPT_API CLispInt :public CLispPrimitive<int> {
public:
	static lisp_ptr s_selfType;
public:
	CLispInt(int v)
		:CLispPrimitive(v, s_selfType)
	{}

	CLispInt(std::string v)
		:CLispPrimitive(atoi(v.c_str()), s_selfType)
	{}

	virtual std::string toString() {
		std::stringstream ss;
		ss << value();
		return ss.str();
	}
};

class LISP_SCRIPT_API CLispBool:public CLispPrimitive<bool> {
public:
	static lisp_ptr s_selfType;
public:
	CLispBool(bool v)
		:CLispPrimitive(v, s_selfType)
	{}
	virtual std::string toString() {
		std::stringstream ss;
		ss << value();
		return ss.str();
	}
};

class LISP_SCRIPT_API CLispString :public CLispPrimitive<std::string> {
public:
	static lisp_ptr s_selfType;
public:
	CLispString(std::string v)
		:CLispPrimitive(v, s_selfType)
	{}

	virtual std::string toString() {
		return value();
	}
};
class LISP_SCRIPT_API CLispRange :public CLispPrimitive<std::pair<int, int> > {
public:
	static lisp_ptr s_selfType;
public:
	CLispRange(int b, int e)
		:CLispPrimitive(value_type(b, e), s_selfType)
	{}

	virtual std::string toString() {
		std::stringstream ss;
		ss << "[" << value().first << "," << value().second << "]";
		return ss.str();
	}
};
class LISP_SCRIPT_API CLispArray :public CLispPrimitive<std::vector<lisp_ptr>> {
public:
	static lisp_ptr s_selfType;
public:
	CLispArray()
		:CLispPrimitive( s_selfType)
	{}
	virtual std::string toString() {
		std::stringstream ss;
		ss << "[";
		for (value_type::size_type i = 0; i < value().size(); ++i) {
			lisp_ptr item = value()[i];
			ss << item->toString();
			if (i < (value().size() - 1)) {
				ss<<",";
			}
		}
		ss<<"]";
		return ss.str();
	}
};


typedef void (*NativeFunc)(int,void* );
class LISP_SCRIPT_API CLispNativeFunc :public CLispObject {
	NativeFunc m_func;

public:
	static lisp_ptr s_selfType;

public:
	CLispNativeFunc(NativeFunc f)
		:CLispObject(s_selfType),
		 m_func(f)
	{}
	NativeFunc getFunc() {
		return m_func;
	}
};

class LISP_SCRIPT_API CLispFuncCall :public CLispObject {
	lisp_ptr m_func;
	lisp_ptr m_args;
public:
	static lisp_ptr s_selfType;
public:
	CLispFuncCall(lisp_ptr func,lisp_ptr args)
		:CLispObject(s_selfType),
		m_func(func),
		m_args(args)
	{}

	lisp_ptr getFunc() {
		return m_func;
	}
	lisp_ptr getArgs() {
		return m_args;
	}

};
class LISP_SCRIPT_API CLispLet :public CLispObject {
	std::string m_name;
	lisp_ptr m_value;
	std::size_t m_idx;
public:
	static lisp_ptr s_selfType;

public:
	CLispLet(std::string name, lisp_ptr v)
		:CLispObject(s_selfType),
		m_name(name),
		m_value(v),
		m_idx(-1)
	{}

	std::string getName() {
		return m_name;
	}

	lisp_ptr getValue() {
		return m_value;
	}

	std::size_t& index() {
		return m_idx;
	}

};
typedef std::vector<lisp_ptr> ImplementsTbl;
class LISP_SCRIPT_API CLispChunck :public CLispObject {
	ImplementsTbl m_implements;
	int m_varNum;
public:
	static lisp_ptr s_elseType;
public:
	CLispChunck(lisp_ptr type)
		:CLispObject(type),
		m_varNum(0)
	{}

	ImplementsTbl& getImplements() {
		return m_implements;
	}

	int& varNum() {
		return m_varNum;
	}
};
class LISP_SCRIPT_API CLispDefunc :public CLispChunck {
	lisp_ptr m_args;
	std::string m_name;
public:
	static lisp_ptr s_selfType;
public:
	CLispDefunc(std::string name,lisp_ptr args)
		:CLispChunck(s_selfType),
		 m_args(args),
		 m_name(name)
	{
	}

	std::string getName() {
		return m_name;
	}
	lisp_ptr getArgs() {
		return m_args;
	}


};
class LISP_SCRIPT_API CLispFor :public CLispChunck {
	lisp_ptr m_rangeExp;
public:
	static lisp_ptr s_selfType;
public:
	CLispFor(lisp_ptr range)
		:CLispChunck(s_selfType),
		 m_rangeExp(range)
	{}

	lisp_ptr getRange() {
		return m_rangeExp;
	}

};
class LISP_SCRIPT_API CLispWhile :public CLispChunck {
	lisp_ptr m_contCondExp;
public:
	static lisp_ptr s_selfType;
public:
	CLispWhile(lisp_ptr contCondExp)
		:CLispChunck(s_selfType),
		 m_contCondExp(contCondExp)
	{}
	lisp_ptr getContCondExp() {
		return m_contCondExp;
	}
};

class LISP_SCRIPT_API CLispIf :public CLispChunck {
	lisp_ptr m_testCondExp;
	ImplementsTbl m_elseIfTbl;
	ImplementsTbl m_elseTbl;
public:
	static lisp_ptr s_selfType;
	static lisp_ptr s_elseIfType;

public:
	CLispIf(lisp_ptr testCondExp)
		:CLispChunck(s_selfType),
		 m_testCondExp(testCondExp)
	{}
	CLispIf(lisp_ptr testCondExp,lisp_ptr type)
		:CLispChunck(type),
		 m_testCondExp(testCondExp)
	{}
	

	lisp_ptr getTestCondExp() {
		return m_testCondExp;
	}
	ImplementsTbl getElseIfTbl() {
		return m_elseIfTbl;
	}
	ImplementsTbl getElseTbl() {
		return m_elseTbl;
	}

};
typedef CLispIf CLispElseIf;
typedef CLispChunck CLispElse;
typedef std::vector<lisp_ptr> VariableTbl;
class LISP_SCRIPT_API CLispClass :public CLispObject {
	VariableTbl m_variables;
	lisp_ptr m_parent;
public:
	CLispClass(lisp_ptr type,int vNum,lisp_ptr parent)
		:CLispObject(type),
		 m_parent(parent)
	{
		m_variables.resize(vNum);
	}
	VariableTbl& getVariables() {
		return m_variables;
	}
	lisp_ptr getParent() {
		return m_parent;
	}
};

class LISP_SCRIPT_API CLispVarAccess :public CLispObject {
	int m_popNum;
	lisp_ptr m_value;
public:
	static lisp_ptr s_selfType;
public:
	CLispVarAccess(lisp_ptr value, int popNum)
		:CLispObject(s_selfType),
		 m_popNum(popNum),
 		 m_value(value)
	{}
	int getPopNum() {
		return m_popNum;
	}
	lisp_ptr getValue() {
		return m_value;
	}
};
extern"C" {
	void print(int,void*);
	void createRange(int, void*);
}