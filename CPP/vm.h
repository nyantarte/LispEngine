#pragma once
#include"lispTypes.h"
enum CMD_TYPE {
	NOP,
	PUSH_INT,
	PUSH_VAR,
	PUSH_STR,
	PUSH_RC,
	NATIVE_CALL,
};
struct CMD {
	CMD_TYPE cmdType;
	union {
		struct {
			int value;
		}push_int;
		struct {
			std::size_t index;
		}push_var;
		struct {
			std::size_t sLen;
		}push_str;
		struct {
			NativeFunc func;
		}native_call;
		
	};
};
class CVM {

	std::stack<lisp_ptr> m_stack;
	std::vector<CMD> m_cmdBuf;
	std::size_t m_pos;
public:

	void setPos(std::size_t pos) {
		m_pos = pos;
	}
	void addPushIntCmd(int i);
	void addPushVarCmd(std::size_t s);
	void addPushStrCmd(std::string s);
	void addPushRCCmd();
	void addNativeCallCmd(NativeFunc f);
	
};