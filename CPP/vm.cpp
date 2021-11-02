#include"pch.h"
#include"vm.h"
LISP_SCRIPT_API const char* CVM::s_lpszDefaultThisTypeName= "global";
void CVM::addPushIntCmd(int i) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_INT;
	cmd.push_int.value = i;
	m_cmdBuf.push_back(cmd);
}
void  CVM::addPushVarCmd(std::size_t s, int popNum) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_VAR;
	cmd.push_var.index = s;
	cmd.push_var.popNum = popNum;
	m_cmdBuf.push_back(cmd);
}
void CVM::addPushStrCmd(std::string s) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_STR;
	cmd.push_str.sLen = (s.size() +1+ sizeof(CMD)-1)/sizeof(CMD);
	m_cmdBuf.push_back(cmd);
	std::size_t curSize = m_cmdBuf.size();
	m_cmdBuf.resize(m_cmdBuf.size() + cmd.push_str.sLen);
	strncpy_s((char*)&m_cmdBuf[curSize],(m_cmdBuf.size()-curSize)*sizeof(CMD), s.c_str(), s.size());

}
void CVM::addPushRCCmd() {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_RC;
	m_cmdBuf.push_back(cmd);

}

void CVM::addNativeCallCmd(int argNum, NativeFunc f) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::NATIVE_CALL;
	cmd.native_call.argNum = argNum;
	cmd.native_call.func = f;
	m_cmdBuf.push_back(cmd);
}
void CVM::addScriptCallCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::SCRIPT_CALL;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);

}
void CVM::addSetCmd(int idx) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::SET;
	cmd.set.index = idx;
	m_cmdBuf.push_back(cmd);
}
void CVM::addCmpCmd() {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::CMP;
	m_cmdBuf.push_back(cmd);
}
void CVM::addJmpCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);

}
void CVM::addJmpEqCmd(std::size_t pos) {

	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_E;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);

}
void CVM::addJmpEqLCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_EL;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);
}
void CVM::addJmpEqGCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_EL;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);
}
void CVM::addJmpLCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_L;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);

}
void CVM::addJmpGCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_G;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);

}
void CVM::addJmpNCmd(std::size_t pos) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::JMP_N;
	cmd.jmp.jmpPos = pos;
	m_cmdBuf.push_back(cmd);
}
void CVM::addThisCmd(lisp_ptr obj) {
	int vNum = 0;
	if (nullptr==obj) {
		addPushStrCmd(std::string( s_lpszDefaultThisTypeName));
	}
	else {
		addPushStrCmd((reinterpret_cast<CLispType*>(obj->getType().get()))->getTypeName());
		CLispChunck* pChunck = reinterpret_cast<CLispChunck*>(obj.get());
		vNum = pChunck->varNum();

	}
	CMD cmd;
	cmd.cmdType = CMD_TYPE::ADD_THIS;
	cmd.add_this.vNum = vNum;
	m_cmdBuf.push_back(cmd);
}
void CVM::addRetCmd(int n) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::RET;
	cmd.ret.removeArgNum = n;
	m_cmdBuf.push_back(cmd);
}
void CVM::addPushRetPosCmd() {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_RET_POS;
	m_cmdBuf.push_back(cmd);

}
void CVM::addPushThisCmd() {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_THIS;
	m_cmdBuf.push_back(cmd);
}
void CVM::addPopThisCmd() {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::POP_THIS;
	m_cmdBuf.push_back(cmd);

}
void CVM::setJmpPos(std::size_t pos, std::size_t njmpPos) {
	CMD& cmd=m_cmdBuf[pos];
	assert(CMD_TYPE::JMP == cmd.cmdType ||
		CMD_TYPE::JMP_E == cmd.cmdType ||
		CMD_TYPE::JMP_EL == cmd.cmdType ||
		CMD_TYPE::JMP_EG == cmd.cmdType ||
		CMD_TYPE::JMP_L == cmd.cmdType ||
		CMD_TYPE::JMP_G == cmd.cmdType ||
		CMD_TYPE::JMP_N == cmd.cmdType);

	cmd.jmp.jmpPos = njmpPos;

}
void CVM::dump() {

	m_pos = 0;
	while (m_pos < m_cmdBuf.size()) {
		CMD& cmd = m_cmdBuf[m_pos];
		switch (cmd.cmdType) {
		case CMD_TYPE::NOP:
			std::cout << std::setw(4)<<m_pos <<":nop" << std::endl;
			break;
		case CMD_TYPE::PUSH_INT:
			std::cout << std::setw(4) << m_pos << ":push int " << cmd.push_int.value << std::endl;
			break;
		case CMD_TYPE::PUSH_VAR:
			std::cout << std::setw(4) << m_pos << ":push var " << cmd.push_var.index <<" "<<cmd.push_var.popNum<< std::endl;
			break;
		case CMD_TYPE::PUSH_STR:
			std::cout << std::setw(4) << m_pos << ":push str \"";
			std::cout<< ((char*)&m_cmdBuf[++m_pos]) << "\"" << std::endl;
			m_pos += cmd.push_str.sLen;
			continue;

		case CMD_TYPE::PUSH_RC:
			std::cout << std::setw(4) << m_pos << ":push rc" << std::endl;
			break;
		case CMD_TYPE::NATIVE_CALL:
			std::cout << std::setw(4) << m_pos << ":native call " << cmd.native_call.argNum<< std::endl;
			break;
		case CMD_TYPE::SET:
			std::cout << std::setw(4) << m_pos << ":set " << cmd.set.index << std::endl;
			break;
		case CMP:
			std::cout << std::setw(4) << m_pos << ":cmp" << std::endl;
			break;
		case JMP:
			std::cout << std::setw(4) << m_pos << ":jmp " << cmd.jmp.jmpPos << std::endl;
			break;
		case JMP_E:
			std::cout << std::setw(4) << m_pos << ":jmpe " << cmd.jmp.jmpPos << std::endl;
			break;
		case JMP_EL:
			std::cout << std::setw(4) << m_pos << ":jmpel " << cmd.jmp.jmpPos << std::endl;
			break;
		case JMP_EG:
			std::cout << std::setw(4) << m_pos << ":jmpeg " << cmd.jmp.jmpPos << std::endl;
			break;
		case JMP_L:
			std::cout << std::setw(4) << m_pos << ":jmpl " << cmd.jmp.jmpPos << std::endl;
			break;
		case JMP_G:
			std::cout << std::setw(4) << m_pos << ":jmpg " << cmd.jmp.jmpPos << std::endl;
			break;

		case JMP_N:
			std::cout << std::setw(4) << m_pos << ":jmpn " << cmd.jmp.jmpPos << std::endl;
			break;
		case ADD_THIS:
			std::cout << std::setw(4) << m_pos << ":set this " <<cmd.add_this.vNum<< std::endl;
			break;
		case RET:
			std::cout << std::setw(4) << m_pos << ":ret " << cmd.ret.removeArgNum << std::endl;
			break;
		case PUSH_RET_POS:
			std::cout << std::setw(4) << m_pos << ":push ret pos" << std::endl;
			break;
		case PUSH_THIS:
			std::cout << std::setw(4) << m_pos << ":push this" << std::endl;
			break;

		case POP_THIS:
			std::cout << std::setw(4) << m_pos << ":pop this" << std::endl;
			break;
		default:
			assert(false);
		}
		++m_pos;
	}
}
void CVM::run(int nPos) {
	m_pos = nPos;
	m_isRunning = true;
	while (m_isRunning && m_pos < m_cmdBuf.size()) {
		CMD& cmd = m_cmdBuf[m_pos];
		switch (cmd.cmdType) {
		case CMD_TYPE::NOP:
			break;
		case CMD_TYPE::PUSH_INT:

			pushIntValue(cmd.push_int.value);
			break;
		case CMD_TYPE::PUSH_VAR:
			
			pushVarValue(cmd.push_var.index,cmd.push_var.popNum);
			break;
		case CMD_TYPE::PUSH_STR:
			pushCStr(((const char*)&m_cmdBuf[++m_pos]));
			m_pos += cmd.push_str.sLen;
			continue;

		case CMD_TYPE::PUSH_RC:
			pushLispValue(m_rc);
			break;
		case CMD_TYPE::NATIVE_CALL:
		{
			NativeFunc f = cmd.native_call.func;
			f(cmd.native_call.argNum,this);
		}break;
		case CMD_TYPE::SET:
		{
			CLispClass* pClass = reinterpret_cast<CLispClass*>(m_this.get());
			pClass->getVariables()[cmd.set.index]=m_stack.top();
			m_stack.pop();
		}
		break;
		case CMP:
		{
			int op1 = popInt();
			int op2 = popInt();
			m_cmpRes = op1 - op2;

		}
		break;
		case JMP: {
			m_pos = cmd.jmp.jmpPos;
			continue;
		}
				break;
		case JMP_E:
		{
			if (0 == m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}

		}
		break;
		case JMP_EL: {
			if (0 >= m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}
		}break;
		case JMP_EG:
		{
			if (0 <= m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}
		}
		break;
		case JMP_L:
		{
			if (0 > m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}
		}
		break;
		case JMP_G:
		{
			if (0 < m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}
		}
		break;

		case JMP_N: {
			if (0 != m_cmpRes) {
				m_pos = cmd.jmp.jmpPos;
				continue;

			}
		}

				  break;
		case ADD_THIS:
		{
			std::string fname = popStr();
			m_this.reset(new CLispClass(lisp_ptr(new CLispType(fname)),cmd.add_this.vNum,m_this));
		}break;
		case RET:
		{
			for (int i = 0; i <cmd.ret.removeArgNum; ++i) {
				popObj();
			}
			m_this = popObj();
			int retPos = m_retPos;
			m_retPos = popInt();
			m_pos = retPos;
			continue;
		}break;
		case PUSH_RET_POS:
			m_stack.push(lisp_ptr(new CLispInt(m_retPos)));
			break;
		case PUSH_THIS:
			m_stack.push(m_this);
			break;
		case POP_THIS:
			m_this = (reinterpret_cast<CLispClass*>(m_this.get()))->getParent();
			break;
		default:
			assert(false);
		}
		++m_pos;
	}
	m_this.reset();
}
void CVM::pushIntValue(int i) {
	m_stack.push(lisp_ptr(new CLispInt(i)));
}
void CVM::pushBoolValue(bool f) {
	m_stack.push(lisp_ptr(new CLispBool(f)));
}

void CVM::pushVarValue(int idx, int popNum) {
	CLispClass* pClass = reinterpret_cast<CLispClass*>(m_this.get());
	for (int i = 0; i < popNum; ++i) {
		pClass= reinterpret_cast<CLispClass*>(pClass->getParent().get());
	
	}
	m_stack.push(pClass->getVariables()[idx]);
}
void CVM::pushCStr(const char* lpszStr) {
	m_stack.push(lisp_ptr(new CLispString(lpszStr)));
}
void CVM::pushLispValue(lisp_ptr p) {
	m_stack.push(m_rc);
}
void CVM::setRCValue(bool f) {
	m_rc.reset(new CLispBool(f));

}
void CVM::setRCValue(int b,int e) {
	m_rc.reset(new CLispRange(b,e));

}
void CVM::setRCValue(lisp_ptr p) {
	m_rc = p;
}
void CVM::setThisValue(lisp_ptr p) {
	m_this = p;
}
int CVM::popInt() {
	lisp_ptr v = m_stack.top();
	int i = 0;
	if (CLispInt::s_selfType == v->getType()) {
		i= (reinterpret_cast<CLispInt*>(v.get()))->value();
	}
	else if (CLispBool::s_selfType == v->getType()) {
		i= ((int)(reinterpret_cast<CLispBool*>(v.get()))->value());

	}
	else {
		assert(false);
	}
	m_stack.pop();
	return i;
}
std::string CVM::popStr() {
	CLispString* pStr = dynamic_cast<CLispString*>(m_stack.top().get());
	
	assert(nullptr != pStr);
	std::string s = pStr->value();
	m_stack.pop();
	return s;
}
lisp_ptr CVM::popObj() {
	return m_stack.top();
}