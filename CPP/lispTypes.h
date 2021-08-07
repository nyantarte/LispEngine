#pragma once

class CLispObject;
typedef std::shared_ptr<CLispObject> lisp_ptr;
typedef std::map < std::string, lisp_ptr> MemberTbl;
class CLispObject {
	lisp_ptr m_type;
	MemberTbl m_members;
public:
	
	CLispObject(lisp_ptr type)
		:m_type(type)
	{}

	virtual ~CLispObject() {}
	lisp_ptr getType() {
		return m_type;
	}
	lisp_ptr find(std::string name);
	void regist(std::string name,lisp_ptr p);

};
class CLispType :public CLispObject {
	std::string m_typeName;
public:
	CLispType(std::string name)
		:CLispObject(nullptr),
		m_typeName(name)
	{}
};
template<class T>
class CLispPrimitive :public CLispObject {
	T m_value;
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

class CLispInt :public CLispPrimitive<int> {
public:
	static lisp_ptr s_selfType;
public:
	CLispInt(int v)
		:CLispPrimitive(v, s_selfType)
	{}

	CLispInt(std::string v)
		:CLispPrimitive(atoi(v.c_str()), s_selfType)
	{}
};

class CLispString :public CLispPrimitive<std::string> {
public:
	static lisp_ptr s_selfType;
public:
	CLispString(std::string v)
		:CLispPrimitive(v, s_selfType)
	{}


};
class CLispArray :public CLispPrimitive<std::vector<lisp_ptr>> {
public:
	static lisp_ptr s_selfType;
public:
	CLispArray()
		:CLispPrimitive( s_selfType)
	{}

};


typedef void (*NativeFunc)(void* );
class CLispNativeFunc :public CLispObject {
	NativeFunc m_func;

public:
	static lisp_ptr s_selfType;

public:
	NativeFunc getFunc() {
		return m_func;
	}
};

class CLispFuncCall :public CLispObject {
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
class CLispLet :public CLispObject {
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
class CLispChunck :public CLispObject {
	ImplementsTbl m_implements;
	int m_varNum;
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