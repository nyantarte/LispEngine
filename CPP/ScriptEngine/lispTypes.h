#pragma once
#include"ScriptEngineConfig.h"
#ifdef USE_CLR
using namespace System;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;

#else
#include<vector>
#include<string>
#include<memory>
#include<map>
#include<sstream>
#endif
namespace LispScriptEngine {
#ifdef USE_CLR
	ref class CLispObject;
#else
	class CLispObject;
#endif
#ifdef USE_CLR
	typedef CLispObject^ lisp_ptr;
#else
	typedef std::shared_ptr<CLispObject> lisp_ptr;
#endif
	typedef MAP<STRING, lisp_ptr> MemberTbl;

	DEF_CLASS(CLispObject) {
		lisp_ptr m_type;
		CLASS(MemberTbl, m_members);
	public:

		CLispObject(lisp_ptr type)
			:m_type(type)
		{}

		virtual ~CLispObject() {}
		virtual lisp_ptr getType() {
			return m_type;
		}
		lisp_ptr find(STRING name);
		void regist(STRING name, lisp_ptr p);
		virtual STRING toString() { return ""; }
	protected:
		void setType(lisp_ptr t) {
			m_type = t;
		}
	};

	DERIVE_CLASS(CLispType, CLispObject) {

		STRING m_typeName;
	public:
		CLispType(STRING name)
			:CLispObject(nullptr),
			m_typeName(name)
		{}
		STRING getTypeName() {
			return m_typeName;
		}

	};
	DERIVE_CLASS(CLispVoid, CLispObject) {
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType = gcnew CLispType("void");
#else
		static lisp_ptr s_selfType;
#endif
	public:
		CLispVoid()
			:CLispObject(s_selfType)
		{}
	};
#ifdef USE_CLR
	generic<class T>
		public ref class  CLispPrimitive :public CLispObject {
#else
	template<class T>
	class  CLispPrimitive :public CLispObject {
#endif
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
#ifdef USE_CLR
		property T value {
			T get() {
				return m_value;
			}
			void set(T v) {
				m_value = v;
			}
		}
#else
		T& value() {
			return m_value;
		}
#endif

	};
	DERIVE_CLASS(CLispInt, CLispPrimitive<int>) {
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("int");
#else
		static lisp_ptr s_selfType ;
#endif
	public:
		CLispInt(int v)
			:CLispPrimitive(v, s_selfType)
		{}

		CLispInt(STRING v)
			:CLispPrimitive(
#ifdef USE_CLR
				int::Parse(v),
#else
				atoi(v.c_str()),
#endif				
				
				s_selfType)
		{}
#ifdef USE_CLR
		String^ toString() override {
			return value.ToString();
		}
#else
		virtual std::string toString() {
			std::stringstream ss;
			ss << value();
			return ss.str();
		}
#endif
	};
	DERIVE_CLASS(CLispBool, CLispPrimitive<bool>) {
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("bool");
#else
		static lisp_ptr s_selfType;
#endif


	public:
		CLispBool(bool v)
			:CLispPrimitive(v, s_selfType)
		{}
#ifdef USE_CLR
		String^ toString() override {
			return value.ToString();
		}
#else
		virtual std::string toString() {
			std::stringstream ss;
			ss << value();
			return ss.str();
		}
#endif
	};
	DERIVE_CLASS(CLispString, CLispPrimitive<STRING>) {
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("string");
#else
		static lisp_ptr s_selfType ;
#endif

	public:
		CLispString(STRING v)
			:CLispPrimitive(v, s_selfType)
		{}
#ifdef USE_CLR
		String^ ToString() override {
			return value;
		}
#else

		virtual std::string toString() {
			return value();
		}
#endif
	};
#ifdef USE_CLR

	public ref class  CLispRange :public CLispPrimitive<CLR_Utils::Tuple<int,int>^ > {
#else		

	class  CLispRange :public CLispPrimitive<std::pair<int, int> > {
#endif

	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("range");
#else
		static lisp_ptr s_selfType;
#endif



	public:
		CLispRange(int b, int e)
#ifdef USE_CLR
			:CLispPrimitive(gcnew CLR_Utils::Tuple<int, int>(b, e), s_selfType)
#else
			: CLispPrimitive(value_type(b, e), s_selfType)
#endif
		{}
#ifdef USE_CLR
		String^ ToString()override {
			return String::Format("{0},{1}", value->first, value->second);
		}
#else		
		virtual std::string toString() {
			std::stringstream ss;
			ss << "[" << value().first << "," << value().second << "]";
			return ss.str();
		}
#endif
	};
#ifdef USE_CLR
	public ref class  CLispArray :public CLispPrimitive<ARRAY<lisp_ptr>^> {
#else
	class  CLispArray :public CLispPrimitive<std::vector<lisp_ptr>> {
#endif
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("array");
#else
		static lisp_ptr s_selfType;
#endif

	public:
		CLispArray()
			:CLispPrimitive(s_selfType)
		{}
#ifdef USE_CLR
		String^ ToString() override {
			StringBuilder^ ss = gcnew StringBuilder();
			ss->Append("[");
			for (_SIZE_T i = 0; i < value->Count; ++i) {
				lisp_ptr item = value[i];
				ss->Append(item->ToString());
				if (i < value->Count) {
					ss->Append(",");
				}

			}

			return ss->ToString();
		}
#else
		virtual std::string toString() {
			std::stringstream ss;
			ss << "[";
			for (value_type::size_type i = 0; i < value().size(); ++i) {
				lisp_ptr item = value()[i];
				ss << item->toString();
				if (i < (value().size() - 1)) {
					ss << ",";
				}
			}
			ss << "]";
			return ss.str();
		}
#endif
	};

#ifdef USE_CLR
	public delegate void NativeFunc(int, Object^);
#else
	typedef void (*NativeFunc)(int, void*);
#endif
	DERIVE_CLASS(CLispNativeFunc, CLispObject) {
#ifdef USE_CLR
		NativeFunc^ m_func;
#else
		NativeFunc m_func;
#endif

	public:
#ifdef USE_CLR

		static lisp_ptr s_selfType=gcnew CLispType("nativeFunc");
#else
		static lisp_ptr s_selfType ;

#endif

	public:
#ifdef USE_CLR
		CLispNativeFunc(NativeFunc ^ f)
#else
		CLispNativeFunc(NativeFunc f)
#endif
			: CLispObject(s_selfType),
			m_func(f)
		{}
#ifdef USE_CLR
		NativeFunc^ getFunc() {
#else
		NativeFunc getFunc() {
#endif
			return m_func;
		}
	};

	DERIVE_CLASS(CLispFuncCall, CLispObject) {
		lisp_ptr m_func;
		lisp_ptr m_args;
	public:

#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("funcCall");
#else
		static lisp_ptr s_selfType;
#endif



	public:
		CLispFuncCall(lisp_ptr func, lisp_ptr args)
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
#ifdef USE_CLR
	ref class CLispChunck;
#else
	class CLispChunck;
#endif
	DERIVE_CLASS(CLispLet, CLispObject) {
		STRING m_name;
		lisp_ptr m_value;
		_SIZE_T m_idx;
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("let");
#else
		static lisp_ptr s_selfType;
#endif

	public:
		CLispLet(STRING name, lisp_ptr v,int idx);

		STRING getName() {
			return m_name;
		}

		lisp_ptr getValue() {
			return m_value;
		}
#ifdef USE_CLR
		property _SIZE_T index {
			_SIZE_T get() {
				return m_idx;
			}
			void set(_SIZE_T i) {
				m_idx = i;
			}
		}
#else
		std::size_t& index() {
			return m_idx;
		}
#endif
	};
#ifdef USE_CLR
	typedef ARRAY<lisp_ptr> ImplementsTbl;
	public ref class  CLispChunck :public CLispObject {
#else

	typedef std::vector<lisp_ptr> ImplementsTbl;
	class  CLispChunck :public CLispObject {
#endif
		CLASS(ImplementsTbl, m_implements);
		int m_varNum;
	public:

#ifdef USE_CLR

		static lisp_ptr s_elseType=gcnew CLispType("else");
#else
		static lisp_ptr s_elseType;
#endif


	public:
		CLispChunck(lisp_ptr type)
			:CLispObject(type),
			m_varNum(0)
		{}

		REF(ImplementsTbl) getImplements() {
			return m_implements;
		}
#ifdef USE_CLR
		property int varNum {
			int get() {
				return m_varNum;
			}
			void set(int i) {
				m_varNum = i;
			}
		}
#else
		int& varNum() {
			return m_varNum;
		}
#endif
	};
	DERIVE_CLASS(CLispDefunc, CLispChunck) {
		lisp_ptr m_args;
		STRING m_name;
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("defunc");
#else
		static lisp_ptr s_selfType;
#endif


	public:
		CLispDefunc(STRING name, lisp_ptr args)
			:CLispChunck(s_selfType),
			m_args(args),
			m_name(name)
		{
		}

		STRING getName() {
			return m_name;
		}
		lisp_ptr getArgs() {
			return m_args;
		}


	};
	DERIVE_CLASS(CLispFor, CLispChunck) {
		lisp_ptr m_rangeExp;
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("for");
#else
		static lisp_ptr s_selfType;
#endif


	public:
		CLispFor(lisp_ptr range)
			:CLispChunck(s_selfType),
			m_rangeExp(range)
		{}

		lisp_ptr getRange() {
			return m_rangeExp;
		}

	};
	DERIVE_CLASS(CLispWhile, CLispChunck) {
		lisp_ptr m_contCondExp;
	public:

#ifdef USE_CLR

		static lisp_ptr s_selfType=gcnew CLispType("while");
#else
		static lisp_ptr s_selfType;
#endif

	public:
		CLispWhile(lisp_ptr contCondExp)
			:CLispChunck(s_selfType),
			m_contCondExp(contCondExp)
		{}
		lisp_ptr getContCondExp() {
			return m_contCondExp;
		}
	};

	DERIVE_CLASS(CLispIf, CLispChunck) {
		lisp_ptr m_testCondExp;
		CLASS(ImplementsTbl, m_elseIfTbl);
		CLASS(ImplementsTbl, m_elseTbl);
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("if");
		static lisp_ptr s_elseIfType=gcnew CLispType("elseif");
#else
		static lisp_ptr s_selfType;
		static lisp_ptr s_elseIfType;
#endif

	public:
		CLispIf(lisp_ptr testCondExp)
			:CLispChunck(s_selfType),
			m_testCondExp(testCondExp)
		{}
		CLispIf(lisp_ptr testCondExp, lisp_ptr type)
			:CLispChunck(type),
			m_testCondExp(testCondExp)
		{}


		lisp_ptr getTestCondExp() {
			return m_testCondExp;
		}
		REF(ImplementsTbl) getElseIfTbl() {
			return m_elseIfTbl;
		}
		REF(ImplementsTbl) getElseTbl() {
			return m_elseTbl;
		}

	};
	typedef CLispIf CLispElseIf;
	typedef CLispChunck CLispElse;
	typedef ARRAY<lisp_ptr> VariableTbl;
	DERIVE_CLASS(CLispClass, CLispChunck) {
		CLASS(VariableTbl, m_variables);
		lisp_ptr m_parent;
	public:
		CLispClass(lisp_ptr type, int vNum, lisp_ptr parent)
			:CLispChunck(type),
			m_parent(parent)
		{
#ifdef USE_CLR
			m_variables->Capacity = vNum;
#else
			m_variables.resize(vNum);
#endif
		}
		REF(VariableTbl) getVariables() {
			return m_variables;
		}
		lisp_ptr getParent() {
			return m_parent;
		}
	};

	DERIVE_CLASS(CLispVarAccess, CLispObject) {
		int m_popNum;
		lisp_ptr m_value;
	public:
#ifdef USE_CLR
		static lisp_ptr s_selfType=gcnew CLispType("var access");
#else
		static lisp_ptr s_selfType;
#endif


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
#ifdef USE_CLR
	void print(int,Object^);
	void createRange(int, Object^);

#else
	extern"C" {
		void print(int, void*);
		void createRange(int, void*);
	}
#endif
};