#include"pch.h"
#include"vm.h"
void CVM::addPushIntCmd(int i) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_INT;
	cmd.push_int.value = i;
	m_cmdBuf.push_back(cmd);
}
void  CVM::addPushVarCmd(std::size_t s) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::PUSH_VAR;
	cmd.push_var.index = s;
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

void CVM::addNativeCallCmd(NativeFunc f) {
	CMD cmd;
	cmd.cmdType = CMD_TYPE::NATIVE_CALL;
	cmd.native_call.func = f;
	m_cmdBuf.push_back(cmd);
}