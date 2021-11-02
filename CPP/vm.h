#pragma once
#include"lispTypes.h"
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
class LISP_SCRIPT_API CVM {

	std::stack<lisp_ptr> m_stack;
	std::vector<CMD> m_cmdBuf;
	std::size_t m_pos;
	lisp_ptr m_this;
	lisp_ptr m_rc;
	int m_cmpRes;
	int m_retPos;
	bool m_isRunning;
	static const char* s_lpszDefaultThisTypeName;
public:
	CVM()
		:m_pos(0),
 		 m_isRunning(true)
	{
		//addThisCmd(lisp_ptr());
	}
	void setPos(std::size_t pos) {
		m_pos = pos;
	}
	std::size_t getCmdBufLen() {
		return m_cmdBuf.size();
	}
	std::size_t getPos() {
		return m_pos;
	}

	void setIsRunning(bool f) {
		m_isRunning = f;
	}
	void addPushIntCmd(int i);
	void addPushVarCmd(std::size_t s,int popNum);
	void addPushStrCmd(std::string s);
	void addPushRCCmd();
	void addNativeCallCmd(int argNum,NativeFunc f);
	void addScriptCallCmd(std::size_t pos);
	void addSetCmd(int idx);
	void addCmpCmd();
	void addJmpCmd(std::size_t pos);
	void addJmpEqCmd(std::size_t pos);
	void addJmpEqLCmd(std::size_t pos);
	void addJmpEqGCmd(std::size_t pos);
	void addJmpLCmd(std::size_t pos);
	void addJmpGCmd(std::size_t pos);
	void addJmpNCmd(std::size_t pos);
	void addThisCmd(lisp_ptr obj);
	void addRetCmd(int n);
	void addPushRetPosCmd();
	void addPushThisCmd();
	void addPopThisCmd();
	void setJmpPos(std::size_t pos,std::size_t njmpPos);
	
	void dump();
	void run(int nPos=0);

	void pushIntValue(int i);
	void pushBoolValue(bool f);
	void pushVarValue(int idx,int popNum);
	void pushCStr(const char* lpszStr);
	void pushLispValue(lisp_ptr p);

	void setRCValue(bool f);
	void setRCValue(int b, int e);
	void setRCValue(lisp_ptr p);
	void setThisValue(lisp_ptr p);
	int popInt();
	std::string popStr();
	lisp_ptr popObj();

};