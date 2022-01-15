#pragma once
#include"ScriptEngineConfig.h"
#ifdef USE_CLR
using namespace System;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;
#else

#include<stack>
#include<vector>
#include<string>
#include<memory>
#endif

#include"lispTypes.h"


namespace LispScriptEngine {
#ifdef USE_CLR
	ref class CVM;
	DEF_CLASS(CCmdBase) {
	public:
		virtual void exec(REF(CVM) vm) {};
	
	};
	typedef CCmdBase^ CMD;
	DERIVE_CLASS(CNOPCmd,CCmdBase) {
	public:
		virtual void exec(REF(CVM) vm) override {}
		virtual String^ ToString() override {
			return "nop";
		}

	};
	DERIVE_CLASS(CPushIntCmd, CCmdBase) {
	public:
		int value;
	public:
		CPushIntCmd(int _value)
			:value(_value)
		{}
	public:
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("push int {0}",value);
		}

	};
	DERIVE_CLASS(CPushVarCmd, CCmdBase) {
	public:
		int popNum;
		_SIZE_T index;
	public:
		CPushVarCmd(int pn, _SIZE_T i)
			:popNum(pn),
			index(i)
		{}
	public:
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("push var {0}", index);
		}

	};

	DERIVE_CLASS(CPushStrCmd, CCmdBase) {
	public:
		String^ str;
	public:
		CPushStrCmd(String^ s)
			:str(s)
		{}
	public:
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("push str \"{0}\"", str);
		}

	};


	DERIVE_CLASS(CPushRCCmd, CCmdBase) {
	public:
		
	public:
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return "push rc";
		}

	};

	DERIVE_CLASS(CNativeCallCmd, CCmdBase) {
	public:
		int argNum;
		NativeFunc^ func;
	public:
		CNativeCallCmd(int an, NativeFunc ^ f)
			:argNum(an),
			func(f)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("native call {0}", argNum);
		}

	};
	DERIVE_CLASS(CScriptCallCmd, CCmdBase) {
	public:
		_SIZE_T jmpPos;
	public:
		CScriptCallCmd(_SIZE_T jp)
			:jmpPos(jp)
		{}
		virtual void exec(REF(CVM) vm) override {

		}
		

	};

	DERIVE_CLASS(CSetCmd, CCmdBase) {
	public:
		_SIZE_T index;
	public:
		CSetCmd(_SIZE_T i)
			:index(i)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("set {0}", index);
		}
	};
	DERIVE_CLASS(CCmpCmd, CCmdBase) {
	public:
	public:
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return "cmp";
		}
	};

	DERIVE_CLASS(CJmpCmd, CCmdBase) {
	public:
		_SIZE_T jmpPos;
	public:
		CJmpCmd(_SIZE_T jp)
			:jmpPos(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmp {0}", jmpPos);
		}
	};

	DERIVE_CLASS(CJmpEqCmd, CJmpCmd) {
	public:
	
	public:
		CJmpEqCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpe {0}", jmpPos);
		}

	};

	DERIVE_CLASS(CJmpEqLCmd, CJmpCmd) {
	public:
	public:
		CJmpEqLCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpel {0}", jmpPos);
		}
	};

	DERIVE_CLASS(CJmpEqGCmd, CJmpCmd) {
	public:
	public:
		CJmpEqGCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpeg {0}", jmpPos);
		}

	};

	DERIVE_CLASS(CJmpLCmd, CJmpCmd) {
	public:
		
	public:
		CJmpLCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpl {0}", jmpPos);
		}

	};

	DERIVE_CLASS(CJmpGCmd, CJmpCmd) {
	public:
		
	public:
		CJmpGCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpg {0}", jmpPos);
		}

	};

	DERIVE_CLASS(CJmpNCmd, CJmpCmd) {
	public:
		_SIZE_T jmpPos;
	public:
		CJmpNCmd(_SIZE_T jp)
			:CJmpCmd(jp)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("jmpn {0}", jmpPos);
		}

	};

	DERIVE_CLASS(CAddThisCmd, CCmdBase) {
	public:
		_SIZE_T vNum;
	public:
		CAddThisCmd(_SIZE_T vn)
			:vNum(vn)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("set this {0}", vNum);
		}

	};

	DERIVE_CLASS(CRetCmd, CCmdBase) {
	public:
		_SIZE_T removeArgNum;
	public:
		CRetCmd(_SIZE_T ran)
			:removeArgNum(ran)
		{}
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return String::Format("ret {0}", removeArgNum);
		}

	};

	DERIVE_CLASS(CPushRetPosCmd, CCmdBase) {
	public:
		
		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return "push ret pos";
		}

	};
	DERIVE_CLASS(CPushThisCmd, CCmdBase) {
	public:

		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return "push this";
		}

	};
	DERIVE_CLASS(CPopThisCmd, CCmdBase) {
	public:

		virtual void exec(REF(CVM) vm) override;
		virtual String^ ToString() override {
			return "pop this";
		}

	};
#else
	enum CMD_TYPE {
		NOP,
		PUSH_INT,
		PUSH_VAR,
		PUSH_STR,
		PUSH_RC,
		NATIVE_CALL,
		SCRIPT_CALL,
		SET,
		CMP,
		JMP,
		JMP_E,
		JMP_EL,
		JMP_EG,
		JMP_L,
		JMP_G,
		JMP_N,
		ADD_THIS,
		RET,
		PUSH_RET_POS,
		PUSH_THIS,
		POP_THIS,

	};
	struct CMD {
		CMD_TYPE cmdType;
		union {
			struct {
				int value;
			}push_int;
			struct {
				int popNum;
				std::size_t index;
			}push_var;
			struct {
				std::size_t sLen;
			}push_str;
			struct {
				int argNum;
				NativeFunc func;
			}native_call;
			struct {
				std::size_t index;
			}set;
			struct {
				std::size_t jmpPos;
			}jmp;
			struct {
				std::size_t removeArgNum;
			}ret;
			struct {
				std::size_t vNum;
			}add_this;
		};
	};
#endif

	DEF_CLASS(CVM) {
		
		CLASS(STACK<lisp_ptr>, m_stack);
		CLASS(ARRAY<CMD>,m_cmdBuf);
		_SIZE_T m_pos;
		lisp_ptr m_this;
		lisp_ptr m_rc;
		int m_cmpRes;
		int m_retPos;
		bool m_isRunning;
#ifdef USE_CLR
		static const char* s_lpszDefaultThisTypeName ="global";
#else
		static const char* s_lpszDefaultThisTypeName;
#endif
	public:
		CVM()
			:m_pos(0),
			m_isRunning(true),
			m_cmpRes(0),
			m_retPos(0)
		{
			//addThisCmd(lisp_ptr());
		}
		void setPos(_SIZE_T pos) {
			m_pos = pos;
		}
		_SIZE_T getCmdBufLen() {
#ifdef USE_CLR
			return m_cmdBuf->Count;
#else
			return m_cmdBuf.size();
#endif
		}
		_SIZE_T getPos() {
			return m_pos;
		}
		lisp_ptr getRC() {
			return m_rc;
		}
		lisp_ptr getThis() {
			return m_this;
		}
		void setThis(lisp_ptr p) {
			m_this = p;
		}
		lisp_ptr getTop() {
#ifdef USE_CLR
			return m_stack->Peek();
#else
			return m_stack.top();
#endif
		}
		void pop() {
#ifdef USE_CLR
			m_stack->Pop();
#else
			m_stack.pop();
#endif
		}
		void setCmpRes(int i) {
			m_cmpRes = i;
		}
		int getCmpRes() {
			return m_cmpRes;
		}

		_SIZE_T getRetPos() {
			return m_retPos;
		}
		void setRetPos(_SIZE_T i) {
			m_retPos = i;
		}

		void setIsRunning(bool f) {
			m_isRunning = f;
		}
		void addPushIntCmd(int i);
		void addPushVarCmd(_SIZE_T s, int popNum);
		void addPushStrCmd(STRING s);
		void addPushRCCmd();
		void addNativeCallCmd(int argNum,
#ifdef USE_CLR
			NativeFunc^ f
#else
			NativeFunc f
#endif
		);
		void addScriptCallCmd(_SIZE_T pos);
		void addSetCmd(int idx);
		void addCmpCmd();
		void addJmpCmd(_SIZE_T pos);
		void addJmpEqCmd(_SIZE_T pos);
		void addJmpEqLCmd(_SIZE_T pos);
		void addJmpEqGCmd(_SIZE_T pos);
		void addJmpLCmd(_SIZE_T pos);
		void addJmpGCmd(_SIZE_T pos);
		void addJmpNCmd(_SIZE_T pos);
		void addThisCmd(lisp_ptr obj);
		void addRetCmd(int n);
		void addPushRetPosCmd();
		void addPushThisCmd();
		void addPopThisCmd();
		void setJmpPos(_SIZE_T pos, _SIZE_T njmpPos);

		void dump();
#ifdef USE_CLR
		void run() {
			run(0);
		}
		void run(int nPos);
#else
		void run(int nPos = 0);
#endif
		

		void pushIntValue(int i);
		void pushBoolValue(bool f);
		void pushVarValue(int idx, int popNum);
#ifdef USE_CLR
		void pushStr(String^ str);
#else
		void pushCStr(const char* lpszStr);
#endif
		void pushLispValue(lisp_ptr p);

		void setRCValue(bool f);
		void setRCValue(int b, int e);
		void setRCValue(lisp_ptr p);
		void setThisValue(lisp_ptr p);
		int popInt();
		STRING popStr();
		lisp_ptr popObj();

	};
};