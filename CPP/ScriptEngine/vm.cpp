#ifndef USE_CRL
#include<cassert>
#include<iostream>
#include<iomanip>
#endif


#include"vm.h"
namespace LispScriptEngine {
#ifdef USE_CLR
	void CPushIntCmd::exec(REF(CVM) vm) {
		vm->pushIntValue(value);
	}
	void CPushVarCmd::exec(REF(CVM) vm) {
		vm->pushVarValue(index, popNum);
	}
	void CPushStrCmd::exec(REF(CVM) vm) {
		vm->pushStr(str);
	}
	void CPushRCCmd::exec(REF(CVM) vm) {
		vm->pushLispValue(vm->getRC());
	}
	void CNativeCallCmd::exec(REF(CVM) vm) {
		func(argNum, vm);
	}
	void CSetCmd::exec(REF(CVM) vm) {
		CLispClass^ pClass = safe_cast<CLispClass^>(vm->getThis());
		pClass->getVariables()[index] = vm->getTop();
		vm->pop();

	}

	void CCmpCmd::exec(REF(CVM) vm) {
		int op1 = vm->popInt();
		int op2 = vm->popInt();
		vm->setCmpRes(op1 - op2);
	}
	void CJmpCmd::exec(REF(CVM) vm) {
		vm->setPos(jmpPos);
	}
	void CJmpEqCmd::exec(REF(CVM) vm) {
		if (0 == vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}

	}
	void CJmpEqLCmd::exec(REF(CVM) vm) {
		if (0 >= vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}
	}
	void CJmpEqGCmd::exec(REF(CVM) vm) {
		if (0 <= vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}
	}
	void CJmpLCmd::exec(REF(CVM) vm) {
		if (0 > vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}
	}
	void CJmpGCmd::exec(REF(CVM) vm) {
		if (0 < vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}
	}
	void CJmpNCmd::exec(REF(CVM) vm) {
		if (0 != vm->getCmpRes()) {

			CJmpCmd::exec(vm);
		}
	}
	void CAddThisCmd::exec(REF(CVM) vm) {
		String^ fname = vm->popStr();
		vm->setThis(gcnew CLispClass(gcnew CLispType(fname), vNum, vm->getThis()));
	}
	void CRetCmd::exec(REF(CVM) vm) {
		for (_SIZE_T i = 0; i < removeArgNum; ++i) {
			vm->popObj();
		}
		vm->setThis(vm->popObj());
		int retPos = vm->getRetPos();
		vm->setRetPos(vm->popInt());
		vm->setPos(retPos);
	}
	void CPushRetPosCmd::exec(REF(CVM) vm) {
		vm->pushIntValue(vm->getRetPos());
	}
	void CPushThisCmd::exec(REF(CVM) vm) {
		vm->pushLispValue(vm->getThis());
	}
	void CPopThisCmd::exec(REF(CVM) vm) {
		vm->setThis(safe_cast<CLispClass^>(vm->getThis())->getParent());
	}

#else
	const char* CVM::s_lpszDefaultThisTypeName = "global";


#endif
	void CVM::addPushIntCmd(int i) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushIntCmd(i));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_INT;
		cmd.push_int.value = i;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void  CVM::addPushVarCmd(std::size_t s, int popNum) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushVarCmd(popNum,s));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_VAR;
		cmd.push_var.index = s;
		cmd.push_var.popNum = popNum;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addPushStrCmd(STRING s) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushStrCmd(s));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_STR;
		cmd.push_str.sLen = (s.size() + 1 + sizeof(CMD) - 1) / sizeof(CMD);
		m_cmdBuf.push_back(cmd);
		std::size_t cur_size = m_cmdBuf.size();
		m_cmdBuf.resize(m_cmdBuf.size() + cmd.push_str.sLen);
		strncpy_s((char*)&m_cmdBuf[cur_size], (m_cmdBuf.size() - cur_size) * sizeof(CMD), s.c_str(), s.size());
#endif

	}
	void CVM::addPushRCCmd() {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushRCCmd());
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_RC;
		m_cmdBuf.push_back(cmd);
#endif

	}

#ifdef USE_CLR
	void CVM::addNativeCallCmd(int argNum, NativeFunc^ f) {
		m_cmdBuf->Add(gcnew CNativeCallCmd(argNum,f));
#else
	void CVM::addNativeCallCmd(int argNum, NativeFunc  f) {
		CMD cmd;
		cmd.cmdType = CMD_TYPE::NATIVE_CALL;
		cmd.native_call.argNum = argNum;
		cmd.native_call.func = f;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addScriptCallCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CScriptCallCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::SCRIPT_CALL;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addSetCmd(int idx) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CSetCmd(idx));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::SET;
		cmd.set.index = idx;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addCmpCmd() {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CCmpCmd());
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::CMP;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addJmpCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addJmpEqCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpEqCmd(pos));
#else

		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_E;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addJmpEqLCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpEqLCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_EL;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addJmpEqGCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpEqGCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_EL;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addJmpLCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpLCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_L;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addJmpGCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpGCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_G;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addJmpNCmd(std::size_t pos) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CJmpNCmd(pos));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::JMP_N;
		cmd.jmp.jmpPos = pos;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addThisCmd(lisp_ptr obj) {
		int vNum = 0;
		if (nullptr == obj) {
			addPushStrCmd(
#ifdef USE_CLR
				gcnew String(s_lpszDefaultThisTypeName)
#else
				std::string(s_lpszDefaultThisTypeName)
#endif		
			);
		}
		else {
			addPushStrCmd(
#ifdef USE_CLR
				(safe_cast<CLispType^>(safe_cast<CLispObject^>(obj)->getType()))
#else
				(reinterpret_cast<CLispType*>(obj->getType().get()))
#endif			
				->getTypeName());
			PTR(CLispChunck) pChunck =
#ifdef USE_CLR
				safe_cast<CLispChunck^>(obj);
#else
				reinterpret_cast<CLispChunck*>(obj.get());
#endif
			vNum =
#ifdef USE_CLR
				pChunck->varNum;
#else
				pChunck->varNum();
#endif
		}
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CAddThisCmd(vNum));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::ADD_THIS;
		cmd.add_this.vNum = vNum;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addRetCmd(int n) {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CRetCmd(n));
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::RET;
		cmd.ret.removeArgNum = n;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addPushRetPosCmd() {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushRetPosCmd());
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_RET_POS;
		m_cmdBuf.push_back(cmd);
#endif

	}
	void CVM::addPushThisCmd() {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPushThisCmd());
#else
		CMD cmd;
		cmd.cmdType = CMD_TYPE::PUSH_THIS;
		m_cmdBuf.push_back(cmd);
#endif
	}
	void CVM::addPopThisCmd() {
#ifdef USE_CLR
		m_cmdBuf->Add(gcnew CPopThisCmd());
#else

		CMD cmd;
		cmd.cmdType = CMD_TYPE::POP_THIS;
		m_cmdBuf.push_back(cmd);

#endif
	}
	void CVM::setJmpPos(std::size_t pos, std::size_t njmpPos) {
#ifdef USE_CLR
		CJmpCmd^ cmd = safe_cast<CJmpCmd^>(m_cmdBuf[pos]);
		cmd->jmpPos = pos;
#else
		CMD& cmd = m_cmdBuf[pos];
		assert(CMD_TYPE::JMP == cmd.cmdType ||
			CMD_TYPE::JMP_E == cmd.cmdType ||
			CMD_TYPE::JMP_EL == cmd.cmdType ||
			CMD_TYPE::JMP_EG == cmd.cmdType ||
			CMD_TYPE::JMP_L == cmd.cmdType ||
			CMD_TYPE::JMP_G == cmd.cmdType ||
			CMD_TYPE::JMP_N == cmd.cmdType);

		cmd.jmp.jmpPos = njmpPos;
#endif

	}
	void CVM::dump() {

		m_pos = 0;
		while (
			m_pos <

#ifdef USE_CLR
			m_cmdBuf->Count
#else
			m_cmdBuf.size()
#endif			
		) {
#ifdef USE_CLR
			CCmdBase^ cmd = m_cmdBuf[m_pos];
			System::Console::WriteLine("{0:4d}:{1}",m_pos,cmd->ToString());
#else
			CMD& cmd = m_cmdBuf[m_pos];
			switch (cmd.cmdType) {
			case CMD_TYPE::NOP:
				std::cout << std::setw(4) << m_pos << ":nop" << std::endl;
				break;
			case CMD_TYPE::PUSH_INT:
				std::cout << std::setw(4) << m_pos << ":push int " << cmd.push_int.value << std::endl;
				break;
			case CMD_TYPE::PUSH_VAR:
				std::cout << std::setw(4) << m_pos << ":push var " << cmd.push_var.index << " " << cmd.push_var.popNum << std::endl;
				break;
			case CMD_TYPE::PUSH_STR:
				std::cout << std::setw(4) << m_pos << ":push str \"";
				std::cout << ((char*)&m_cmdBuf[++m_pos]) << "\"" << std::endl;
				m_pos += cmd.push_str.sLen;
				continue;

			case CMD_TYPE::PUSH_RC:
				std::cout << std::setw(4) << m_pos << ":push rc" << std::endl;
				break;
			case CMD_TYPE::NATIVE_CALL:
				std::cout << std::setw(4) << m_pos << ":native call " << cmd.native_call.argNum << std::endl;
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
				std::cout << std::setw(4) << m_pos << ":set this " << cmd.add_this.vNum << std::endl;
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
#endif
			++m_pos;
		}
	}
	void CVM::run(int nPos) {
		m_pos = nPos;
		m_isRunning = true;
		while (m_isRunning && m_pos <
#ifdef USE_CLR
			m_cmdBuf->Count
#else
			m_cmdBuf.size()
#endif		
			
			) {
#ifdef USE_CLR
			CCmdBase^ cmd = m_cmdBuf[m_pos];
			cmd->exec(this);
#else
			CMD& cmd = m_cmdBuf[m_pos];
			switch (cmd.cmdType) {
			case CMD_TYPE::NOP:
				break;
			case CMD_TYPE::PUSH_INT:

				pushIntValue(cmd.push_int.value);
				break;
			case CMD_TYPE::PUSH_VAR:

				pushVarValue(cmd.push_var.index, cmd.push_var.popNum);
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
				f(cmd.native_call.argNum, this);
			}break;
			case CMD_TYPE::SET:
			{
				CLispClass* pClass = reinterpret_cast<CLispClass*>(m_this.get());
				pClass->getVariables()[cmd.set.index] = m_stack.top();
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
				m_this.reset(new CLispClass(lisp_ptr(new CLispType(fname)), cmd.add_this.vNum, m_this));
			}break;
			case RET:
			{
				for (std::size_t i = 0; i < cmd.ret.removeArgNum; ++i) {
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
#endif
			++m_pos;
		}
#ifdef USE_CLR
		m_this = nullptr;
#else
		m_this.reset();
#endif
	}
	void CVM::pushIntValue(int i) {
#ifdef USE_CLR
		m_stack->Push((gcnew CLispInt(i)));
#else
		m_stack.push(lisp_ptr(new CLispInt(i)));
#endif
	}
	void CVM::pushBoolValue(bool f) {
#ifdef USE_CLR
		m_stack->Push((gcnew CLispBool(f)));
#else
		m_stack.push(lisp_ptr(new CLispBool(f)));
#endif
	}

	void CVM::pushVarValue(int idx, int popNum) {
#ifdef USE_CLR
		CLispClass^ pClass = safe_cast<CLispClass^>(m_this);
		for (int i = 0; i < popNum; ++i) {
			pClass = safe_cast<CLispClass^>(pClass->getParent());

		}
		m_stack->Push(pClass->getVariables()[idx]);
#else
		CLispClass* pClass = reinterpret_cast<CLispClass*>(m_this.get());
		for (int i = 0; i < popNum; ++i) {
			pClass = reinterpret_cast<CLispClass*>(pClass->getParent().get());

		}
		m_stack.push(pClass->getVariables()[idx]);
#endif
	}
#ifdef USE_CLR
	void CVM::pushStr(String^ lpszStr) {
		m_stack->Push((gcnew CLispString(lpszStr)));
#else
	void CVM::pushCStr(const char* lpszStr) {
		m_stack.push(lisp_ptr(new CLispString(lpszStr)));
#endif
	}
	void CVM::pushLispValue(lisp_ptr p) {
#ifdef USE_CLR
		m_stack->Push(p);
#else
		m_stack.push(p);
#endif
	}
	void CVM::setRCValue(bool f) {
#ifdef USE_CLR
		m_rc=(gcnew CLispBool(f));
#else
		m_rc.reset(new CLispBool(f));
#endif

	}
	void CVM::setRCValue(int b, int e) {
#ifdef USE_CLR
		m_rc=(gcnew CLispRange(b, e));
#else
		m_rc.reset(new CLispRange(b, e));
#endif

	}
	void CVM::setRCValue(lisp_ptr p) {
		m_rc = p;
	}
	void CVM::setThisValue(lisp_ptr p) {
		m_this = p;
	}
	int CVM::popInt() {
		lisp_ptr v =
#ifdef USE_CLR
			m_stack->Peek();
#else

			m_stack.top();
#endif
		int i = 0;
		if (CLispInt::s_selfType == v->getType()) {
			i =
#ifdef USE_CLR
				safe_cast<CLispInt^>(v)->value;
#else

				(reinterpret_cast<CLispInt*>(v.get()))->value();
#endif
		}
		else if (CLispBool::s_selfType == v->getType()) {
			i = ((int)
#ifdef USE_CLR
			(safe_cast<CLispBool^>(v))->value);
#else
			(reinterpret_cast<CLispBool*>(v.get()))->value());
#endif

		}
		else {
			assert(false);
		}
#ifdef USE_CLR
		m_stack->Pop();
#else
		m_stack.pop();
#endif
		return i;
	}
	STRING CVM::popStr() {

		PTR(CLispString) pStr =
#ifdef USE_CLR
			safe_cast<CLispString^>(m_stack->Peek());
#else
			dynamic_cast<CLispString*>(m_stack.top().get());
#endif
		ASSERT(nullptr != pStr);
		STRING s = 
#ifdef USE_CLR
			pStr->value;
#else
			pStr->value();
#endif
#ifdef USE_CLR
		m_stack->Pop();
#else
		m_stack.pop();
#endif
		return s;
	}
	lisp_ptr CVM::popObj() {
		lisp_ptr p=
#ifdef USE_CLR
		m_stack->Peek();
		m_stack->Pop();
#else
		m_stack.top();
		m_stack.pop();
#endif
		return p;
	}
};